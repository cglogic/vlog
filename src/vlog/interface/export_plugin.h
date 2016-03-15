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

#include "export_data.h"

#include <QObject>
#include <QAction>
#include <QStringList>
#include <QFile>

namespace Interface
{
	class CExportPlugin
	{
	public:
		CExportPlugin();
		virtual ~CExportPlugin();

		// get plugin info
		virtual QString getUid() const = 0;
		virtual QString getInfoHeader() const = 0;

		// register in application
		virtual QAction* createMenuAction() = 0;
		virtual QStringList getFileExtensions() const = 0;

		// property template
		virtual QString propertyTemplate() const;

		// get handled export data
		virtual CExportData* getCleanDataHandle() const = 0;

		// exporting
		virtual void process(const QString& filename = "") = 0;

	private:
		// disable copying
		CExportPlugin(const CExportPlugin& plugin);
		CExportPlugin& operator=(const CExportPlugin& rhs);

	};
}

Q_DECLARE_INTERFACE(Interface::CExportPlugin, "com.company.vanamingo.CExportPlugin/1.0")
