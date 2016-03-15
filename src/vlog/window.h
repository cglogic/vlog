#pragma once

#include "config.h"

#include "system/dialog.h"
#include "system/log.h"
#include "system/config.h"
#include "interface/import_plugin.h"
#include "interface/export_plugin.h"

#include "preferences.h"
#include "view.h"
#include "scene/scene.h"
#include "manager.h"
#include "project.h"
#include "project_new.h"

//#include "preview/preview.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMap>
#include <QStyleFactory>
#include <QDockWidget>
#include <QSettings>
#include <QUndoStack>
#include <QUndoCommand>

namespace General
{
	// Основное окно программы
	class CWindow
		: public QMainWindow
	{
		Q_OBJECT

	public:
		CWindow();
		virtual ~CWindow();

		typedef QMap<QString, Interface::CImportPlugin*> t_import_file_ext;
		typedef QMap<QString, Interface::CImportPlugin*>::iterator t_import_file_ext_iter;

		typedef QMap<QAction*, Interface::CImportPlugin*> t_import_actions;
		typedef QMap<QAction*, Interface::CImportPlugin*>::iterator t_import_actions_iter;

		typedef QMap<QString, Interface::CExportPlugin*> t_export_file_ext;
		typedef QMap<QString, Interface::CExportPlugin*>::iterator t_export_file_ext_iter;

		typedef QMap<QAction*, Interface::CExportPlugin*> t_export_actions;
		typedef QMap<QAction*, Interface::CExportPlugin*>::iterator t_export_actions_iter;

	protected:
		virtual void closeEvent(QCloseEvent* event);

		virtual void keyPressEvent(QKeyEvent* event);
		virtual void keyReleaseEvent(QKeyEvent* event);

	private slots:
		void slotInvokeImportAction();
		void slotInvokeExportAction();

		void slotProjectNew();
		void slotProjectOpen();
		void slotProjectSave();
		void slotProjectClose();

		void slotImportFile();
		void slotExportFile();

		void slotPreferences();
		void slotExit();

		void slotModePixmap();
		void slotModeDepth();
		void slotModeParameter();
		void slotModeCurve();
		void slotModePoint();

		void slotAbout();

		void slotSetStatusText(const QString& tx);

	private:
		CView *m_view;
		CScene *m_scene;
		QDockWidget *m_workplace_dock;
		void setupView();

		/*CPreview *m_preview;
		QDockWidget *m_preview_dock;
		void setupPreview();*/

		void createScene();
		void destroyScene();

		QDockWidget *m_object_dock;
		QDockWidget *m_property_dock;
		CObjectManager *m_manager;
		void setupObjectManager();

		void setupUi();

		QAction *m_project_new_action;
		QAction *m_project_open_action;
		QAction *m_project_save_action;
		QAction *m_project_close_action;
		QAction *m_import_file_action;
		QAction *m_export_file_action;
		QAction *m_preferences_action;
		QAction *m_exit_action;
		QAction *m_about_action;

		QAction *m_mode_pixmap_action;
		QAction *m_mode_depth_action;
		QAction *m_mode_parameter_action;
		QAction *m_mode_curve_action;
		QAction *m_mode_point_action;

		QAction *m_undo_action;
		QAction *m_redo_action;

		void setupActions();

		QMenu *m_import_menu;
		QMenu *m_export_menu;
		QMenu *m_file_menu;
		QMenu *m_edit_menu;
		QMenu *m_mode_menu;
		QMenu *m_help_menu;

		void setupMainMenu();

		QToolBar *m_project_toolbar;
		QToolBar *m_mode_toolbar;

		void setupToolBars();

		QStatusBar *m_status_bar;
		QLabel *m_label;

		void setupStatusBar();

		t_import_file_ext m_import_file_ext;
		t_export_file_ext m_export_file_ext;

		t_import_actions m_import_actions;
		void invokedImportAction(Interface::CImportPlugin* plugin);

		t_export_actions m_export_actions;
		void invokedExportAction(Interface::CExportPlugin* plugin);

		void loadPlugins();
		void setupPlugin(QObject* plugin);
		void destroyPlugins();

		void readSettings();
		void writeSettings();

		CProject *m_active_project;

		QUndoStack *m_undo_stack;
	};
}
