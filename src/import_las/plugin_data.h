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

#include "interface/import_data.h"

#include "loadform.h"

#include <QString>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QMap>
#include <QPaintEngine>
#include <QPaintEngineState>
#include <QPainterPath>
#include <limits>

namespace General
{
	class CImportLasData
		: public Interface::CImportData
	{
	public:
		enum ESectionType
		{
			ST_NONE,
			ST_VERSION,
			ST_WELL,
			ST_PARAMETER,
			ST_CURVE,
			ST_ASCII
		};

		CImportLasData();
		virtual ~CImportLasData();

		// get pixmap (your Captain Obvious)
		virtual const QPixmap& getPixmap();

		// import not canceled
		virtual bool accepted() const;

		// load (aka open&read)
		void load();

	private:
		QPixmap m_pixmap;

		bool m_accepted;

		typedef QVector<double> t_value;
		typedef QVector<t_value> t_values;

	};
}
