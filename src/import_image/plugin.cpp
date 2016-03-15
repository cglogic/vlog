#include "plugin.h"

namespace General
{
	CImportImagePlugin::CImportImagePlugin()
	{
	}

	CImportImagePlugin::~CImportImagePlugin()
	{
	}

	QString CImportImagePlugin::getInfoHeader() const
	{
		return "ImportImagePlugin(v.0.1.0)";
	}

	QAction* CImportImagePlugin::createMenuAction()
	{
		return 0;
	}

	QStringList CImportImagePlugin::getFileExtensions() const
	{
		QStringList file_ext;

		QList<QByteArray> formats = QImageReader::supportedImageFormats();
		for (int i = 0; i < formats.size(); ++i)
		{
			QByteArray bytes = formats.at(i);
			file_ext.push_back(QString(bytes));
		}

		return file_ext;
	}

	Interface::CImportData* CImportImagePlugin::process(const QString& filename)
	{
		CImportImageData *data = new CImportImageData();
		data->loadFromFile(filename);

		return data;
	}
}

Q_EXPORT_PLUGIN2(import_image, General::CImportImagePlugin)
