#pragma once

#include "config.h"

#include "system/dialog.h"
#include "object.h"

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QtXml/QtXml>

namespace General
{
	class CProject
		: public CObject
	{
		Q_OBJECT

	public:
		CProject();
		virtual ~CProject();

		QDomDocument& document();

		bool create(const QString& name, const QString& path);
		QDomElement open(const QString& filename);
		bool save();
		bool close();

		virtual QVariant propertyValue(const QString& name) const;
		virtual void setPropertyValue(const QString& name, const QVariant& value);

		virtual bool isRoot() const;

	private:
		QString m_project_name;
		QString m_project_path;

		QDomDocument m_document;


	};
}
