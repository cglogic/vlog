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
