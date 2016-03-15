/****************************************************************************
 * Copyright (C) 2012-2013 Ievgen Polyvanyi
 * Copyright (C) 2012-2016 Oleg Gushchenkov
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
****************************************************************************/

#include "window.h"

#include "about.h"

namespace General
{
	CWindow::CWindow()
		: m_view(0), m_scene(0), m_active_project(0)
	{
		m_undo_stack = new QUndoStack(this);

		setWindowTitle(tr("VLog 1.0")
#ifdef VL_DEMO_BUILD
						  .append(tr(" DEMO"))
#endif
						  );

		setupUi();
		loadPlugins();

		setWindowState(windowState() ^ Qt::WindowMaximized);

		readSettings();
	}

	CWindow::~CWindow()
	{
		writeSettings();
	}

	//

	void CWindow::closeEvent(QCloseEvent* event)
	{
		Q_UNUSED(event);

		slotProjectClose();
	}

	void CWindow::keyPressEvent(QKeyEvent* event)
	{
		if (m_scene)
		{
			m_scene->globalKeyPressEvent(event);
		}

		QMainWindow::keyPressEvent(event);
	}

	void CWindow::keyReleaseEvent(QKeyEvent* event)
	{
		if (m_scene)
		{
			m_scene->globalKeyReleaseEvent(event);
		}

		QMainWindow::keyReleaseEvent(event);
	}

	//

	void CWindow::slotInvokeImportAction()
	{
		QAction *action = qobject_cast<QAction*>(sender());

		t_import_actions_iter iter = m_import_actions.find(action);

		if (iter != m_import_actions.end())
		{
			invokedImportAction(iter.value());
		}
	}

	void CWindow::slotInvokeExportAction()
	{
		QAction *action = qobject_cast<QAction*>(sender());

		t_export_actions_iter iter = m_export_actions.find(action);

		if (iter != m_export_actions.end())
		{
			invokedExportAction(iter.value());
		}
	}

	void CWindow::slotProjectNew()
	{
		CProjectNewDialog dialog;

		if (dialog.exec() == QDialog::Accepted)
		{
			slotProjectClose();

			m_active_project = new CProject();

			m_active_project->create(dialog.projectName(), dialog.projectPath());

			m_manager->addProject(m_active_project);

			createScene();

			m_project_save_action->setEnabled(true);
			m_project_close_action->setEnabled(true);

			m_import_menu->setEnabled(true);
			m_export_menu->setEnabled(true);

			m_mode_pixmap_action->setEnabled(true);
			m_mode_depth_action->setEnabled(true);
			m_mode_parameter_action->setEnabled(true);
			m_mode_curve_action->setEnabled(true);
			m_mode_point_action->setEnabled(true);
		}
	}

	void CWindow::slotProjectOpen()
	{
		QString filename = CDialog::openFile(tr("VLog project files (*.vpr)"), tr("Open project"), QDir::homePath() + "/vlog");

		if (!filename.isEmpty())
		{
			slotProjectClose();

			m_active_project = new CProject();

			createScene();

			m_manager->addProject(m_active_project);

			m_view->setEnabled(true);

			m_scene->deserialize(m_active_project->open(filename));

			m_project_save_action->setEnabled(true);
			m_project_close_action->setEnabled(true);

			m_import_menu->setEnabled(true);
			m_export_menu->setEnabled(true);

			m_mode_pixmap_action->setEnabled(true);
			m_mode_depth_action->setEnabled(true);
			m_mode_parameter_action->setEnabled(true);
			m_mode_curve_action->setEnabled(true);
			m_mode_point_action->setEnabled(true);
		}
	}

	void CWindow::slotProjectSave()
	{
		if (m_active_project && m_scene)
		{
			m_scene->serialize(m_active_project->document(),  m_active_project->propertyValue("project_path").toString());

			m_active_project->save();
		}
	}

	void CWindow::slotProjectClose()
	{
		if (m_active_project)
		{
			if (m_active_project->close())
			{
				destroyScene();

				m_manager->removeProject(m_active_project);

				delete m_active_project;

				m_active_project = 0;

				m_project_save_action->setEnabled(false);
				m_project_close_action->setEnabled(false);

				m_import_menu->setEnabled(false);
				m_export_menu->setEnabled(false);

				m_mode_pixmap_action->setEnabled(false);
				m_mode_depth_action->setEnabled(false);
				m_mode_parameter_action->setEnabled(false);
				m_mode_curve_action->setEnabled(false);
				m_mode_point_action->setEnabled(false);
			}
		}
	}

