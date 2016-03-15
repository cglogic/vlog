#include "plugin.h"

namespace General
{
	CExportLas3Plugin::CExportLas3Plugin()
	{
		m_data = new CExportLas3Data();
	}

	CExportLas3Plugin::~CExportLas3Plugin()
	{
		delete m_data;
	}

	QString CExportLas3Plugin::getInfoHeader() const
	{
		return "ExportLas3Plugin(v.0.1.0)";
	}

	QString CExportLas3Plugin::getUid() const
	{
		return "EXPLAS3";
	}

	QAction* CExportLas3Plugin::createMenuAction()
	{
		QAction *action = new QAction(this);
		action->setText(tr("Export to LAS 3.0..."));

		return action;
	}

	QStringList CExportLas3Plugin::getFileExtensions() const
	{
		QStringList file_ext;

		return file_ext;
	}

	QString CExportLas3Plugin::propertyTemplate() const
	{
		QFile file(":las3props.xml");

		if (file.open(QIODevice::ReadOnly))
		{
			return QString(file.readAll());
		}

		return QString();
	}

	Interface::CExportData* CExportLas3Plugin::getCleanDataHandle() const
	{
		m_data->clean();

		return m_data;
	}

	void CExportLas3Plugin::process(const QString& filename)
	{
		Q_UNUSED(filename);

		QString fname = QFileDialog::getSaveFileName(0, tr("Export to file"), QDir::currentPath(), "LAS 3.0 (*.las)");

		if (!fname.isEmpty())
		{
			m_data->saveToFile(fname);
		}
	}
}

Q_EXPORT_PLUGIN2(export_las3, General::CExportLas3Plugin)
