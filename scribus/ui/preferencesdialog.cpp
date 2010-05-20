/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QPushButton>
#include <QLayout>
#include <QListWidget>
#include <QVBoxLayout>

#include "ui/preferencesdialog.h"

#include "commonstrings.h"
#include "pluginmanager.h"
#include "prefsmanager.h"
#include "scplugin.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "units.h"
#include "util_icon.h"

PreferencesDialog::PreferencesDialog(QWidget* parent, ApplicationPrefs& prefsData, ScribusDoc* doc)
	: QDialog(parent),
	prefs_ColorManagement(0),
	prefs_UserInterface(0),
	prefs_DocumentInformation(0),
	prefs_DocumentSections(0),
	prefs_DocumentSetup(0),
	prefs_Display(0),
	prefs_ExternalTools(0),
	prefs_Guides(0),
	prefs_KeyboardShortcuts(0),
	prefs_Paths(0),
	prefs_Printer(0),
	prefs_Hyphenator(0),
	prefs_Miscellaneous(0),
	prefs_Plugins(0),prefs_PreflightVerifier(0),
	prefs_Scrapbook(0),
	prefs_Typography(0),
	prefs_ItemTools(0),
	prefs_OperatorTools(0),
	prefs_Fonts(0),
	prefs_TableOfContents(0),
	prefs_PDFExport(0),
	prefs_DocumentItemAttributes(0),
	prefs_ImageCache(0),
	counter(0),
	m_Doc(doc)
{
	setupUi(this);
	setObjectName(QString::fromLocal8Bit("PreferencesDialog"));
	setupListWidget();
	while (prefsStackWidget->currentWidget()!=0)
		prefsStackWidget->removeWidget(prefsStackWidget->currentWidget());
	if(doc)
	{
		setWindowTitle( tr("Document Setup") );
		exportButton->hide();
		defaultsButton->hide();
	}
	if (!doc)
	{
		prefs_UserInterface = new Prefs_UserInterface(prefsStackWidget, m_Doc);
		addItem( tr("User Interface"), loadIcon("scribus.png"), prefs_UserInterface);
		prefs_Paths = new Prefs_Paths(prefsStackWidget, m_Doc);
		addItem( tr("Paths"), loadIcon("22/system-file-manager.png"), prefs_Paths);
	}
	prefs_DocumentSetup = new Prefs_DocumentSetup(prefsStackWidget, m_Doc);
	addItem( tr("Document Setup"), loadIcon("scribusdoc.png"), prefs_DocumentSetup);
	if (doc)
	{
		prefs_DocumentInformation = new Prefs_DocumentInformation(prefsStackWidget, m_Doc);
		addItem( tr("Document Information"), loadIcon("documentinfo32.png"), prefs_DocumentInformation);
	}
	prefs_Guides = new Prefs_Guides(prefsStackWidget, m_Doc);
	addItem( tr("Guides"), loadIcon("guides.png"), prefs_Guides);
	prefs_Typography = new Prefs_Typography(prefsStackWidget, m_Doc);
	addItem( tr("Typography"), loadIcon("22/draw-text.png"), prefs_Typography);
	prefs_ItemTools = new Prefs_ItemTools(prefsStackWidget, m_Doc);
	addItem( tr("Item Tools"), loadIcon("tools.png"), prefs_ItemTools);
	prefs_OperatorTools = new Prefs_OperatorTools(prefsStackWidget, m_Doc);
	addItem( tr("Operator Tools"), loadIcon("tools.png"), prefs_OperatorTools);
	prefs_Hyphenator = new Prefs_Hyphenator(prefsStackWidget, m_Doc);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), prefs_Hyphenator);
	prefs_Fonts = new Prefs_Fonts(prefsStackWidget, m_Doc);
	addItem( tr("Fonts"), loadIcon("22/preferences-desktop-font.png"), prefs_Fonts);
	prefs_ColorManagement = new Prefs_ColorManagement(prefsStackWidget, m_Doc);
	addItem( tr("Color Management"), loadIcon("blend.png"), prefs_ColorManagement);
	prefs_Printer = new Prefs_Printer(prefsStackWidget, m_Doc);
	addItem( tr("Printer"), loadIcon("22/printer.png"), prefs_Printer);
	prefs_PDFExport = new Prefs_PDFExport(prefsStackWidget, m_Doc);
	addItem( tr("PDF Export"), loadIcon("acroread22.png"), prefs_PDFExport);
	prefs_PreflightVerifier = new Prefs_PreflightVerifier(prefsStackWidget, m_Doc);
	addItem( tr("Preflight Verifier"), loadIcon("checkdoc.png"), prefs_PreflightVerifier);
	prefs_DocumentItemAttributes = new Prefs_DocumentItemAttributes(prefsStackWidget, m_Doc);
	addItem( tr("Document Item Attributes"), loadIcon("docattributes.png"), prefs_DocumentItemAttributes);
	prefs_TableOfContents = new Prefs_TableOfContents(prefsStackWidget, m_Doc);
	addItem( tr("Tables of Contents"), loadIcon("tabtocindex.png"), prefs_TableOfContents);
	if (doc)
	{
		prefs_DocumentSections  = new Prefs_DocumentSections(prefsStackWidget, m_Doc);
		addItem( tr("Sections"), loadIcon("tabtocindex.png"), prefs_DocumentSections);
	}
	if (!doc)
	{
		prefs_KeyboardShortcuts = new Prefs_KeyboardShortcuts(prefsStackWidget, m_Doc);
		addItem( tr("Keyboard Shortcuts"), loadIcon("22/preferences-desktop-keyboard-shortcuts.png"), prefs_KeyboardShortcuts);
		prefs_Scrapbook = new Prefs_Scrapbook(prefsStackWidget, m_Doc);
		addItem( tr("Scrapbook"), loadIcon("scrap.png"), prefs_Scrapbook);
	}
	prefs_Display = new Prefs_Display(prefsStackWidget, m_Doc);
	addItem( tr("Display"), loadIcon("22/video-display.png"), prefs_Display);
	if (!doc)
	{
		prefs_ExternalTools = new Prefs_ExternalTools(prefsStackWidget, m_Doc);
		addItem( tr("External Tools"), loadIcon("externaltools.png"), prefs_ExternalTools);
		prefs_Miscellaneous = new Prefs_Miscellaneous(prefsStackWidget, m_Doc);
		addItem( tr("Miscellaneous"), loadIcon("misc.png"), prefs_Miscellaneous);
		prefs_Plugins = new Prefs_Plugins(prefsStackWidget, m_Doc);
		addItem( tr("Plugins"), loadIcon("plugins.png"), prefs_Plugins);
		prefs_ImageCache = new Prefs_ImageCache(prefsStackWidget, m_Doc);
		addItem( tr("Image Cache"), loadIcon("22/image-x-generic.png"), prefs_ImageCache);
	}
	arrangeIcons();

	//**********

	if (preferencesTypeList->count()>0)
	{
		preferencesTypeList->item(0)->setSelected(true);
		itemSelected(preferencesTypeList->item(0));
	}

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));
	connect(preferencesTypeList, SIGNAL(itemSelectionChanged()), this, SLOT(newItemSelected()));

	localPrefs=prefsData;
	initPreferenceValues();
	setupGui();
	if (!doc)
		addPlugins();
}


PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::restoreDefaults()
{
	prefsManager->initDefaults();
	setupGui();
}

void PreferencesDialog::initPreferenceValues()
{
	prefsManager=PrefsManager::instance();
	mainWin = (ScribusMainWindow*)parent();
	docUnitIndex = localPrefs.docSetupPrefs.docUnitIndex;
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
}

void PreferencesDialog::setupGui()
{
	if (prefs_UserInterface)
		prefs_UserInterface->restoreDefaults(&localPrefs);
	if (prefs_Paths)
		prefs_Paths->restoreDefaults(&localPrefs);
	prefs_DocumentSetup->restoreDefaults(&localPrefs);
	if (prefs_DocumentInformation)
		prefs_DocumentInformation->restoreDefaults(&localPrefs);
	prefs_Guides->restoreDefaults(&localPrefs);
	prefs_Typography->restoreDefaults(&localPrefs);
	prefs_ItemTools->restoreDefaults(&localPrefs);
	prefs_OperatorTools->restoreDefaults(&localPrefs);
	prefs_Hyphenator->restoreDefaults(&localPrefs);
	prefs_Fonts->restoreDefaults(&localPrefs);
	prefs_Printer->restoreDefaults(&localPrefs);
	prefs_PDFExport->restoreDefaults(&localPrefs, ScCore->PDFXProfiles);
	prefs_PreflightVerifier->restoreDefaults(&localPrefs);
	prefs_DocumentItemAttributes->restoreDefaults(&localPrefs);
	prefs_TableOfContents->restoreDefaults(&localPrefs);
	if (prefs_DocumentSections)
		prefs_DocumentSections->restoreDefaults(&localPrefs);
	if (prefs_KeyboardShortcuts)
		prefs_KeyboardShortcuts->restoreDefaults(&localPrefs);
	prefs_ColorManagement->restoreDefaults(&localPrefs);
	prefs_ColorManagement->setProfiles(&localPrefs, &ScCore->InputProfiles, &ScCore->InputProfilesCMYK, &ScCore->PrinterProfiles, &ScCore->MonitorProfiles);
	if (prefs_Scrapbook)
		prefs_Scrapbook->restoreDefaults(&localPrefs);
	prefs_Display->restoreDefaults(&localPrefs);
	if (prefs_ExternalTools)
		prefs_ExternalTools->restoreDefaults(&localPrefs);
	if (prefs_Plugins)
		prefs_Plugins->restoreDefaults(&localPrefs);
	if (prefs_Miscellaneous)
		prefs_Miscellaneous->restoreDefaults(&localPrefs);
	if (prefs_ImageCache)
		prefs_ImageCache->restoreDefaults(&localPrefs);
}


