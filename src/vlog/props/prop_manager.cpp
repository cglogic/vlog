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

#include "prop_manager.h"

namespace General
{
	CPropManager::CPropManager()
		: m_curr_objname("")
	{
		m_group_manager = new QtGroupPropertyManager(this);
		m_variant_manager = new QtVariantPropertyManager(this);
		m_variant_factory = new QtVariantEditorFactory(this);

		setFactoryForManager(m_variant_manager, m_variant_factory);

		connect(m_variant_manager, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this, SLOT(slotPropertyValueChanged(QtProperty*, const QVariant&)));
		connect(this, SIGNAL(currentItemChanged(QtBrowserItem*)), this, SLOT(slotCurrentPropertyChanged(QtBrowserItem*)));
	}

	CPropManager::~CPropManager()
	{
		for (int i = 0; i < m_objects.size(); ++i)
		{
			SObjectProps *object = m_objects.at(i);

			for (int j = 0; j < object->props.size(); ++j)
			{
				CPropItem *item = object->props.at(j);

				delete item;
			}
		}
	}

	bool CPropManager::loadTemplate(const QString& filename)
	{
		// загрузка шаблона дерева свойств объектов из файла

		bool is_loaded = false;

		QFile file(filename);

		if (file.open(QIODevice::ReadOnly))
		{
			QDomDocument document;

			if (document.setContent(&file))
			{
				QDomElement element = document.documentElement();

				is_loaded = parseObjectsSection(element);
			}

			file.close();
		}

		return is_loaded;
	}

	bool CPropManager::loadTemplate(QFile* file)
	{
		bool is_loaded = false;

		if (file->open(QIODevice::ReadOnly))
		{
			QDomDocument document;

			if (document.setContent(file))
			{
				QDomElement element = document.documentElement();

				is_loaded = parseObjectsSection(element);
			}

			file->close();
		}

		return is_loaded;
	}

	bool CPropManager::installSharedProps(const QString& props)
	{
		QDomDocument document;

		if (document.setContent(props))
		{
			QDomElement element = document.documentElement();

			parseObjectsSection(element);

			return true;
		}

		return true;
	}

	bool CPropManager::isPropPathExists(const QString& path) const
	{
		for (int i = 0; i < m_prop_paths.size(); ++i)
		{
			if (m_prop_paths.at(i) == path)
			{
				return true;
			}
		}

		return false;
	}

	CPropItem* CPropManager::propItemByPath(QString path, CPropItem* item) const
	{
		if (item->name() == path)
		{
			return item;
		}

		for (int i = 0; i < item->childCount(); ++i)
		{
			CPropItem *child = item->childAt(i);

			if (path.mid(0, child->name().size()) == child->name())
			{
				CPropItem *res = propItemByPath(path.mid(child->name().size() + 1), child);

				if (res)
				{
					return res;
				}
			}
		}

		return 0;
	}

	bool CPropManager::parseObjectsSection(const QDomElement& element)
	{
		QDomElement objects = element.firstChildElement("object");

		while (!objects.isNull())
		{
			SObjectProps *object = 0;

			bool is_exists = false;

			QString objname = objects.attribute("name", "objectname");

			for (int i = 0; i < m_objects.size(); ++i)
			{
				if (m_objects.at(i)->objname == objname)
				{
					object = m_objects.at(i);

					is_exists = true;
				}
			}

			if (!object)
			{
				object = new SObjectProps();

				object->objname = objname;
			}

			QVector<CPropItem*> object_props = object->props;

			QDomElement props = objects.firstChildElement();

			while (!props.isNull())
			{
				if (props.tagName() == "prop" || props.tagName() == "group")
				{
					QString path = object->objname;

					CPropItem *item = createPropertyItem(props, path);

					if (item)
					{
						object_props.push_back(item);

						m_prop_paths.push_back(path + "/" + item->name());
					}
				}

				props = props.nextSiblingElement();
			}

			object->props = object_props;

			if (!is_exists)
			{
				m_objects.push_back(object);
			}

			objects = objects.nextSiblingElement("object");
		}

		return true;
	}