	void CWindow::slotImportFile()
	{
		if (m_import_file_ext.size() != 0)
		{
			QStringList file_exts;
			file_exts.push_back(tr("All files (*.*)"));

			for (t_import_file_ext_iter iter = m_import_file_ext.begin(); iter != m_import_file_ext.end(); ++iter)
			{
				file_exts.push_back("*." + iter.key());
			}

			// note: to solid line
			QString pattern = file_exts.join(";;");

			QString filename = CDialog::openFile(pattern, tr("Import file"));

			if (!filename.isEmpty())
			{
				QString ext = QFileInfo(filename).suffix().toLower();
				t_import_file_ext_iter iter = m_import_file_ext.find(ext);
				if (iter != m_import_file_ext.end())
				{
					Interface::CImportPlugin *plugin = iter.value();
					Interface::CImportData *data = plugin->process(filename);

					m_view->setEnabled(true);

					m_scene->setPixmap(data->getPixmap());

					delete data;
				}
				else
				{
					CDialog::error(tr("This file type is not supported."), tr("Can't import file"));
				}
			}
		}
	}

	void CWindow::slotExportFile()
	{
		if (m_export_file_ext.size() != 0)
		{
			QStringList file_exts;
			for (t_export_file_ext_iter iter = m_export_file_ext.begin(); iter != m_export_file_ext.end(); ++iter)
			{
				file_exts.push_back("*." + iter.key());
			}

			QString pattern = file_exts.join(";;");

			QString filename = CDialog::saveFile(pattern, tr("Export file"));

			if (!filename.isEmpty())
			{
				QString ext = QFileInfo(filename).suffix();
				t_export_file_ext_iter iter = m_export_file_ext.find(ext);
				if (iter != m_export_file_ext.end())
				{
					Interface::CExportPlugin *plugin = iter.value();
					Interface::CExportData *data = plugin->getCleanDataHandle();

					m_scene->exportAction(data, m_active_project);

					plugin->process(filename);
				}
			}
		}
	}

	void CWindow::slotPreferences()
	{
		CPreferencesDialog preferences;

		preferences.setLanguage(CConfig::getInstance()->getSection("interface")->getParamString("language"));
		preferences.setAutosave(CConfig::getInstance()->getSection("projects")->getParamInt("autosave"));
		preferences.setAutosaveTime(CConfig::getInstance()->getSection("projects")->getParamInt("autosavetime"));

		if (preferences.exec() == QDialog::Accepted)
		{
			CConfig::getInstance()->getSection("interface")->setParam("language", preferences.getLanguage());
			CConfig::getInstance()->getSection("projects")->setParam("autosave", preferences.isAutosave());
			CConfig::getInstance()->getSection("projects")->setParam("autosavetime", preferences.getAutosaveTime());

			CConfig::getInstance()->save();
		}
	}

	void CWindow::slotExit()
	{
		close();
	}

	void CWindow::slotModePixmap()
	{
		if (m_mode_pixmap_action->isChecked())
		{
			m_scene->setMode(CScene::ModePixmap, true);
		}
		else
		{
			m_scene->setMode(CScene::ModePixmap, false);
		}
	}

	void CWindow::slotModeDepth()
	{
		if (m_mode_depth_action->isChecked())
		{
			m_scene->setMode(CScene::ModeDepth, true);
		}
		else
		{
			m_scene->setMode(CScene::ModeDepth, false);
		}
	}

	void CWindow::slotModeParameter()
	{
		if (m_mode_parameter_action->isChecked())
		{
			m_scene->setMode(CScene::ModeParameter, true);
		}
		else
		{
			m_scene->setMode(CScene::ModeParameter, false);
		}
	}

	void CWindow::slotModeCurve()
	{
		if (m_mode_curve_action->isChecked())
		{
			m_scene->setMode(CScene::ModeCurve, true);
		}
		else
		{
			m_scene->setMode(CScene::ModeCurve, false);
		}
	}

	void CWindow::slotModePoint()
	{
		if (m_mode_point_action->isChecked())
		{
			m_scene->setMode(CScene::ModePoint, true);
		}
		else
		{
			m_scene->setMode(CScene::ModePoint, false);
		}
	}

	void CWindow::slotAbout()
	{
		CAboutDialog *dialog = new CAboutDialog();

		if (dialog->exec() == QDialog::Accepted)
		{
		}

		if (dialog != NULL)
		{
			delete dialog;
		}
	}

