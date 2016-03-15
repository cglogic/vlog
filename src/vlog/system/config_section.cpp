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

#include "config_section.h"

namespace General
{
	CConfigSection::CConfigSection()
	{
	}

	CConfigSection::~CConfigSection()
	{
	}

	int CConfigSection::getParamInt(const QString& name) const
	{
		if (isParam(name))
		{
			return m_params.find(name).value().toInt();
		}

		return 0;
	}

	qreal CConfigSection::getParamDouble(const QString& name) const
	{
		if (isParam(name))
		{
			return m_params.find(name).value().toReal();
		}

		return 0.0;
	}

	QString CConfigSection::getParamString(const QString& name) const
	{
		if (isParam(name))
		{
			return m_params.find(name).value().toString();
		}

		return "";
	}

	QColor CConfigSection::getParamColor(const QString& name) const
	{
		if (isParam(name))
		{
			return m_params.find(name).value().value<QColor>();
		}

		return QColor(255, 255, 255);
	}

	void CConfigSection::setParam(const QString& name, int value)
	{
		m_params[name] = QVariant(value);
	}

	void CConfigSection::setParam(const QString& name, qreal value)
	{
		m_params[name] = QVariant(value);
	}

	void CConfigSection::setParam(const QString& name, const QString& value)
	{
		m_params[name] = QVariant(value);
	}

	void CConfigSection::setParam(const QString& name, const QColor& value)
	{
		m_params[name] = QVariant(value);
	}

	bool CConfigSection::isParam(const QString& name) const
	{
		return m_params.contains(name);
	}

	QMap<QString, QVariant> CConfigSection::getParams() const
	{
		return m_params;
	}
}
