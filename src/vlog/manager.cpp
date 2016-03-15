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

#include "manager.h"

namespace General
{
	CObjectManager::CObjectManager(QObject* parent)
		: QObject(parent), m_active_root(0), m_active_object(0)
	{
		createPropManager();
		createObjectTree();
	}

	CObjectManager::~CObjectManager()
	{
	}

	QWidget* CObjectManager::getTreeWidget() const
	{
		return m_object_tree;
	}

	QWidget* CObjectManager::getPropertyWidget() const
	{
		return m_prop_manager;
	}

	void CObjectManager::addProject(CObject* object)
	{
		if (!object->isRoot())
		{
			return;
		}

		slotObjectRegister(object);

		setCurrentProject(object);
	}

	void CObjectManager::removeProject(CObject* object)
	{
		for (int i = 0; i < m_object_tree->topLevelItemCount(); ++i)
		{
			QTreeWidgetItem *item = m_object_tree->topLevelItem(i);

			unsigned int pint = item->data(0, Qt::UserRole).toUInt();
			CObject *item_object = reinterpret_cast<CObject*>(pint);

			if (item_object == object)
			{
				m_object_tree->removeItemWidget(item, 0);

				delete item;

				break;
			}
		}

		if (m_object_tree->topLevelItemCount())
		{
			QTreeWidgetItem *item = m_object_tree->topLevelItem(0);

			unsigned int pint = item->data(0, Qt::UserRole).toUInt();
			CObject *item_object = reinterpret_cast<CObject*>(pint);

			setCurrentProject(item_object);
		}

		clearProperties();
	}

	void CObjectManager::setCurrentProject(CObject* object)
	{
		for (int i = 0; i < m_object_tree->topLevelItemCount(); ++i)
		{
			QTreeWidgetItem *item = m_object_tree->topLevelItem(i);

			unsigned int pint = item->data(0, Qt::UserRole).toUInt();
			CObject *item_object = reinterpret_cast<CObject*>(pint);

			if (item_object == object)
			{
				m_active_root = item;

				slotTreeItemSelect(item);

				break;
			}
		}
	}

	void CObjectManager::installSharedProps(const QString& uid, const QString& props)
	{
		// установка общих свойств

		if (m_prop_manager->installSharedProps(props))
		{
			CLog::getInstance()->information(tr("Installed properties for UID: \"") + uid + tr("\""));
		}
		else
		{
			CLog::getInstance()->warning(tr("Can't install properties for UID: \"") + uid + tr("\""));
		}
	}

	void CObjectManager::slotObjectRegister(CObject* object)
	{
		// регистрация объекта в менеджере объектов

		QString name = object->caption().isEmpty() ? object->name() : object->caption();
		CObject *parent = object->fantomParent();
		QIcon icon = object->icon();

		connect(object, SIGNAL(signalPropertiesUpdated(CObject*)), this, SLOT(slotObjectPropertiesUpdated(CObject*)));
		connect(object, SIGNAL(signalPropAttrUpdate(CObject*, const QString&, const QString&, const QVariant&)), this, SLOT(slotObjectPropAttrUpdate(CObject*, const QString&, const QString&, const QVariant&)));

		if (!object->isPropertiesLoaded())
		{
			slotObjectPropertiesFill(object);
		}

		QTreeWidgetItem *item = new QTreeWidgetItem();

		item->setText(0, name);
		item->setIcon(0, icon);
		item->setData(0, Qt::UserRole, reinterpret_cast<t_pointer>(object));

		if (parent)
		{
			for (int i = 0; i < m_object_tree->topLevelItemCount(); ++i)
			{
				QTreeWidgetItem *ret_item = findTreeWidgetItem(parent, m_object_tree->topLevelItem(i));

				if (ret_item)
				{
					ret_item->addChild(item);

					m_object_tree->setCurrentItem(item);

					break;
				}
			}
		}
		else
		{
			if (object->isRoot())
			{
				m_object_tree->addTopLevelItem(item);
				m_object_tree->setCurrentItem(item);
			}
			else if (m_active_root)
			{
				if (object->name() == tr("Log"))
				{
					m_active_root->insertChild(0, item);
				}

				m_active_root->addChild(item);
				m_object_tree->setCurrentItem(item);
			}
		}
	}

