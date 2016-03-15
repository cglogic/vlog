#pragma once

#include "config.h"

#include "system/log.h"
#include "props/prop_manager.h"
#include "system/config.h"
#include "system/config_section.h"

#include "tree.h"
#include "object.h"

#include <QObject>
#include <QVector>
#include <QHeaderView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>

namespace General
{
	class CObjectManager
		: public QObject
	{
		Q_OBJECT

	public:
		explicit CObjectManager(QObject* parent = 0);
		virtual ~CObjectManager();

		QWidget* getTreeWidget() const;
		QWidget* getPropertyWidget() const;

		void addProject(CObject* object);
		void removeProject(CObject* object);
		void setCurrentProject(CObject* object);

		void installSharedProps(const QString& uid, const QString &props);

	signals:
		void signalObjectActivated(CObject* object);
		void signalObjectDeletion(CObject* object);

	public slots:
		void slotObjectRegister(CObject* object);
		void slotObjectUnregister(CObject* object);
		void slotExternalObjectActivation(CObject* object);
		void slotObjectCaptionChanged(CObject* object);

	private slots:
		void slotObjectPropertiesUpdated(CObject* object);
		void slotObjectPropAttrUpdate(CObject* object, const QString& prop, const QString& attrib, const QVariant& value);
		void slotObjectPropertiesFill(CObject* object);

		void slotTreeItemSelect(QTreeWidgetItem* item);
		void slotTreeItemDelete(QTreeWidgetItem* item);

		void slotPropertyValueChanged(const QString& caption, const QVariant& value);
		void slotCurrentPropertyChanged(const QString& caption);


	private:
		CTree *m_object_tree;

		QTreeWidgetItem *m_active_root;
		CObject *m_active_object;

		void createObjectTree();

		CPropManager *m_prop_manager;

		void createPropManager();

		QTreeWidgetItem* findTreeWidgetItem(CObject *object, QTreeWidgetItem* curr);

		void objectPropertiesUpdated(CObject* object, CPropItem* item);
		void objectPropertyFill(CObject* object, CPropItem* item);

		void reloadProperties(CObject *object);
		void clearProperties();

	};
}
