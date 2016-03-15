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

#include "plugin_data.h"

namespace General
{
	CExportLas2Data::SLine::SLine(const QString& m, const QString& u, const QString& v, const QString& d)
		: mnem(m), unit(u), value(v), desc(d)
	{
	}

	QString CExportLas2Data::SLine::toString() const
	{
		QString addon;
		QString string;

		string = mnem;
		addon.fill(' ', (string.size() < 8 ? 8 - string.size() : 0));
		string += addon;

		string += "." + unit;
		addon.fill(' ', (string.size() < 16 ? 16 - string.size() : 1));
		string += addon;

		string += value;
		addon.fill(' ', (string.size() < 28 ? 28 - string.size() : 1));
		string += addon;

		string += ":" + desc;
		addon.fill(' ', (string.size() < 51 ? 51 - string.size() : 1));
		string += addon;

		return string;
	}

	CExportLas2Data::CExportLas2Data()
	{
		initVersion();
		initWell();
	}

	CExportLas2Data::~CExportLas2Data()
	{
	}

	QString CExportLas2Data::getVersion()
	{
		return "2.0";
	}

	QStringList CExportLas2Data::getHeader()
	{
		QStringList list;
		list.push_back("Digital logging by VLog");
		list.push_back("e-mail: aaaaaa@aaaaaa.aaa");
		list.push_back("www: http://aaaaaa.aaaaaa.aaa");

		return list;
	}

	void CExportLas2Data::setWrap(const QString& wrap)
	{
		changeVersionLine("WRAP", wrap);
	}

	void CExportLas2Data::setDlm(const QString& dlm)
	{
		Q_UNUSED(dlm);
	}

	void CExportLas2Data::setStart(const QString& start, const QString unit)
	{
		changeWellLine("STRT", start, unit);
	}

	void CExportLas2Data::setStop(const QString& stop, const QString unit)
	{
		changeWellLine("STOP", stop, unit);
	}

	void CExportLas2Data::setStep(const QString& step, const QString unit)
	{
		changeWellLine("STEP", step, unit);
	}

	void CExportLas2Data::setNull(const QString& null)
	{
		changeWellLine("NULL", null);
	}

	void CExportLas2Data::setCompany(const QString& company)
	{
		changeWellLine("COMP", company);
	}

	void CExportLas2Data::setWell(const QString& well)
	{
		changeWellLine("WELL", well);
	}

	void CExportLas2Data::setField(const QString& field)
	{
		changeWellLine("FLD", field);
	}

	void CExportLas2Data::setLocation(const QString& location)
	{
		changeWellLine("LOC", location);
	}

	void CExportLas2Data::setCountry(const QString& country)
	{
		changeWellLine("CTRY", country);
	}

	void CExportLas2Data::setServiceCompany(const QString& company)
	{
		changeWellLine("SRVC", company);
	}

	void CExportLas2Data::setDate(const QString& date, const QString format)
	{
		changeWellLine("DATE", date, "", format);
	}

	void CExportLas2Data::setLatitude(const QString& latitude, const QString& format)
	{
		changeWellLine("LATI", latitude, "", format);
	}

	void CExportLas2Data::setLongitude(const QString& longitude, const QString& format)
	{
		changeWellLine("LONG", longitude, "", format);
	}

	void CExportLas2Data::setX(const QString& x)
	{
		changeWellLine("X", x);
	}

	void CExportLas2Data::setY(const QString& y)
	{
		changeWellLine("Y", y);
	}

	void CExportLas2Data::setGeodeticDatum(const QString& datum)
	{
		changeWellLine("GDAT", datum);
	}

	void CExportLas2Data::setHorizontalCoords(const QString& hzcs)
	{
		changeWellLine("HZCS", hzcs);
	}

	void CExportLas2Data::setProvince(const QString& province)
	{
		changeWellLine("PROV", province);
	}

	void CExportLas2Data::setWellID(const QString& id)
	{
		changeWellLine("UWI", id);
	}

	void CExportLas2Data::setLicence(const QString& licence)
	{
		changeWellLine("LIC", licence);
	}

	void CExportLas2Data::setState(const QString& state)
	{
		changeWellLine("STAT", state);
	}

	void CExportLas2Data::setCounty(const QString& county)
	{
		changeWellLine("CNTY", county);
	}

	void CExportLas2Data::setAPI(const QString& api)
	{
		changeWellLine("API", api);
	}

	void CExportLas2Data::addWellParameter(const QString& mnemonic, const QString& unit, const QString& value, const QString& description, const QString& format)
	{
		Q_UNUSED(format);

		m_well.push_back(SLine(mnemonic, unit, value, description));
	}

	void CExportLas2Data::addLogParameter(const QString& mnemonic, const QString& unit, const QString& value, const QString& description, const QString& format, const QString& association)
	{
		Q_UNUSED(format);
		Q_UNUSED(association);

		m_log_parameters.push_back(SLine(mnemonic, unit, value, description));
	}

