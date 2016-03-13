#include "config.h"

namespace General
{
	CConfig::CConfig(const QString& filename)
		: m_filename(filename)
	{
	}

	CConfig::~CConfig()
	{
		save();

		destroySections();
	}

	CConfig* CConfig::getInstance()
	{
		static CConfig* instance = new CConfig("config.xml");

		return instance;
	}

	void CConfig::freeInstance()
	{
		delete this;
	}

	CConfigSection* CConfig::getSection(const QString& name)
	{
		if (isSection(name))
		{
			return m_sections.find(name).value();
		}

		return &m_dummy_section;
	}

	bool CConfig::isSection(const QString& name) const
	{
		return m_sections.contains(name);
	}

	bool CConfig::load(QFile* exists_file)
	{
		bool is_loaded = false;

		destroySections();

		QDomDocument document;

		if (!exists_file)
		{
			QFile file(m_filename);

			if (file.open(QIODevice::ReadOnly))
			{
				if (document.setContent(&file))
				{
					QDomElement element = document.documentElement();
					parseSections(element);

					is_loaded = true;
				}
			}

			file.close();
		}
		else
		{
			if (exists_file->open(QIODevice::ReadOnly))
			{
				if (document.setContent(exists_file))
				{
					QDomElement element = document.documentElement();
					parseSections(element);

					is_loaded = true;
				}
			}

			exists_file->close();
		}

		return is_loaded;
	}

	void CConfig::save()
	{
		QDomDocument document;

		QDomProcessingInstruction instruct = document.createProcessingInstruction("xml", QString("version=\"1.0\" encoding=\"utf-8\""));
		document.appendChild(instruct);

		QDomElement root = document.createElement("config");
		document.appendChild(root);

		for (QMap<QString, CConfigSection*>::const_iterator sect_iter = m_sections.constBegin(); sect_iter != m_sections.constEnd(); ++sect_iter)
		{
			QDomElement sect_elem = document.createElement(sect_iter.key());

			QMap<QString, QVariant> params = sect_iter.value()->getParams();
			for (QMap<QString, QVariant>::const_iterator param_iter = params.constBegin(); param_iter != params.constEnd(); ++param_iter)
			{
				QDomElement param_elem = document.createElement(param_iter.key());

				QDomText param_text = document.createTextNode(param_iter.value().toString());
				param_elem.appendChild(param_text);

				sect_elem.appendChild(param_elem);
			}

			root.appendChild(sect_elem);
		}

		QFile file(m_filename);
		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream(&file) << document.toString();
			file.close();
		}
	}

	void CConfig::parseSections(const QDomElement& sections)
	{
		QDomNode node = sections.firstChild();

		while (!node.isNull())
		{
			if (node.isElement())
			{
				QDomElement element = node.toElement();
				if (!element.isNull())
				{
					QString name = element.tagName();

					CConfigSection *section = new CConfigSection();
					addSection(name.toLower(), section);

					parseParams(section, element);
				}
			}

			node = node.nextSibling();
		}
	}

	void CConfig::parseParams(CConfigSection* section, const QDomElement& params)
	{
		QDomNode node = params.firstChild();

		while (!node.isNull())
		{
			if (node.isElement())
			{
				QDomElement element = node.toElement();
				if (!element.isNull())
				{
					QString name = element.tagName();
					QString value = element.text();

					section->setParam(name, value);
				}
			}

			node = node.nextSibling();
		}
	}

	void CConfig::addSection(const QString& name, CConfigSection* section)
	{
		if (!isSection(name))
		{
			m_sections[name] = section;
		}
	}

	void CConfig::destroySections()
	{
		for (QMap<QString, CConfigSection*>::iterator iter = m_sections.begin(); iter != m_sections.end(); ++iter)
		{
			CConfigSection *section = iter.value();

			delete section;
		}

		m_sections.clear();
	}
}
