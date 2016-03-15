#include "plugin.h"

namespace General
{
	CImportLasPlugin::CImportLasPlugin()
	{
	}

	CImportLasPlugin::~CImportLasPlugin()
	{
	}

	QString CImportLasPlugin::getInfoHeader() const
	{
		return "ImportLasPlugin(v.0.1.0)";
	}

	QAction* CImportLasPlugin::createMenuAction()
	{
		QAction *action = new QAction(this);

		action->setText(tr("Import from LAS..."));

		return action;
	}

	QStringList CImportLasPlugin::getFileExtensions() const
	{
		QStringList file_ext;

		return file_ext;
	}

	Interface::CImportData* CImportLasPlugin::process(const QString& filename)
	{
		Q_UNUSED(filename);

		CImportLasData *data = new CImportLasData();

		data->load();

		return data;
	}
}

Q_EXPORT_PLUGIN2(import_las, General::CImportLasPlugin)
