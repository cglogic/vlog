#include "plugin_data.h"

namespace General
{
	CExportLas3Data::SLine::SLine(const QString& m, const QString& u, const QString& v, const QString& d, const QString& f, const QString& a)
		: mnem(m), unit(u), value(v), desc(d), format(f), assoc(a)
	{
	}

	QString CExportLas3Data::SLine::toString() const
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

		string += (format.isEmpty() ? "" : " {" + format + "}");
		addon.fill(' ', (string.size() < 64 ? 64 - string.size() : 1));
		string += addon;

		string += (assoc.isEmpty() ? "" : "|" + assoc);

		return string;
	}

	CExportLas3Data::CExportLas3Data()
		: m_delimiter(",")
	{
		initVersion();
		initWell();
	}

	CExportLas3Data::~CExportLas3Data()
	{
	}

	QString CExportLas3Data::getVersion()
	{
		return "3.0";
	}

	QStringList CExportLas3Data::getHeader()
	{
		QStringList list;
		list.push_back("Digital logging by VLog");
		list.push_back("e-mail: aaaaaa@aaaaaa.aaa");
		list.push_back("www: http://aaaaaa.aaaaaa.aaa");

		return list;
	}

	void CExportLas3Data::setWrap(const QString& wrap)
	{
		Q_UNUSED(wrap);
	}

	void CExportLas3Data::setDlm(const QString& dlm)
	{
		changeVersionLine("DLM", dlm);

		if (dlm.toUpper() == "COMMA")
		{
			m_delimiter = ",";
		}
		else if (dlm.toUpper() == "SPACE")
		{
			m_delimiter = " ";
		}
		else if (dlm.toUpper() == "TAB")
		{
			m_delimiter = "\t";
		}
	}

	void CExportLas3Data::setStart(const QString& start, const QString unit)
	{
		changeWellLine("STRT", start, unit);
	}

	void CExportLas3Data::setStop(const QString& stop, const QString unit)
	{
		changeWellLine("STOP", stop, unit);
	}

	void CExportLas3Data::setStep(const QString& step, const QString unit)
	{
		changeWellLine("STEP", step, unit);
	}

	void CExportLas3Data::setNull(const QString& null)
	{
		changeWellLine("NULL", null);
	}

	void CExportLas3Data::setCompany(const QString& company)
	{
		changeWellLine("COMP", company);
	}

	void CExportLas3Data::setWell(const QString& well)
	{
		changeWellLine("WELL", well);
	}

	void CExportLas3Data::setField(const QString& field)
	{
		changeWellLine("FLD", field);
	}

	void CExportLas3Data::setLocation(const QString& location)
	{
		changeWellLine("LOC", location);
	}

	void CExportLas3Data::setCountry(const QString& country)
	{
		changeWellLine("CTRY", country);
	}

	void CExportLas3Data::setServiceCompany(const QString& company)
	{
		changeWellLine("SRVC", company);
	}

	void CExportLas3Data::setDate(const QString& date, const QString format)
	{
		changeWellLine("DATE", date, "", format);
	}

	void CExportLas3Data::setLatitude(const QString& latitude, const QString& format)
	{
		changeWellLine("LATI", latitude, "", format);
	}

	void CExportLas3Data::setLongitude(const QString& longitude, const QString& format)
	{
		changeWellLine("LONG", longitude, "", format);
	}

	void CExportLas3Data::setX(const QString& x)
	{
		changeWellLine("X", x);
	}

	void CExportLas3Data::setY(const QString& y)
	{
		changeWellLine("Y", y);
	}

	void CExportLas3Data::setGeodeticDatum(const QString& datum)
	{
		changeWellLine("GDAT", datum);
	}

	void CExportLas3Data::setHorizontalCoords(const QString& hzcs)
	{
		changeWellLine("HZCS", hzcs);
	}

	void CExportLas3Data::setProvince(const QString& province)
	{
		changeWellLine("PROV", province);
	}

	void CExportLas3Data::setWellID(const QString& id)
	{
		changeWellLine("UWI", id);
	}

	void CExportLas3Data::setLicence(const QString& licence)
	{
		changeWellLine("LIC", licence);
	}

	void CExportLas3Data::setState(const QString& state)
	{
		changeWellLine("STAT", state);
	}

	void CExportLas3Data::setCounty(const QString& county)
	{
		changeWellLine("CNTY", county);
	}

	void CExportLas3Data::setAPI(const QString& api)
	{
		changeWellLine("API", api);
	}

	void CExportLas3Data::addWellParameter(const QString& mnemonic, const QString& unit, const QString& value, const QString& description, const QString& format)
	{
		m_well.push_back(SLine(mnemonic, unit, value, description, format));
	}

	void CExportLas3Data::addLogParameter(const QString& mnemonic, const QString& unit, const QString& value, const QString& description, const QString& format, const QString& association)
	{
		m_log_parameters.push_back(SLine(mnemonic, unit, value, description, format, association));
	}

	void CExportLas3Data::addLogDefinition(const QString& mnemonic, const QString& unit, const QString& code, const QString& description, const QString& format, const QString& association)
	{
		m_log_definitions.push_back(SLine(mnemonic, unit, code, description, format, association));
	}

	void CExportLas3Data::addLodData(const QStringList& data)
	{
		m_log_data.push_back(data);
	}

	void CExportLas3Data::clean()
	{
		m_delimiter = ",";

		m_version.clear();
		initVersion();

		m_well.clear();
		initWell();

		m_log_parameters.clear();
		m_log_definitions.clear();
		m_log_data.clear();
	}

	void CExportLas3Data::saveToFile(const QString& filename)
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
		stream << "#MNEM    UNIT    VALUE       DESCRIPTION             FORMAT" << "\n";
		stream << "#----    ----    --------    ---------------------   ----------" << "\n";
		for (int i = 0; i < m_well.size(); ++i)
		{
			stream << " " << m_well.at(i).toString() << "\n";
		}

		stream << "\n" << "~Parameter" << "\n";
		stream << "#MNEM    UNIT    VALUE       DESCRIPTION             FORMAT      ASSOCIATION" << "\n";
		stream << "#----    ----    --------    ---------------------   ----------  ----------------" << "\n";
		for (int i = 0; i < m_log_parameters.size(); ++i)
		{
			stream << " " << m_log_parameters.at(i).toString() << "\n";
		}

		stream << "\n" << "~Curve" << "\n";
		stream << "#MNEM    UNIT    API CODE    DESCRIPTION             FORMAT      ASSOCIATION" << "\n";
		stream << "#----    ----    ----------  ---------------------   ----------  ----------------" << "\n";
		for (int i = 0; i < m_log_definitions.size(); ++i)
		{
			stream << " " << m_log_definitions.at(i).toString() << "\n";
		}

		stream << "\n" << "~Ascii" << "\n";
		for (int i = 0; i < m_log_data.size(); ++i)
		{
			stream << " " << m_log_data.at(i).join(m_delimiter) << "\n";
		}

		file.close();
	}

	void CExportLas3Data::initVersion()
	{
		m_version.push_back(SLine("VERS", "", "3.0", "LAS version identifier"));
		m_version.push_back(SLine("WRAP", "", "NO", "WRAP mode"));
		m_version.push_back(SLine("DLM", "", "COMMA", "Column data section delimiter"));
	}

	void CExportLas3Data::changeVersionLine(const QString& mnem, const QString& value)
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

	void CExportLas3Data::initWell()
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
		m_well.push_back(SLine("DATE", "", "", "Service date", "DD/MM/YYYY"));
		m_well.push_back(SLine("LATI", "", "", "Latitude", "F"));
		m_well.push_back(SLine("LONG", "", "", "Longitude", "F"));
		m_well.push_back(SLine("X", "", "", "East-West coordinate", ""));
		m_well.push_back(SLine("Y", "", "", "North-South coordinate", ""));
		m_well.push_back(SLine("GDAT", "", "", "Geodetic datum", ""));
		m_well.push_back(SLine("HZCS", "", "", "Horiz. coord. system", ""));
		m_well.push_back(SLine("PROV", "", "", "Province", ""));
		m_well.push_back(SLine("UWI", "", "", "Unique well ID", ""));
		m_well.push_back(SLine("LIC", "", "", "License number", ""));
		m_well.push_back(SLine("STAT", "", "", "State", ""));
		m_well.push_back(SLine("CNTY", "", "", "County", ""));
		m_well.push_back(SLine("API", "", "", "API number", ""));
	}

	void CExportLas3Data::changeWellLine(const QString& mnem, const QString& value, const QString& unit, const QString& format)
	{
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
			line.format = format;
			m_well[index] = line;
		}
	}
}
