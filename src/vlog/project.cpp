#include "project.h"

namespace General
{
	CProject::CProject()
	{
		setName("Project");
		setIcon(QIcon(":/icons/folder.png"));
		setCaption(tr("Project"));
	}

	CProject::~CProject()
	{
	}

	QDomDocument& CProject::document()
	{
		m_document.clear();

		QDomProcessingInstruction instruct = m_document.createProcessingInstruction("xml", QString("version=\"1.0\" encoding=\"utf-8\""));
		m_document.appendChild(instruct);

		QDomElement project_elem = m_document.createElement("project");
		project_elem.setAttribute("version", "0.1.0");
		m_document.appendChild(project_elem);

		QDomElement shared_props = serializeSharedProps(m_document);
		project_elem.appendChild(shared_props);

		return m_document;
	}

	bool CProject::create(const QString& name, const QString& path)
	{
		QDir dir(path);

		dir.mkdir(name);
		dir.cd(name);

		m_project_name = name;
		m_project_path = dir.absolutePath();

		signalPropertiesUpdated(this);

		return true;
	}

	QDomElement CProject::open(const QString& filename)
	{
		QFile file(filename);

		if (file.open(QIODevice::ReadOnly))
		{
			QDomDocument document;

			if (document.setContent(&file))
			{
				QDomElement project_elem = document.documentElement();

				deserializeSharedProps(project_elem);

				m_project_name = QFileInfo(filename).baseName();
				m_project_path = QFileInfo(filename).canonicalPath();

				QDomElement project_name = document.createElement("project_name");
				project_name.setAttribute("value", m_project_name);
				project_elem.appendChild(project_name);

				QDomElement project_path = document.createElement("project_path");
				project_path.setAttribute("value", m_project_path);
				project_elem.appendChild(project_path);

				emit signalPropertiesUpdated(this);

				return project_elem;
			}
		}

		return QDomElement();
	}

	bool CProject::save()
	{
		QString filename = m_project_path + "/" + m_project_name + ".vpr";

		QFile file(filename);

		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream(&file) << m_document.toString();

			file.close();
		}

		return true;
	}

	bool CProject::close()
	{
		return true;
	}

	QVariant CProject::propertyValue(const QString& name) const
	{
		if (name == "project_name")
		{
			return m_project_name;
		}
		else if (name == "project_path")
		{
			return m_project_path;
		}

		return CObject::propertyValue(name);
	}

	void CProject::setPropertyValue(const QString& name, const QVariant& value)
	{
		if (name == "project_name")
		{
		}
		else if (name == "project_path")
		{
		}
		else
		{
			CObject::setPropertyValue(name, value);
		}
	}

	bool CProject::isRoot() const
	{
		return true;
	}
}
