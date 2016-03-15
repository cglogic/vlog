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