	int CPropManager::itemCount(const QString& objname) const
	{
		// вернуть количество свойств (корневых) объекта "objname"

		for (int i = 0; i < m_objects.size(); ++i)
		{
			SObjectProps *object = m_objects.at(i);

			if (object->objname == objname)
			{
				return object->props.size();
			}
		}

		return -1;
	}

	CPropItem* CPropManager::itemAt(const QString& objname, int index) const
	{
		// вернуть свойство (корневое) объекта "objname" по индексу

		for (int i = 0; i < m_objects.size(); ++i)
		{
			SObjectProps *object = m_objects.at(i);

			if (object->objname == objname)
			{
				if (index < object->props.size())
				{
					return object->props.at(index);
				}
				else
				{
					return 0;
				}
			}
		}

		return 0;
	}

	QString CPropManager::propertyNameByCaption(const QString& objname, const QString& caption)
	{
		// вернуть имя свойства по его заголовку

		for (int i = 0; i < m_objects.size(); ++i)
		{
			SObjectProps *object = m_objects.at(i);

			if (object->objname == objname)
			{
				for (int j = 0; j < object->props.size(); ++j)
				{
					QString name = propertyNameByCaption(caption, object->props.at(j));

					if (!name.isEmpty())
					{
						return name;
					}
				}
			}
		}

		return QString();
	}

	void CPropManager::loadProperties(const QString& objname)
	{
		// загрузка дерева свойств объекта "objname" в редактор свойств

		for (int i = 0; i < m_objects.size(); ++i)
		{
			SObjectProps *object = m_objects.at(i);

			if (object->objname == objname)
			{
				for (int j = 0; j < object->props.size(); ++j)
				{
					loadProperties(object->props.at(j), 0);
				}
			}
		}

		m_curr_objname = objname;
	}

	void CPropManager::clearProperties()
	{
		// очистка редактора свойств

		disconnect(m_variant_manager, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this, SLOT(slotPropertyValueChanged(QtProperty*, const QVariant&)));
		disconnect(this, SIGNAL(currentItemChanged(QtBrowserItem*)), this, SLOT(slotCurrentPropertyChanged(QtBrowserItem*)));

		// сохранение свернутости/развернутости свойств текущего объекта

		if (!m_curr_objname.isEmpty())
		{
			QList<QtBrowserItem*> items = topLevelItems();

			for (QList<QtBrowserItem*>::iterator iter = items.begin(); iter != items.end(); ++iter)
			{
				saveExpandedState(*iter);

				clearPropertyValue(*iter);
			}
		}

		clear();

		connect(m_variant_manager, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this, SLOT(slotPropertyValueChanged(QtProperty*, const QVariant&)));
		connect(this, SIGNAL(currentItemChanged(QtBrowserItem*)), this, SLOT(slotCurrentPropertyChanged(QtBrowserItem*)));
	}

	void CPropManager::updateProperty(CPropItem* item, const QVariant& value)
	{
		// обновление значения свойства в редакторе

		if (item->type() != CPropItem::PT_GROUP)
		{
			item->editorProperty()->setValue(value);
		}
	}

	void CPropManager::changeObjectPropertyAttribute(const QString& objname, const QString& prop, const QString& attrib, const QVariant& value)
	{
		// изменение атрибута свойства объекта

		for (int i = 0; i < m_objects.size(); ++i)
		{
			SObjectProps *object = m_objects.at(i);

			if (object->objname == objname)
			{
				for (int j = 0; j < object->props.size(); ++j)
				{
					if (changePropertyAttribute(object->props.at(j), prop, attrib, value))
					{
						return;
					}
				}
			}
		}
	}

	void CPropManager::slotPropertyValueChanged(QtProperty* prop, const QVariant& value)
	{
		// в редакторе было изменено значение свойства

		emit signalPropertyValueChanged(prop->propertyName(), value);
	}

	void CPropManager::slotCurrentPropertyChanged(QtBrowserItem* item)
	{
		// в редакторе стало активным другое свойство

		emit signalCurrentPropertyChanged(item->property()->propertyName());
	}

