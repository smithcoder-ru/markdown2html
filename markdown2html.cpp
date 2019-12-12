#include "markdown2html.h"
#include <QStringBuilder>

enum Sections
{
	None,
	Text,
	Header,
	Code
};

QString markdown2html(const QString& source)
{
	Sections section = None;
	Sections wait = None;
	int level = 0;
	QString res;
	for(int i = 0; i < source.size(); i++)
	{
		QChar ch = source.at(i);

		if (section == None)
		{
			if (ch == "#")
			{
				level++;
				wait = Header;
			}
			else if (ch == "\t")
			{
				section = Code;
				res.append("<code>");
			}
			else if (ch == " " && wait == Header)
			{
				section = Header;
				wait = None;
				res.append("<h" % QString::number(level) %  ">");
			}
			else
			{
				if (ch != "\n")
				{
					section = Text;
					res.append("<p>").append(ch);
				}
			}
		}
		else if (ch == "\n")
		{
			if (section == Text)
				res.append("</p>\n");
			else if (section == Code)
				res.append("</code>\n");
			else if (section == Header)
			{
				res.append("</h" % QString::number(level) % ">\n");
				level = 0;
			}

			section = None;
		}
		else
			res.append(ch);
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