void PreferencesDialog::saveGuiToPrefs()
{
	if (prefs_UserInterface)
		prefs_UserInterface->saveGuiToPrefs(&localPrefs);
	if (prefs_Paths)
		prefs_Paths->saveGuiToPrefs(&localPrefs);
	prefs_DocumentSetup->saveGuiToPrefs(&localPrefs);
	if (prefs_DocumentInformation)
		prefs_DocumentInformation->saveGuiToPrefs(&localPrefs);
	prefs_Guides->saveGuiToPrefs(&localPrefs);
	prefs_Typography->saveGuiToPrefs(&localPrefs);
	prefs_ItemTools->saveGuiToPrefs(&localPrefs);
	prefs_OperatorTools->saveGuiToPrefs(&localPrefs);
	prefs_Hyphenator->saveGuiToPrefs(&localPrefs);
	prefs_Fonts->saveGuiToPrefs(&localPrefs);
	prefs_Printer->saveGuiToPrefs(&localPrefs);
	prefs_PDFExport->saveGuiToPrefs(&localPrefs);
	prefs_PreflightVerifier->saveGuiToPrefs(&localPrefs);
	prefs_DocumentItemAttributes->saveGuiToPrefs(&localPrefs);
	prefs_TableOfContents->saveGuiToPrefs(&localPrefs);
	if (prefs_DocumentSections)
		prefs_DocumentSections->saveGuiToPrefs(&localPrefs);
	if (prefs_KeyboardShortcuts)
		prefs_KeyboardShortcuts->saveGuiToPrefs(&localPrefs);
	prefs_ColorManagement->saveGuiToPrefs(&localPrefs);
	if (prefs_Scrapbook)
		prefs_Scrapbook->saveGuiToPrefs(&localPrefs);
	prefs_Display->saveGuiToPrefs(&localPrefs);
	if (prefs_ExternalTools)
		prefs_ExternalTools->saveGuiToPrefs(&localPrefs);
	if (prefs_Plugins)
		prefs_Plugins->saveGuiToPrefs(&localPrefs);
	if (prefs_Miscellaneous)
		prefs_Miscellaneous->saveGuiToPrefs(&localPrefs);
	if (prefs_ImageCache)
		prefs_ImageCache->saveGuiToPrefs(&localPrefs);
}

void PreferencesDialog::applyButtonClicked()
{
	Prefs_Pane* pp=qobject_cast<Prefs_Pane *>(prefsStackWidget->currentWidget());
	if (pp)
		pp->saveGuiToPrefs(&localPrefs);
}

void PreferencesDialog::accept()
{
	saveGuiToPrefs();
	emit accepted();
	return QDialog::accept();
}

void PreferencesDialog::setupListWidget()
{
	preferencesTypeList->setDragEnabled(false);
	preferencesTypeList->setViewMode(QListView::ListMode);
	preferencesTypeList->setFlow(QListView::TopToBottom);
	preferencesTypeList->setIconSize(QSize(32,32));
	preferencesTypeList->setSortingEnabled(false);
	preferencesTypeList->setWrapping(false);
	preferencesTypeList->setWordWrap(true);
	preferencesTypeList->setAcceptDrops(false);
	preferencesTypeList->setDropIndicatorShown(false);
	preferencesTypeList->setDragDropMode(QAbstractItemView::NoDragDrop);
	//preferencesTypeList->setResizeMode(QListView::Adjust);
	preferencesTypeList->setSelectionMode(QAbstractItemView::SingleSelection);
	preferencesTypeList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	preferencesTypeList->clear();
}

int PreferencesDialog::addItem(QString name, QPixmap icon, QWidget* tab)
{
	//TODO: Can we avoid using this name and duplicating strings by getting it from the tab UIs
	QListWidgetItem* newItem = new QListWidgetItem(icon, name, preferencesTypeList);
	newItem->setTextAlignment(Qt::AlignHCenter);
	prefsStackWidget->addWidget(tab);
	stackWidgetMap.insert(newItem, counter);
	counter++;
	return counter-1;
}