	void CObjectManager::slotObjectUnregister(CObject* object)
	{
		// отмена регистрации объекта в менеджере объектов

		for (int i = 0; i < m_object_tree->topLevelItemCount(); ++i)
		{
			QTreeWidgetItem *ret_item = findTreeWidgetItem(object, m_object_tree->topLevelItem(i));

			if (ret_item)
			{
				m_object_tree->removeItemWidget(ret_item, 0);

				delete ret_item;

				break;
			}
		}

		slotTreeItemSelect(m_object_tree->currentItem());
	}

	void CObjectManager::slotExternalObjectActivation(CObject* object)
	{
		// объект был активирован извне

		QTreeWidgetItem *curr_item = m_object_tree->currentItem();

		if (curr_item)
		{
			unsigned int pint = curr_item->data(0, Qt::UserRole).toUInt();
			CObject *item_object = reinterpret_cast<CObject*>(pint);

			if (item_object == object)
			{
				return;
			}
		}

		for (int i = 0; i < m_object_tree->topLevelItemCount(); ++i)
		{
			QTreeWidgetItem *ret_item = findTreeWidgetItem(object, m_object_tree->topLevelItem(i));

			if (ret_item)
			{
				m_object_tree->setCurrentItem(ret_item);

				break;
			}
		}
	}

	void CObjectManager::slotObjectCaptionChanged(CObject* object)
	{
		// заголовок объекта было изменен

		for (int i = 0; i < m_object_tree->topLevelItemCount(); ++i)
		{
			QTreeWidgetItem *ret_item = findTreeWidgetItem(object, m_object_tree->topLevelItem(i));

			if (ret_item)
			{
				ret_item->setText(0, object->caption().isEmpty() ? object->name() : object->caption());

				break;
			}
		}
	}

	void CObjectManager::slotObjectPropertiesUpdated(CObject* object)
	{
		// отображение измененных свойств объекта в редакторе

		if (object == m_active_object)
		{
			QString name = object->name();

			for (int i = 0; i < m_prop_manager->itemCount(name); ++i)
			{
				CPropItem *item = m_prop_manager->itemAt(name, i);

				objectPropertiesUpdated(object, item);
			}
		}
	}

	void CObjectManager::slotObjectPropAttrUpdate(CObject* object, const QString& prop, const QString& attrib, const QVariant& value)
	{
		// смена атрибута свойства объекта

		m_prop_manager->changeObjectPropertyAttribute(object->name(), prop, attrib, value);
	}

	void CObjectManager::slotObjectPropertiesFill(CObject* object)
	{
		// заполнение свойств объекта начальными значениями

		clearProperties();

		for (int i = 0; i < m_prop_manager->itemCount(object->name()); ++i)
		{
			CPropItem *item = m_prop_manager->itemAt(object->name(), i);

			objectPropertyFill(object, item);
		}
	}

	void CObjectManager::slotTreeItemSelect(QTreeWidgetItem* item)
	{
		if (item)
		{
			unsigned int pint = item->data(0, Qt::UserRole).toUInt();
			CObject *object = reinterpret_cast<CObject*>(pint);

			if (m_active_object == object)
			{
				return;
			}

			m_active_object = object;

			if (m_active_object)
			{
				reloadProperties(m_active_object);

				emit signalObjectActivated(m_active_object);
			}
			else
			{
				clearProperties();
			}
		}
		else
		{
			clearProperties();
		}
	}

	void CObjectManager::slotTreeItemDelete(QTreeWidgetItem* item)
	{
		unsigned int pint = item->data(0, Qt::UserRole).toUInt();
		CObject *object = reinterpret_cast<CObject*>(pint);

		if (object != 0)
		{
			emit signalObjectDeletion(object);
		}
	}

