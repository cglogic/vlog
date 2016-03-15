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