	void CPropManager::loadProperties(CPropItem* item, CPropItem* parent)
	{
		// рекурсивный спуск для loadProperties()

		QtBrowserItem *browser_item = 0;

		if (parent)
		{
			parent->editorProperty()->addSubProperty(item->editorProperty());
		}
		else
		{
			browser_item = addProperty(item->editorProperty());
		}

		for (int i = 0; i < item->childCount(); ++i)
		{
			loadProperties(item->childAt(i), item);
		}

		if (browser_item)
		{
			setExpanded(browser_item, item->isExpanded());
		}
	}

	CPropItem* CPropManager::createPropertyItem(const QDomElement& element, QString path)
	{
		// создание свойства по шаблону

		CPropItem *item = 0;

		path += "/" + element.attribute("name", "unname");

		bool path_exists = isPropPathExists(path);

		if (!path_exists)
		{
			item = new CPropItem();

			item->setName(element.attribute("name", item->name()));
			item->setType(element.attribute("type", "group"));
			item->setCaption(element.attribute("caption", item->caption()));
			item->setReadOnly(static_cast<bool>(element.attribute("readonly", QString::number(static_cast<int>(item->isReadOnly()))).toInt()));
			item->setExpanded(static_cast<bool>(element.attribute("expanded", QString::number(static_cast<int>(item->isExpanded()))).toInt()));
			item->setMinimum(element.attribute("min", QString::number(item->minimum())).toDouble());
			item->setMaximum(element.attribute("max", QString::number(item->maximum())).toDouble());
			item->setStep(element.attribute("step", QString::number(item->step())).toDouble());
			item->setDecimals(element.attribute("decimals", QString::number(item->decimals())).toInt());
			item->setEnumNames(element.attribute("enumnames", item->enumNames().join(";")));
			item->setInitValue(element.attribute("value", item->initValue().toString()));

			item->setEditorProperty(createEditorProperty(item));
		}
		else
		{
			for (int i = 0; i < m_objects.size() && !item; ++i)
			{
				SObjectProps *object = m_objects.at(i);

				if (object->objname == path.mid(0, object->objname.size()))
				{
					for (int j = 0; j < object->props.size() && !item; ++j)
					{
						item = propItemByPath(path.mid(object->objname.size() + 1), object->props.at(j));
					}
				}
			}

			if (!item)
			{
				return 0;
			}
		}

		if (item->type() == CPropItem::PT_GROUP)
		{
			QDomElement childs = element.firstChildElement();

			while (!childs.isNull())
			{
				if ((childs.tagName() == "prop" || childs.tagName() == "group"))
				{
					CPropItem *subitem = createPropertyItem(childs, path);

					if (subitem)
					{
						item->addChild(subitem);

						m_prop_paths.push_back(path + "/" + subitem->name());
					}
				}

				childs = childs.nextSiblingElement();
			}
		}

		return (!path_exists ? item : 0);
	}

	QtVariantProperty* CPropManager::createEditorProperty(CPropItem* item)
	{
		// создание свойства для редактора

		QtVariantProperty *prop = 0;

		if (item->type() == CPropItem::PT_GROUP)
		{
			prop = m_variant_manager->addProperty(QtVariantPropertyManager::groupTypeId(), item->caption());
		}

		else if (item->type() == CPropItem::PT_BOOL)
		{
			prop = m_variant_manager->addProperty(QVariant::Bool, item->caption());
		}

		else if (item->type() == CPropItem::PT_INT)
		{
			prop = m_variant_manager->addProperty(QVariant::Int, item->caption());

			prop->setAttribute("minimum", static_cast<int>(item->minimum()));
			prop->setAttribute("maximum", static_cast<int>(item->maximum()));
			prop->setAttribute("singleStep", static_cast<int>(item->step()));
		}

		else if (item->type() == CPropItem::PT_DOUBLE)
		{
			prop = m_variant_manager->addProperty(QVariant::Double, item->caption());

			prop->setAttribute("minimum", item->minimum());
			prop->setAttribute("maximum", item->maximum());
			prop->setAttribute("singleStep", item->step());
			prop->setAttribute("decimals", item->decimals());
		}

		else if (item->type() == CPropItem::PT_STRING)
		{
			prop = m_variant_manager->addProperty(QVariant::String, item->caption());
		}

		else if (item->type() == CPropItem::PT_DATE)
		{
			prop = m_variant_manager->addProperty(QVariant::Date, item->caption());
		}

		else if (item->type() == CPropItem::PT_POINT)
		{
			prop = m_variant_manager->addProperty(QVariant::PointF, item->caption());

			prop->setAttribute("decimals", item->decimals());
		}

		else if (item->type() == CPropItem::PT_SIZE)
		{
			prop = m_variant_manager->addProperty(QVariant::SizeF, item->caption());

			prop->setAttribute("decimals", item->decimals());
		}

		else if (item->type() == CPropItem::PT_ENUM)
		{
			prop = m_variant_manager->addProperty(QtVariantPropertyManager::enumTypeId(), item->caption());

			prop->setAttribute("enumNames", item->enumNames());
		}

		else if (item->type() == CPropItem::PT_FONT)
		{
			prop = m_variant_manager->addProperty(QVariant::Font, item->caption());
		}

		else if (item->type() == CPropItem::PT_COLOR)
		{
			prop = m_variant_manager->addProperty(QVariant::Color, item->caption());
		}

		if (prop)
		{
			prop->setEnabled(!item->isReadOnly());
		}

		return prop;
	}

