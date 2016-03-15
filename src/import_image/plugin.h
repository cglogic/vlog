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

#pragma once

#include "interface/import_plugin.h"
#include "plugin_data.h"

#include <QObject>
#include <QAction>
#include <QStringList>
#include <QtPlugin>
#include <QPixmap>
#include <QImageReader>

namespace General
{
	class CImportImagePlugin
		: public QObject, public Interface::CImportPlugin
	{
		Q_OBJECT
		Q_INTERFACES(Interface::CImportPlugin)

	public:
		CImportImagePlugin();
		virtual ~CImportImagePlugin();

		// get plugin info
		virtual QString getInfoHeader() const;

		// register in application
		virtual QAction* createMenuAction();
		virtual QStringList getFileExtensions() const;

		// get import data
		virtual Interface::CImportData* process(const QString& filename = "");

	};
}
