#ifndef SXWDIA_H
#define SXWDIA_H

#include "config.h"

#ifdef HAVE_XML

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qstringlist.h>

class SxwDialog : public QDialog
{
	Q_OBJECT

public:
	SxwDialog(bool update, bool prefix);
	~SxwDialog();
	bool shouldUpdate();
	bool usePrefix();
	bool askAgain();
private:
	QCheckBox* updateCheck;
	QCheckBox* prefixCheck;
	QCheckBox* doNotAskCheck;
	QPushButton* okButton;
};

#endif // HAVE_XML

#endif // CSVDIA_H
