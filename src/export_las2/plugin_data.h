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

#include "interface/export_data.h"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QTextStream>

namespace General
{
	class CExportLas2Data
		: public Interface::CExportData
	{
	public:
		struct SLine
		{
			QString mnem;
			QString unit;
			QString value;
			QString desc;

			SLine(const QString& m = "", const QString& u = "", const QString& v = "", const QString& d = "");
			QString toString() const;
		};

		CExportLas2Data();
		virtual ~CExportLas2Data();

		// for derived classes
		virtual QString getVersion();
		virtual QStringList getHeader();

		// version section
		virtual void setWrap(const QString& wrap);
		virtual void setDlm(const QString& dlm);

		// well section
		virtual void setStart(const QString& start, const QString unit = "M");
		virtual void setStop(const QString& stop, const QString unit = "M");
		virtual void setStep(const QString& step, const QString unit = "M");
		virtual void setNull(const QString& null);
		virtual void setCompany(const QString& company);
		virtual void setWell(const QString& well);
		virtual void setField(const QString& field);
		virtual void setLocation(const QString& location);
		virtual void setCountry(const QString& country);
		virtual void setServiceCompany(const QString& company);
		virtual void setDate(const QString& date, const QString format = "DD/MM/YYYY");
		virtual void setLatitude(const QString& latitude, const QString& format = "F");
		virtual void setLongitude(const QString& longitude, const QString& format = "F");
		virtual void setX(const QString& x);
		virtual void setY(const QString& y);
		virtual void setGeodeticDatum(const QString& datum);
		virtual void setHorizontalCoords(const QString& hzcs);
		virtual void setProvince(const QString& province);
		virtual void setWellID(const QString& id);
		virtual void setLicence(const QString& licence);
		virtual void setState(const QString& state);
		virtual void setCounty(const QString& county);
		virtual void setAPI(const QString& api);
		virtual void addWellParameter(const QString& mnemonic, const QString& unit, const QString& value, const QString& description, const QString& format = "");

		// log parameter section
		virtual void addLogParameter(const QString& mnemonic, const QString& unit, const QString& value, const QString& description, const QString& format = "", const QString& association = "");

		// log definition section
		virtual void addLogDefinition(const QString& mnemonic, const QString& unit, const QString& code, const QString& description, const QString& format = "", const QString& association = "");

		// log data section
		virtual void addLodData(const QStringList& data);

		void clean();
		void saveToFile(const QString& filename);

	public:
		QVector<SLine> m_version;
		void initVersion();
		void changeVersionLine(const QString& mnem, const QString& value);

		QVector<SLine> m_well;
		void initWell();
		void changeWellLine(const QString& mnem, const QString& value, const QString& unit = "", const QString& format = "");

		QVector<SLine> m_log_parameters;
		QVector<SLine> m_log_definitions;
		QVector<QStringList> m_log_data;

	};
}
