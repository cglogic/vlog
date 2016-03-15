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