	void CWindow::slotSetStatusText(const QString& tx)
	{
		m_label->setText(tx);
	}

	//

	void CWindow::setupUi()
	{
		setupView();
		//setupPreview();
		setupObjectManager();

		setupActions();
		setupMainMenu();
		setupToolBars();
		setupStatusBar();
	}

	void CWindow::setupView()
	{
		m_view = new CView(this);
		m_view->setEnabled(false);

		/*m_workplace_dock = new QDockWidget(this);
		m_workplace_dock->setObjectName("workplace_dock");
		m_workplace_dock->setWindowTitle(tr("Workplace"));
		m_workplace_dock->setWidget(m_view);

		addDockWidget(Qt::RightDockWidgetArea, m_workplace_dock);*/

		setCentralWidget(m_view);

		m_view->show();
		m_view->setFocusPolicy(Qt::WheelFocus);
	}

	/*void CWindow::setupPreview()
	{
		m_preview = new CPreview(this);

		m_preview_dock = new QDockWidget(this);
		m_preview_dock->setObjectName("preview_dock");
		m_preview_dock->setWindowTitle(tr("Preview"));
		m_preview_dock->setWidget(m_preview);

		addDockWidget(Qt::RightDockWidgetArea, m_preview_dock);
	}*/

	void CWindow::createScene()
	{
		if (m_scene)
		{
			destroyScene();
		}

		m_scene = new CScene(this);

		m_view->setScene(m_scene);

		connect(m_view, SIGNAL(signalZoomChange(int)), m_scene, SLOT(slotZoomChange(int)));

		connect(m_scene, SIGNAL(signalObjectCreated(CObject*)), m_manager, SLOT(slotObjectRegister(CObject*)));
		connect(m_scene, SIGNAL(signalObjectDeleted(CObject*)), m_manager, SLOT(slotObjectUnregister(CObject*)));
		connect(m_scene, SIGNAL(signalObjectActivated(CObject*)), m_manager, SLOT(slotExternalObjectActivation(CObject*)));
		connect(m_scene, SIGNAL(signalObjectCaptionChanged(CObject*)), m_manager, SLOT(slotObjectCaptionChanged(CObject*)));

		connect(m_manager, SIGNAL(signalObjectActivated(CObject*)), m_scene, SLOT(slotExternalObjectActivation(CObject*)));
		connect(m_manager, SIGNAL(signalObjectDeletion(CObject*)), m_scene, SLOT(slotExternalObjectDeletion(CObject*)));

		//connect(m_scene, SIGNAL(signalObjectActivated(CObject*)), m_preview, SLOT(slotObjectSelected(CObject*)));
		//connect(m_scene, SIGNAL(signalObjectDeactivated()), m_preview, SLOT(slotObjectUnselected(CObject*)));

		connect(m_scene, SIGNAL(signalSetStatusText(const QString&)), this, SLOT(slotSetStatusText(const QString&)));

		m_view->resetTransform();

		m_view->setEnabled(false);
	}

	void CWindow::destroyScene()
	{
		m_view->setScene(0);

		delete m_scene;

		m_scene = 0;

		m_view->setEnabled(false);
	}

	void CWindow::setupObjectManager()
	{
		m_manager = new CObjectManager(this);

		m_object_dock = new QDockWidget(this);
		m_object_dock->setObjectName("object_tree_dock");
		m_object_dock->setWindowTitle(tr("Object tree"));
		m_object_dock->setWidget(m_manager->getTreeWidget());

		addDockWidget(Qt::LeftDockWidgetArea, m_object_dock);

		m_property_dock = new QDockWidget(this);
		m_property_dock->setObjectName("object_props_dock");
		m_property_dock->setWindowTitle(tr("Object properties"));
		m_property_dock->setWidget(m_manager->getPropertyWidget());

		addDockWidget(Qt::LeftDockWidgetArea, m_property_dock);
	}

	void CWindow::setupStatusBar()
	{
		m_status_bar = new QStatusBar(this);
		m_status_bar->setSizeGripEnabled(true);

		setStatusBar(m_status_bar);

		m_label = new QLabel(m_status_bar);

		m_status_bar->addWidget(m_label);
	}

