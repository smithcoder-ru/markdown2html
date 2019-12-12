
#include "markdown2html.h"
#include <QFile>
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc < 2)
		return -1;
	QFile file(argv[1]);
	if (!file.open(QIODevice::ReadOnly))
		return -2;

	QString str = file.readAll();
	file.close();
	QString res;

	try {
		res = markdown2html(str);
	} catch (...) {
		return -3;
	}

	std::cout<<res.toStdString();

	return 0;
}

