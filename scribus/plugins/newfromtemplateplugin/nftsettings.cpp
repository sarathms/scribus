/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#include "nftsettings.h"

nftsettings::nftsettings(QString guilang, QString templateDir)
{
	lang = guilang;
	scribusShare = PREL;
	scribusShare += "/share/scribus";
	scribusUserHome = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus");
	userTemplateDir = templateDir;
	if (userTemplateDir.right(1) == "/")
		userTemplateDir = userTemplateDir.left(userTemplateDir.length() - 1);
	read();
}

void nftsettings::read()
{
	handler = new nftrcreader(&templates,scribusUserHome);
	reader = new QXmlSimpleReader();
	reader->setContentHandler(handler);

	addTemplates(scribusShare+"/templates");
	addTemplates(scribusUserHome+"/templates");
	if ((userTemplateDir != NULL) && (userTemplateDir != ""))
		addTemplates(userTemplateDir);
}

void nftsettings::addTemplates(QString dir) // dir will be searched for a sub folder called templates
{
	// Add templates from the dir itself
	QString tmplFile = findTemplateXml(dir);
	QFile* tmplxml = new QFile(QDir::convertSeparators(tmplFile));
	handler->setSourceDir(dir);
	handler->setSourceFile(tmplFile);
	if (tmplxml->exists())
	{
		QXmlInputSource* source = new QXmlInputSource(tmplxml);
		reader->parse(source);
		delete source;
	}
	delete tmplxml;
	
	
	// And from all the subdirectories. template.xml file is only searched one dir level deeper than the dir
	QDir tmpldir(dir);
	if (tmpldir.exists())
	{
		tmpldir.setFilter(QDir::Dirs);
		QStringList dirs = tmpldir.entryList();
		for (uint i = 0; i < dirs.size(); ++i)
		{
			if ((dirs[i] != ".") && (dirs[i] != "..")) {
				tmplFile = findTemplateXml(dir + "/" + dirs[i]);
				QFile* tmplxml = new QFile(QDir::convertSeparators(tmplFile));
				handler->setSourceDir(dir+"/"+dirs[i]);
				handler->setSourceFile(tmplFile);
				if (tmplxml->exists())
				{
					QXmlInputSource* source = new QXmlInputSource(tmplxml);
					reader->parse(source);
					delete source;
				}
				delete tmplxml;
			}
		}
	}
}

QString nftsettings::findTemplateXml(QString dir)
{
	QString tmp = dir + "/template." + lang + ".xml";
	if (QFile(tmp).exists())
		return tmp;

	if (lang.length() > 2)
	{
		tmp = dir + "/template." + lang.left(2) + ".xml";
		if (QFile(tmp).exists())
			return tmp;
	}
	return dir + "/template.xml";	
}

nftsettings::~ nftsettings()
{
	delete reader;
	delete handler;
	for (uint i = 0; i < templates.size(); ++i)
	{
		if (templates[i] != NULL)
			delete templates[i];
	}
}