	void CWindow::setupActions()
	{
		// project
		m_project_new_action = new QAction(this);
		m_project_new_action->setText(tr("New project"));
		m_project_new_action->setIcon(QIcon(":/icons/blank.png"));
		m_project_new_action->setStatusTip(tr("Create new project"));
		connect(m_project_new_action, SIGNAL(triggered()), this, SLOT(slotProjectNew()));

		m_project_open_action = new QAction(this);
		m_project_open_action->setText(tr("Open project"));
		m_project_open_action->setIcon(QIcon(":/icons/folder.png"));
		m_project_open_action->setStatusTip(tr("Open existing project"));
		connect(m_project_open_action, SIGNAL(triggered()), this, SLOT(slotProjectOpen()));

		m_project_save_action = new QAction(this);
		m_project_save_action->setText(tr("Save project"));
		m_project_save_action->setIcon(QIcon(":/icons/diskette.png"));
		m_project_save_action->setStatusTip(tr("Save current project"));
		m_project_save_action->setEnabled(false);
		connect(m_project_save_action, SIGNAL(triggered()), this, SLOT(slotProjectSave()));

		m_project_close_action = new QAction(this);
		m_project_close_action->setText(tr("Close project"));
		m_project_close_action->setIcon(QIcon(":/icons/box_closed.png"));
		m_project_close_action->setStatusTip(tr("Close current project"));
		m_project_close_action->setEnabled(false);
		connect(m_project_close_action, SIGNAL(triggered()), this, SLOT(slotProjectClose()));

		// import and export
		m_import_file_action = new QAction(this);
		m_import_file_action->setText(tr("Import from file..."));
		m_import_file_action->setIcon(QIcon(":/icons/import_file.png"));
		connect(m_import_file_action, SIGNAL(triggered()), this, SLOT(slotImportFile()));

		m_export_file_action = new QAction(this);
		m_export_file_action->setText(tr("Export to file..."));
		m_export_file_action->setIcon(QIcon(":/icons/export_file.png"));
		connect(m_export_file_action, SIGNAL(triggered()), this, SLOT(slotExportFile()));

		m_preferences_action = new QAction(this);
		m_preferences_action->setText(tr("Preferences"));
		m_preferences_action->setIcon(QIcon(":/icons/wrench.png"));
		connect(m_preferences_action, SIGNAL(triggered()), this, SLOT(slotPreferences()));

		m_exit_action = new QAction(this);
		m_exit_action->setText(tr("Exit"));
		m_exit_action->setIcon(QIcon(":/icons/door_out.png"));
		connect(m_exit_action, SIGNAL(triggered()), this, SLOT(slotExit()));

		// edit
		m_undo_action = m_undo_stack->createUndoAction(this, tr("Undo"));
		m_undo_action->setShortcuts(QKeySequence::Undo);

		m_redo_action = m_undo_stack->createRedoAction(this, tr("Redo"));
		m_redo_action->setShortcuts(QKeySequence::Redo);

		// mode
		m_mode_pixmap_action = new QAction(this);
		m_mode_pixmap_action->setText(tr("Pixmap filter mode"));
		m_mode_pixmap_action->setIcon(QIcon(":/icons/layer.png"));
		m_mode_pixmap_action->setCheckable(true);
		m_mode_pixmap_action->setChecked(true);
		m_mode_pixmap_action->setEnabled(false);
		m_mode_pixmap_action->setStatusTip(tr("Filter creation pixmap"));
		connect(m_mode_pixmap_action, SIGNAL(triggered()), this, SLOT(slotModePixmap()));

		m_mode_depth_action = new QAction(this);
		m_mode_depth_action->setText(tr("Depth filter mode"));
		m_mode_depth_action->setIcon(QIcon(":/icons/draw_line.png"));
		m_mode_depth_action->setCheckable(true);
		m_mode_depth_action->setChecked(true);
		m_mode_depth_action->setEnabled(false);
		m_mode_depth_action->setStatusTip(tr("Filter creation depth"));
		connect(m_mode_depth_action, SIGNAL(triggered()), this, SLOT(slotModeDepth()));

		m_mode_parameter_action = new QAction(this);
		m_mode_parameter_action->setText(tr("Parameter filter mode"));
		m_mode_parameter_action->setIcon(QIcon(":/icons/canvas.png"));
		m_mode_parameter_action->setCheckable(true);
		m_mode_parameter_action->setChecked(true);
		m_mode_parameter_action->setEnabled(false);
		m_mode_parameter_action->setStatusTip(tr("Filter creation parameter"));
		connect(m_mode_parameter_action, SIGNAL(triggered()), this, SLOT(slotModeParameter()));

		m_mode_curve_action = new QAction(this);
		m_mode_curve_action->setText(tr("Curve filter mode"));
		m_mode_curve_action->setIcon(QIcon(":/icons/draw_wave.png"));
		m_mode_curve_action->setCheckable(true);
		m_mode_curve_action->setChecked(true);
		m_mode_curve_action->setEnabled(false);
		m_mode_curve_action->setStatusTip(tr("Filter creation curve"));
		connect(m_mode_curve_action, SIGNAL(triggered()), this, SLOT(slotModeCurve()));

		m_mode_point_action = new QAction(this);
		m_mode_point_action->setText(tr("Point filter mode"));
		m_mode_point_action->setIcon(QIcon(":/icons/draw_vertex.png"));
		m_mode_point_action->setCheckable(true);
		m_mode_point_action->setChecked(true);
		m_mode_point_action->setEnabled(false);
		m_mode_point_action->setStatusTip(tr("Filter creation control point"));
		connect(m_mode_point_action, SIGNAL(triggered()), this, SLOT(slotModePoint()));

		// about
		m_about_action = new QAction(this);
		m_about_action->setText(tr("About"));
		//m_about_action->setIcon(QIcon(":/icons/layer.png"));
		connect(m_about_action, SIGNAL(triggered()), this, SLOT(slotAbout()));
	}