	void CExportLas2Data::addLogDefinition(const QString& mnemonic, const QString& unit, const QString& code, const QString& description, const QString& format, const QString& association)
	{
		Q_UNUSED(format);
		Q_UNUSED(association);

		m_log_definitions.push_back(SLine(mnemonic, unit, code, description));
	}

	void CExportLas2Data::addLodData(const QStringList& data)
	{
		m_log_data.push_back(data);
	}

	void CExportLas2Data::clean()
	{
		m_version.clear();
		initVersion();

		m_well.clear();
		initWell();

		m_log_parameters.clear();
		m_log_definitions.clear();
		m_log_data.clear();
	}

	void CExportLas2Data::saveToFile(const QString& filename)
	{
		QFile file(filename);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			return;
		}

		QTextStream stream(&file);

		QStringList header = getHeader();
		foreach (const QString& s, header)
		{
			stream << "# " << s << "\n";
		}

		stream << "\n" << "~Version" << "\n";
		stream << "#MNEM    UNIT    VALUE       DESCRIPTION" << "\n";
		stream << "#----    ----    --------    ---------------------" << "\n";
		for (int i = 0; i < m_version.size(); ++i)
		{
			stream << " " << m_version.at(i).toString() << "\n";
		}

		stream << "\n" << "~Well" << "\n";
		stream << "#MNEM    UNIT    VALUE       DESCRIPTION" << "\n";
		stream << "#----    ----    --------    ---------------------" << "\n";
		for (int i = 0; i < m_well.size(); ++i)
		{
			stream << " " << m_well.at(i).toString() << "\n";
		}

		stream << "\n" << "~Parameter" << "\n";
		stream << "#MNEM    UNIT    VALUE       DESCRIPTION" << "\n";
		stream << "#----    ----    --------    ---------------------" << "\n";
		for (int i = 0; i < m_log_parameters.size(); ++i)
		{
			stream << " " << m_log_parameters.at(i).toString() << "\n";
		}

		stream << "\n" << "~Curve" << "\n";
		stream << "#MNEM    UNIT    API CODE    DESCRIPTION" << "\n";
		stream << "#----    ----    ----------  ---------------------" << "\n";
		for (int i = 0; i < m_log_definitions.size(); ++i)
		{
			stream << " " << m_log_definitions.at(i).toString() << "\n";
		}

		stream << "\n" << "~Ascii" << "\n";
		for (int i = 0; i < m_log_data.size(); ++i)
		{
			stream << " " << m_log_data.at(i).join(" ") << "\n";
		}

		file.close();
	}

	void CExportLas2Data::initVersion()
	{
		m_version.push_back(SLine("VERS", "", "2.0", "LAS version identifier"));
		m_version.push_back(SLine("WRAP", "", "NO", "WRAP mode"));
	}

	void CExportLas2Data::changeVersionLine(const QString& mnem, const QString& value)
	{
		int index;
		for (index = 0; index < m_version.size(); ++index)
		{
			if (m_version.at(index).mnem.toUpper() == mnem.toUpper())
			{
				break;
			}
		}

		if (index != m_version.size())
		{
			SLine line = m_version.at(index);
			line.value = value;
			m_version[index] = line;
		}
	}

	void CExportLas2Data::initWell()
	{
		m_well.push_back(SLine("STRT", "M", "", "First index value"));
		m_well.push_back(SLine("STOP", "M", "", "Last index value"));
		m_well.push_back(SLine("STEP", "M", "", "Step of index"));
		m_well.push_back(SLine("NULL", "", "", "Null value"));
		m_well.push_back(SLine("COMP", "", "", "Company"));
		m_well.push_back(SLine("WELL", "", "", "Well"));
		m_well.push_back(SLine("FLD", "", "", "Field"));
		m_well.push_back(SLine("LOC", "", "", "Location"));
		m_well.push_back(SLine("CTRY", "", "", "Country"));
		m_well.push_back(SLine("SRVC", "", "", "Service company"));
		m_well.push_back(SLine("DATE", "", "", "Service date"));
		m_well.push_back(SLine("PROV", "", "", "Province"));
		m_well.push_back(SLine("UWI", "", "", "Unique well ID"));
		m_well.push_back(SLine("LIC", "", "", "License number"));
		m_well.push_back(SLine("STAT", "", "", "State"));
		m_well.push_back(SLine("CNTY", "", "", "County"));
		m_well.push_back(SLine("API", "", "", "API number"));
	}

	void CExportLas2Data::changeWellLine(const QString& mnem, const QString& value, const QString& unit, const QString& format)
	{
		Q_UNUSED(format);

		int index;
		for (index = 0; index < m_well.size(); ++index)
		{
			if (m_well.at(index).mnem.toUpper() == mnem.toUpper())
			{
				break;
			}
		}

		if (index != m_well.size())
		{
			SLine line = m_well.at(index);
			line.unit = unit;
			line.value = value;
			m_well[index] = line;
		}
		else
		{
			addWellParameter(mnem, unit, value, "");
		}
	}
}
