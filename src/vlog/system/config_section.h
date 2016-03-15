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

#include <QMap>
#include <QString>
#include <QVariant>
#include <QColor>

namespace General
{
	class CConfigSection
	{
	public:
		CConfigSection();
		~CConfigSection();

		int getParamInt(const QString& name) const;
		qreal getParamDouble(const QString& name) const;
		QString getParamString(const QString& name) const;
		QColor getParamColor(const QString& name) const;

		void setParam(const QString& name, int value);
		void setParam(const QString& name, qreal value);
		void setParam(const QString& name, const QString& value);
		void setParam(const QString& name, const QColor& value);

		bool isParam(const QString& name) const;
		QMap<QString, QVariant> getParams() const;

	private:
		QMap<QString, QVariant> m_params;

	};
}
