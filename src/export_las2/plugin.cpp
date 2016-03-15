#include "plugin.h"

namespace General
{
	CExportLas2Plugin::CExportLas2Plugin()
	{
		m_data = new CExportLas2Data();
	}

	CExportLas2Plugin::~CExportLas2Plugin()
	{
		delete m_data;
	}

	QString CExportLas2Plugin::getInfoHeader() const
	{
		return "ExportLas2Plugin(v.0.1.0)";
	}

	QString CExportLas2Plugin::getUid() const
	{
		return "EXPLAS2";
	}

	QAction* CExportLas2Plugin::createMenuAction()
	{
		QAction *action = new QAction(this);
		action->setText(tr("Export to LAS 2.0..."));

		return action;
	}

	QStringList CExportLas2Plugin::getFileExtensions() const
	{
		QStringList file_ext;

		return file_ext;
	}

	QString CExportLas2Plugin::propertyTemplate() const
	{
		QFile file(":las2props.xml");

		if (file.open(QIODevice::ReadOnly))
		{
			return QString(file.readAll());
		}

		return QString();
	}

	Interface::CExportData* CExportLas2Plugin::getCleanDataHandle() const
	{
		m_data->clean();

		return m_data;
	}

	void CExportLas2Plugin::process(const QString& filename)
	{
		Q_UNUSED(filename);

		QString fname = QFileDialog::getSaveFileName(0, tr("Export to file"), QDir::currentPath(), "LAS 2.0 (*.las)");

		if (!fname.isEmpty())
		{
			m_data->saveToFile(fname);
		}
	}
}

Q_EXPORT_PLUGIN2(export_las2, General::CExportLas2Plugin)