	void CWindow::setupMainMenu()
	{
		m_import_menu = new QMenu(tr("Import"), this);
		m_import_menu->addAction(m_import_file_action);
		m_import_menu->setEnabled(false);

		m_export_menu = new QMenu(tr("Export"), this);
		m_export_menu->addAction(m_export_file_action);
		m_export_menu->setEnabled(false);

		m_file_menu = new QMenu(tr("File"), this);
		m_file_menu->addAction(m_project_new_action);
		m_file_menu->addAction(m_project_open_action);
		m_file_menu->addAction(m_project_save_action);
		m_file_menu->addAction(m_project_close_action);
		m_file_menu->addSeparator();
		m_file_menu->addMenu(m_import_menu);
		m_file_menu->addMenu(m_export_menu);
		m_file_menu->addSeparator();
		m_file_menu->addAction(m_preferences_action);
		m_file_menu->addSeparator();
		m_file_menu->addAction(m_exit_action);
		menuBar()->addMenu(m_file_menu);

		m_edit_menu = new QMenu(tr("Edit"), this);
		m_edit_menu->addAction(m_undo_action);
		m_edit_menu->addAction(m_redo_action);
		menuBar()->addMenu(m_edit_menu);

		m_mode_menu = new QMenu(tr("Mode"), this);
		m_mode_menu->addAction(m_mode_pixmap_action);
		m_mode_menu->addAction(m_mode_depth_action);
		m_mode_menu->addAction(m_mode_parameter_action);
		m_mode_menu->addAction(m_mode_curve_action);
		m_mode_menu->addAction(m_mode_point_action);
		//m_mode_menu->addSeparator();
		//m_mode_menu->addAction(m_param_list_action);
		menuBar()->addMenu(m_mode_menu);

		m_help_menu = new QMenu(tr("Help"), this);
		m_help_menu->addAction(m_about_action);
		menuBar()->addMenu(m_help_menu);
	}

	void CWindow::setupToolBars()
	{
		m_project_toolbar = new QToolBar(tr("Project"), this);
		m_project_toolbar->setObjectName("project_toolbar");
		m_project_toolbar->addAction(m_project_new_action);
		m_project_toolbar->addAction(m_project_open_action);
		m_project_toolbar->addAction(m_project_save_action);
		m_project_toolbar->addAction(m_project_close_action);
		addToolBar(m_project_toolbar);

		m_mode_toolbar = new QToolBar(tr("Mode"), this);
		m_mode_toolbar->setObjectName("mode_toolbar");
		m_mode_toolbar->addAction(m_mode_pixmap_action);
		m_mode_toolbar->addAction(m_mode_depth_action);
		m_mode_toolbar->addAction(m_mode_parameter_action);
		m_mode_toolbar->addAction(m_mode_curve_action);
		m_mode_toolbar->addAction(m_mode_point_action);
		addToolBar(m_mode_toolbar);
	}

	void CWindow::invokedImportAction(Interface::CImportPlugin* plugin)
	{
		Interface::CImportData *data = plugin->process();

		m_view->setEnabled(true);

		m_scene->setPixmap(data->getPixmap());

		delete data;
	}

