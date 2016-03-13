#pragma once

#include "../config.h"

#include "prop_item.h"

#include "qttreepropertybrowser.h"
#include "qtpropertymanager.h"
#include "QtVariantPropertyManager"
#include "qteditorfactory.h"

#include <QString>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QtXml/QtXml>

namespace General
{
	class CPropManager
		: public QtTreePropertyBrowser
	{
		Q_OBJECT

	public:
		CPropManager();
		~CPropManager();

		bool loadTemplate(const QString& filename);
		bool loadTemplate(QFile* file);

		bool installSharedProps(const QString& props);

		int itemCount(const QString& objname) const;
		CPropItem* itemAt(const QString& objname, int index) const;

		QString propertyNameByCaption(const QString& objname, const QString& caption);

		void loadProperties(const QString& objname);

		void clearProperties();

		void updateProperty(CPropItem *item, const QVariant& value);

		void changeObjectPropertyAttribute(const QString& objname, const QString& prop, const QString& attrib, const QVariant& value);

	signals:
		void signalPropertyValueChanged(const QString& caption, const QVariant& value);
		void signalCurrentPropertyChanged(const QString& caption);

	private slots:
		void slotPropertyValueChanged(QtProperty* prop, const QVariant& value);
		void slotCurrentPropertyChanged(QtBrowserItem* item);

	private:
		struct SObjectProps
		{
			QString objname;
			QVector<CPropItem*> props;
		};

		struct SBrowserProp
		{
			QtBrowserItem *browser_item;
			CPropItem *prop_item;
		};

		QVector<SObjectProps*> m_objects;

		QVector<QString> m_prop_paths;

		bool isPropPathExists(const QString& path) const;
		CPropItem* propItemByPath(QString path, CPropItem* item) const;

		QString m_curr_objname;

		bool parseObjectsSection(const QDomElement& element);

		void loadProperties(CPropItem *item, CPropItem* parent);

		QtGroupPropertyManager *m_group_manager;
		QtVariantPropertyManager *m_variant_manager;
		QtVariantEditorFactory *m_variant_factory;

		CPropItem* createPropertyItem(const QDomElement& element, QString path);
		QtVariantProperty *createEditorProperty(CPropItem* item);

		QString propertyNameByCaption(const QString& caption, CPropItem* item, CPropItem** out = 0);

		void saveExpandedState(QtBrowserItem* item);
		void clearPropertyValue(QtBrowserItem* item);

		bool changePropertyAttribute(CPropItem* item, const QString& propname, const QString& attrib, const QVariant& value);

	};
}