	QString CPropManager::propertyNameByCaption(const QString &caption, CPropItem* item, CPropItem** out)
	{
		// вернуть имя свойства по его заголовку

		if (item->caption() == caption)
		{
			if (out)
			{
				*out = item;
			}

			return item->name();
		}

		for (int i = 0; i < item->childCount(); ++i)
		{
			QString name = propertyNameByCaption(caption, item->childAt(i), out);

			if (!name.isEmpty())
			{
				return name;
			}
		}

		return QString();
	}

	void CPropManager::saveExpandedState(QtBrowserItem* item)
	{
		for (int i = 0; i < m_objects.size(); ++i)
		{
			SObjectProps *object = m_objects.at(i);

			if (object->objname == m_curr_objname)
			{
				CPropItem *prop = 0;

				for (int j = 0; j < object->props.size(); ++j)
				{
					propertyNameByCaption(item->property()->propertyName(), object->props.at(j), &prop);

					if (prop)
					{
						prop->setExpanded(isExpanded(item));

						break;
					}
				}

				break;
			}
		}

		QList<QtBrowserItem*> childs = item->children();

		for (QList<QtBrowserItem*>::iterator iter = childs.begin(); iter != childs.end(); ++iter)
		{
			saveExpandedState(*iter);
		}
	}

	void CPropManager::clearPropertyValue(QtBrowserItem* item)
	{
		QtVariantProperty *prop = reinterpret_cast<QtVariantProperty*>(item->property());

		if (prop)
		{
			prop->setValue("");
		}

		QList<QtBrowserItem*> childs = item->children();

		for (QList<QtBrowserItem*>::iterator iter = childs.begin(); iter != childs.end(); ++iter)
		{
			clearPropertyValue(*iter);
		}
	}

	bool CPropManager::changePropertyAttribute(CPropItem* item, const QString& propname, const QString& attrib, const QVariant& value)
	{
		// рекурсивный спуск для changeObjectPropertyAttribute()

		if (item->name() == propname)
		{
			QtVariantProperty *prop = item->editorProperty();

			if (prop)
			{
				if (attrib == "decimals")
				{
					item->setDecimals(value.toInt());
					prop->setAttribute("decimals", value);
				}
				else if (attrib == "min")
				{
					item->setMinimum(value.toDouble());
					prop->setAttribute("minimum", value);
				}
				else if (attrib == "max")
				{
					item->setMaximum(value.toDouble());
					prop->setAttribute("maximum", value);
				}
				else if (attrib == "step")
				{
					item->setStep(value.toDouble());
					prop->setAttribute("singleStep", value);
				}
				else if (attrib == "enumnames")
				{
					item->setEnumNames(value.toString());
					prop->setAttribute("enumNames", value);
				}
				else if (attrib == "readonly")
				{
					item->setReadOnly(value.toBool());
					prop->setEnabled(!value.toBool());
				}

				return true;
			}
		}

		for (int i = 0; i < item->childCount(); ++i)
		{
			if (changePropertyAttribute(item->childAt(i), propname, attrib, value))
			{
				return true;
			}
		}

		return false;
	}
}
