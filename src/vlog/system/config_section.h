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
