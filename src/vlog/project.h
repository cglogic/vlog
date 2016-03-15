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

#include "config.h"

#include "system/dialog.h"
#include "object.h"

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QtXml/QtXml>

namespace General
{
	class CProject
		: public CObject
	{
		Q_OBJECT

	public:
		CProject();
		virtual ~CProject();

		QDomDocument& document();

		bool create(const QString& name, const QString& path);
		QDomElement open(const QString& filename);
		bool save();
		bool close();

		virtual QVariant propertyValue(const QString& name) const;
		virtual void setPropertyValue(const QString& name, const QVariant& value);

		virtual bool isRoot() const;

	private:
		QString m_project_name;
		QString m_project_path;

		QDomDocument m_document;


	};
}
