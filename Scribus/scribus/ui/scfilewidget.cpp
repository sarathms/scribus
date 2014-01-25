/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDesktopServices>
#include <QListView>
#include <QPushButton>
#include <QUrl>

#include "scescapecatcher.h"
#include "scfilewidget.h"


ScFileWidget::ScFileWidget(QWidget * parent) : QFileDialog(parent, Qt::Widget)
{
	setOption(QFileDialog::DontUseNativeDialog);
	setSizeGripEnabled(false);
	setModal(false);
	setViewMode(QFileDialog::List);
	setWindowFlags(Qt::Widget);

#ifdef Q_OS_MAC
	QList<QUrl> urls;
	QUrl computer(QUrl::fromLocalFile(QLatin1String("")));
	if (!urls.contains(computer))
		urls << computer;
	QUrl volumes(QUrl::fromLocalFile("/Volumes"));
	if (!urls.contains(volumes))
		urls << volumes;
	//desktop too?	QUrl computer(QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation)));
	setSidebarUrls(urls);
#endif

	ScEscapeCatcher* keyCatcher = new ScEscapeCatcher(this);
	QList<QListView *> lv = findChildren<QListView *>();
	QListIterator<QListView *> lvi(lv);
	while (lvi.hasNext())
		lvi.next()->installEventFilter(keyCatcher);
	connect(keyCatcher, SIGNAL(escapePressed()), this, SLOT(reject()));

	QList<QPushButton *> b = findChildren<QPushButton *>();
	QListIterator<QPushButton *> i(b);
	while (i.hasNext())
		i.next()->setVisible(false);
	setMinimumSize(QSize(480, 310));
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QString ScFileWidget::selectedFile()
{
	QStringList l(selectedFiles());
	if (l.count() == 0)
		return QString();
	return l.at(0);
}

/* Hack to make the previews in our file dialogs useable again
   needed e.g on OpenSuse patched Qt versions */
void ScFileWidget::accept()
{
#ifndef Q_OS_LINUX
	QFileDialog::accept();
#endif
}
