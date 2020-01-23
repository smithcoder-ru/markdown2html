#include "markdown2html.h"
#include <QStringBuilder>

enum Sections
{
	None,
	Text,
	Header,
	Code
};

enum SubText
{
	PlainText,
	CursivOpen,
	Cursiv,
	CursivClose,
	BoldOpen,
	Bold,
	CloseBold
};

QString markdown2html(const QString& source)
{
	Sections section = None;
	Sections wait = None;
	int sectionlevel = 0;
	SubText subtext = PlainText;

	QString res;
	for (int i = 0; i < source.size(); i++)
	{
		QChar ch = source.at(i);

		if (ch == '<')
		{
			res.append("&lt;");
			continue;
		}

		if (wait == Code && section == Code)
		{
			wait = None;
			if (ch == "\t")
			{
				res.append("<br/>\n");
				continue;
			}
			res.append("</code>\n");
			section = None;
		}

		if (section == None)
		{
			if (ch == "#")
			{
				sectionlevel++;
				wait = Header;
				continue;
			}
			else if (ch == "\t")
			{
				section = Code;
				res.append("<code>");
				wait = None;
				continue;
			}
			else if (ch == " " && wait == Header)
			{
				section = Header;
				wait = None;
				res.append("<h" % QString::number(sectionlevel) %  ">");
				continue;
			}
			else
			{
				if (ch != "\n")
				{
					section = Text;
					res.append("<p>");
				}
			}
		}

		if (ch != '*')
		{
			if (subtext == CursivOpen)
			{
				res.append("<i>");
				subtext = Cursiv;
			}
			else if(subtext == BoldOpen)
			{
				res.append("<b>");
				subtext = Bold;
			}
			if (subtext == CursivClose)
			{
				res.append("</i>");
				subtext = PlainText;
			}
			else if(subtext == CloseBold)
			{
				res.append("</b>");
				subtext = PlainText;
			}
		}

		if (ch == "\n")
		{
			if (section == Text)
			{
				res.append("</p>\n");
				section = None;
			}
			else if (section == Code)
			{
				wait = Code;
			}
			else if (section == Header)
			{
				res.append("</h" % QString::number(sectionlevel) % ">\n");
				sectionlevel = 0;
				section = None;
			}
		}
		else
		{
			if (ch == '*')
			{
				if (subtext == PlainText)
				{
					subtext = CursivOpen;
					continue;
				}
				else if (subtext == CursivOpen)
				{
					subtext = BoldOpen;
					continue;
				}
				else if (subtext == Cursiv)
				{
					subtext = CursivClose;
					continue;
				}
				else if (subtext == Bold || subtext == CloseBold)
				{
					subtext = CloseBold;
					continue;
				}
			}

			res.append(ch);
		}
	}

	if (section == Text)
		res.append("</p>\n");
	else if (section == Code)
		res.append("</code>\n");
	else if (section == Header)
		res.append("</h1>\n");
	section = None;

	return res;
}