	void CWindow::invokedExportAction(Interface::CExportPlugin* plugin)
	{
		Interface::CExportData *data = plugin->getCleanDataHandle();

		if (m_scene->exportAction(data, m_active_project) == true)
		{
			plugin->process();
		}
	}

	void CWindow::loadPlugins()
	{
		QDir plugins_dir = QDir(qApp->applicationDirPath());

#ifdef QT_DEBUG
		plugins_dir.cdUp();
		plugins_dir.cdUp();
#endif

		plugins_dir.cd("plugins");

		QStringList plugin_files = plugins_dir.entryList(QDir::Files);
		for (QStringList::iterator iter = plugin_files.begin(); iter != plugin_files.end(); ++iter)
		{
			QPluginLoader loader(plugins_dir.absoluteFilePath(*iter));
			QObject *plugin = loader.instance();
			if (plugin != 0)
			{
				setupPlugin(plugin);
			}
		}

		if (m_import_file_ext.size() == 0)
		{
			m_import_file_action->setEnabled(false);
		}

		if (m_export_file_ext.size() == 0)
		{
			m_export_file_action->setEnabled(false);
		}
	}

	void CWindow::setupPlugin(QObject *plugin)
	{
		Interface::CImportPlugin *import_plugin = qobject_cast<Interface::CImportPlugin*>(plugin);

		if (import_plugin != 0)
		{
			// register file extensions
			QStringList file_ext = import_plugin->getFileExtensions();

			for (QStringList::iterator iter = file_ext.begin(); iter != file_ext.end(); ++iter)
			{
				t_import_file_ext_iter ext_iter = m_import_file_ext.find(*iter);

				if (ext_iter == m_import_file_ext.end())
				{
					m_import_file_ext.insert((*iter).toLower(), import_plugin);
				}
			}

			// register menu action

			QAction *action = import_plugin->createMenuAction();

			if (action != 0)
			{
				m_import_menu->addAction(action);

				connect(action, SIGNAL(triggered()), this, SLOT(slotInvokeImportAction()));

				m_import_actions.insert(action, import_plugin);
			}

			CLog::getInstance()->information(tr("Installed import plugin: ") + import_plugin->getInfoHeader());
		}

		Interface::CExportPlugin *export_plugin = qobject_cast<Interface::CExportPlugin*>(plugin);

		if (export_plugin != 0)
		{
			// register file extensions

			QStringList file_ext = export_plugin->getFileExtensions();

			for (QStringList::iterator iter = file_ext.begin(); iter != file_ext.end(); ++iter)
			{
				t_export_file_ext_iter ext_iter = m_export_file_ext.find(*iter);

				if (ext_iter == m_export_file_ext.end())
				{
					m_export_file_ext.insert((*iter).toLower(), export_plugin);
				}
			}

			// register menu action

			QAction *action = export_plugin->createMenuAction();

			if (action != 0)
			{
				m_export_menu->addAction(action);

				connect(action, SIGNAL(triggered()), this, SLOT(slotInvokeExportAction()));

				m_export_actions.insert(action, export_plugin);
			}

			// load property template

			QString props = export_plugin->propertyTemplate();

			if (!props.isEmpty())
			{
				m_manager->installSharedProps(export_plugin->getUid(), props);
			}

			CLog::getInstance()->information(tr("Installed export plugin: ") + export_plugin->getInfoHeader());
		}
	}

	void CWindow::destroyPlugins()
	{
		for (t_import_file_ext_iter iter = m_import_file_ext.begin(); iter != m_import_file_ext.end(); ++iter)
		{
			delete (iter.value());
		}

		for (t_import_actions_iter iter = m_import_actions.begin(); iter != m_import_actions.end(); ++iter)
		{
			delete (iter.value());
		}

		for (t_export_file_ext_iter iter = m_export_file_ext.begin(); iter != m_export_file_ext.end(); ++iter)
		{
			delete (iter.value());
		}

		for (t_export_actions_iter iter = m_export_actions.begin(); iter != m_export_actions.end(); ++iter)
		{
			delete (iter.value());
		}
	}

	void CWindow::readSettings()
	{
		QSettings settings(this);
		settings.beginGroup("/settings");

		restoreGeometry(settings.value("geometry").toByteArray());
		restoreState(settings.value("state").toByteArray());

		settings.endGroup();
	}

	void CWindow::writeSettings()
	{
		QSettings settings(this);
		settings.beginGroup("/settings");

		settings.setValue("geometry", saveGeometry());
		settings.setValue("state", saveState());

		settings.endGroup();
	}
}
