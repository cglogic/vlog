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

#include <QString>
#include <QStringList>

namespace Interface
{
	class CExportData
	{
	public:
		CExportData();
		virtual ~CExportData();

		// for derived classes
		virtual QString getVersion() = 0;
		virtual QStringList getHeader() = 0;

		// version section
		virtual void setWrap(const QString& wrap) = 0;
		virtual void setDlm(const QString& dlm) = 0;

		// well section
		virtual void setStart(const QString& start, const QString unit = "M") = 0;
		virtual void setStop(const QString& stop, const QString unit = "M") = 0;
		virtual void setStep(const QString& step, const QString unit = "M") = 0;
		virtual void setNull(const QString& null) = 0;
		virtual void setCompany(const QString& company) = 0;
		virtual void setWell(const QString& well) = 0;
		virtual void setField(const QString& field) = 0;
		virtual void setLocation(const QString& location) = 0;
		virtual void setCountry(const QString& country) = 0;
		virtual void setServiceCompany(const QString& company) = 0;
		virtual void setDate(const QString& date, const QString format = "DD/MM/YYYY") = 0;
		virtual void setLatitude(const QString& latitude, const QString& format = "F") = 0;
		virtual void setLongitude(const QString& longitude, const QString& format = "F") = 0;
		virtual void setX(const QString& x) = 0;
		virtual void setY(const QString& y) = 0;
		virtual void setGeodeticDatum(const QString& datum) = 0;
		virtual void setHorizontalCoords(const QString& hzcs) = 0;
		virtual void setProvince(const QString& province) = 0;
		virtual void setWellID(const QString& id) = 0;
		virtual void setLicence(const QString& licence) = 0;
		virtual void setState(const QString& state) = 0;
		virtual void setCounty(const QString& county) = 0;
		virtual void setAPI(const QString& api) = 0;
		virtual void addWellParameter(const QString& mnemonic, const QString& unit, const QString& value, const QString& description, const QString& format = "") = 0;

		// log parameter section
		virtual void addLogParameter(const QString& mnemonic, const QString& unit, const QString& value, const QString& description, const QString& format = "", const QString& association = "") = 0;

		// log definition section
		virtual void addLogDefinition(const QString& mnemonic, const QString& unit, const QString& code, const QString& description, const QString& format = "", const QString& association = "") = 0;

		// log data section
		virtual void addLodData(const QStringList& data) = 0;

	};
}
