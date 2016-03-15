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