	void CObjectManager::slotPropertyValueChanged(const QString& caption, const QVariant& value)
	{
		// свойство было изменено в редакторе

		if (m_active_object)
		{
			m_active_object->setPropertyValue(m_prop_manager->propertyNameByCaption(m_active_object->name(), caption), value);
		}
	}

	void CObjectManager::slotCurrentPropertyChanged(const QString &caption)
	{
		// выделено другое свойство в редакторе

		Q_UNUSED(caption);
	}

	void CObjectManager::createObjectTree()
	{
		m_object_tree = new CTree();

		connect(m_object_tree, SIGNAL(signalItemSelect(QTreeWidgetItem*)), this, SLOT(slotTreeItemSelect(QTreeWidgetItem*)));
		connect(m_object_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(slotTreeItemSelect(QTreeWidgetItem*)));
		connect(m_object_tree, SIGNAL(signalItemDelete(QTreeWidgetItem*)), this, SLOT(slotTreeItemDelete(QTreeWidgetItem*)));
	}


	void CObjectManager::createPropManager()
	{
		m_prop_manager = new CPropManager();

		connect(m_prop_manager, SIGNAL(signalPropertyValueChanged(const QString&, const QVariant&)), this, SLOT(slotPropertyValueChanged(const QString&, const QVariant&)));
		connect(m_prop_manager, SIGNAL(signalCurrentPropertyChanged(const QString&)), this, SLOT(slotCurrentPropertyChanged(const QString&)));


		QString language = CConfig::getInstance()->getSection("interface")->getParamString("language").toLower();
		QString filename = language != "english" ? "properties_" + language + ".xml" : "properties.xml";

		if (m_prop_manager->loadTemplate(filename))
		{
			CLog::getInstance()->information(tr("General template of properties loaded"));
		}
		else
		{
			QFile prop(":files/properties.xml");

			if (m_prop_manager->loadTemplate(&prop))
			{
				CLog::getInstance()->warning(tr("Can't load properties.xml. Properties load by default"));
			}
			else
			{
				CLog::getInstance()->error("Properties template not loaded");
			}
		}
	}

	QTreeWidgetItem* CObjectManager::findTreeWidgetItem(CObject *object, QTreeWidgetItem* curr)
	{
		unsigned int pint = curr->data(0, Qt::UserRole).toUInt();
		CObject *item_object = reinterpret_cast<CObject*>(pint);

		if (item_object == object)
		{
			return curr;
		}

		for (int i = 0; i < curr->childCount(); ++i)
		{
			QTreeWidgetItem *item = findTreeWidgetItem(object, curr->child(i));

			if (item)
			{
				return item;
			}
		}

		return 0;
	}

	void CObjectManager::objectPropertiesUpdated(CObject* object, CPropItem* item)
	{
		// рекурсивный спуск для slotObjectPropertiesUpdated()

		if (item->type() != CPropItem::PT_GROUP)
		{
			m_prop_manager->updateProperty(item, object->propertyValue(item->name()));
		}

		for (int i = 0; i < item->childCount(); ++i)
		{
			objectPropertiesUpdated(object, item->childAt(i));
		}
	}

	void CObjectManager::objectPropertyFill(CObject* object, CPropItem* item)
	{
		// рекурсивный спуск для slotObjectPropertiesFill()

		if (item->type() != CPropItem::PT_GROUP)
		{
			object->setPropertyValue(item->name(), item->initValue());
		}

		for (int i = 0; i < item->childCount(); ++i)
		{
			objectPropertyFill(object, item->childAt(i));
		}
	}

	void CObjectManager::reloadProperties(CObject* object)
	{
		// загрузка свойств объекта в редактор

		m_prop_manager->clearProperties();

		m_prop_manager->loadProperties(object->name());

		slotObjectPropertiesUpdated(object);
	}

	void CObjectManager::clearProperties()
	{
		// удаление свойств из редактора

		m_prop_manager->clearProperties();
	}
}