void PreferencesDialog::newItemSelected()
{
	QList<QListWidgetItem*> items = preferencesTypeList->selectedItems();
	itemSelected((items.count() > 0) ? items.at(0) : NULL);
}

void PreferencesDialog::itemSelected(QListWidgetItem* ic)
{
	if (ic == 0)
		return;
	if (stackWidgetMap.contains(ic))
	{
		//emit aboutToShow(prefsWidgets->widget(itemMap[ic]));
		prefsStackWidget->setCurrentIndex(stackWidgetMap[ic]);
		if (prefsStackWidget->currentWidget()==dynamic_cast<QWidget*>(prefs_ItemTools))
			prefs_ItemTools->enableFontPreview(true);
		if (prefsStackWidget->currentWidget()==dynamic_cast<QWidget*>(prefs_TableOfContents))
			prefs_TableOfContents->setupItemAttrs( prefs_DocumentItemAttributes->getDocAttributesNames() );
		if (prefsStackWidget->currentWidget()==dynamic_cast<QWidget*>(prefs_PDFExport))
			prefs_PDFExport->enableCMS(prefs_ColorManagement->cmActive());
	}
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void PreferencesDialog::languageChange()
{
	setWindowTitle( tr( "Preferences" ) );
}

void PreferencesDialog::addPlugins()
{
	// Scan for plugins that provide a prefs widget, and add it to the
	// prefs dialog.
	// For each plugin, enabled or not:
	ScPlugin* plugin = 0;
	Prefs_Pane* panel = 0;
	QString panelCaption;
	QPixmap panelIcon;

	PluginManager& pluginManager = PluginManager::instance();
	QStringList pluginNames(pluginManager.pluginNames(true));

	foreach (QString pName, pluginManager.pluginNames(true))
	{
		// Ask the plugin manager for a plugin (skipping disabled plugins).
		plugin = pluginManager.getPlugin(pName, false);
		// If we got a plugin (which we know is enabled):
		if (plugin)
		{
			// Ask the plugin for a prefs widget
			bool wantPanel = plugin->newPrefsPanelWidget(prefsStackWidget, panel, panelCaption, panelIcon);
			// If it gave us one...
			if (wantPanel)
			{
				// Ensure that we got sane return values
				Q_ASSERT(panel);
				Q_ASSERT(!panelIcon.isNull());
				Q_ASSERT(!panelCaption.isNull());
				// plug it in to the dialog,
				addItem(panelCaption, panelIcon, panel);
				// and connect a signal to tell it to save its
				// settings.
				connect(this, SIGNAL(accepted()), panel, SLOT(apply()));
			}
		}
	}
}

void PreferencesDialog::getResizeDocumentPages(bool &resizePages, bool &resizeMasterPages, bool &resizePageMargins, bool &resizeMasterPageMargins)
{
	prefs_DocumentSetup->getResizeDocumentPages(resizePages, resizeMasterPages, resizePageMargins, resizeMasterPageMargins);
}


void PreferencesDialog::arrangeIcons()
{/*
	int maxWidth = 0;
	QListWidgetItem* ic;
	int startY = 5;
	for (int cc = 0; cc < preferencesTypeList->count(); ++cc)
	{
		ic = preferencesTypeList->item(cc);
		QRect ir = preferencesTypeList->visualItemRect(ic);
		maxWidth = qMax(ir.width(), maxWidth);
	}
	preferencesTypeList->setMaximumWidth(maxWidth+16);
	preferencesTypeList->setResizeMode(QListView::Fixed);
#ifdef _WIN32
	int scrollBarWidth = 0;
	QList<QScrollBar*> scrollBars = preferencesTypeList->findChildren<QScrollBar*>();
	for (int cc = 0; cc < scrollBars.count(); ++cc)
	{
		if (scrollBars.at(cc)->orientation() == Qt::Vertical)
		{
			scrollBarWidth = scrollBars.at(cc)->height();
			break;
		}
	}
#else
	int scrollBarWidth = maxWidth;
#endif
	int startX = qMax((preferencesTypeList->viewport()->width() - scrollBarWidth) / 2, 0);
	for (int cc = 0; cc < preferencesTypeList->count(); ++cc)
	{
		ic = preferencesTypeList->item(cc);
		QRect ir = preferencesTypeList->visualItemRect(ic);

#ifdef _WIN32
		preferencesTypeList->setPositionForIndex(QPoint(qMax(startX - ir.width() / 2, 0), startY), preferencesTypeList->indexFromItem(ic));
#else
		int moveW = (maxWidth - ir.width()) / 2;
		preferencesTypeList->setPositionForIndex(QPoint(moveW + startX, startY), preferencesTypeList->indexFromItem(ic));
#endif

		startY += ir.height()+5;
	}*/
}



