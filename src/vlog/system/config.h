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

#include "config_section.h"

#include <QMap>
#include <QString>
#include <QVariant>
#include <QFile>
#include <QtXml>

namespace General
{
	class CConfig
	{
	public:
		CConfig(const QString& filename);
		~CConfig();

		static CConfig* getInstance();
		void freeInstance();

		bool load(QFile* exists_file = 0);
		void save();

		CConfigSection* getSection(const QString& name);
		bool isSection(const QString& name) const;

	private:
		QString m_filename;

		void parseSections(const QDomElement& sections);
		void parseParams(CConfigSection* section, const QDomElement& params);

		CConfigSection m_dummy_section;
		QMap<QString, CConfigSection*> m_sections;

		void addSection(const QString& name, CConfigSection* section);
		void destroySections();
	};
}
