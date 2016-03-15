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

#include "prop_item.h"

namespace General
{
	CPropItem::CPropItem()
		: m_prop(0), m_readonly(false), m_expanded(true)
	{
		m_type_names.insert("group", PT_GROUP);
		m_type_names.insert("bool", PT_BOOL);
		m_type_names.insert("int", PT_INT);
		m_type_names.insert("double", PT_DOUBLE);
		m_type_names.insert("string", PT_STRING);
		m_type_names.insert("date", PT_DATE);
		m_type_names.insert("point", PT_POINT);
		m_type_names.insert("size", PT_SIZE);
		m_type_names.insert("enum", PT_ENUM);
		m_type_names.insert("font", PT_FONT);
		m_type_names.insert("color", PT_COLOR);

		m_name = "prop_name";
		m_type = PT_INT;
		m_caption = "PropertyCaption";
		m_min = 0.0;
		m_max = 100.0;
		m_step = 1.0;
		m_decimals = 4.0;
	}

	CPropItem::~CPropItem()
	{
		for (int i = 0; i < m_childs.size(); ++i)
		{
			CPropItem *item = m_childs.at(i);

			delete item;
		}
	}

	int CPropItem::childCount() const
	{
		return m_childs.size();
	}

	CPropItem* CPropItem::childAt(int index)
	{
		return m_childs.at(index);
	}

	void CPropItem::addChild(CPropItem* child)
	{
		m_childs.push_back(child);
	}

	int CPropItem::handleCount() const
	{
		return m_handles.size();
	}

	QString CPropItem::handleAt(int index)
	{
		return m_handles.at(index);
	}

	void CPropItem::addHandle(const QString& handle)
	{
		m_handles.push_back(handle);
	}

	QtVariantProperty* CPropItem::editorProperty()
	{
		return m_prop;
	}

	void CPropItem::setEditorProperty(QtVariantProperty* prop)
	{
		m_prop = prop;
	}

	bool CPropItem::isReadOnly() const
	{
		return m_readonly;
	}

	void CPropItem::setReadOnly(bool readonly)
	{
		m_readonly = readonly;
	}

	bool CPropItem::isExpanded() const
	{
		return m_expanded;
	}

	void CPropItem::setExpanded(bool expanded)
	{
		m_expanded = expanded;
	}

	QString CPropItem::name() const
	{
		return m_name;
	}

	void CPropItem::setName(const QString& name)
	{
		m_name = name;
	}

	CPropItem::EPropType CPropItem::type() const
	{
		return m_type;
	}

	void CPropItem::setType(EPropType type)
	{
		m_type = type;
	}

	void CPropItem::setType(const QString& type)
	{
		QMap<QString, EPropType>::iterator iter = m_type_names.find(type);

		if (iter != m_type_names.end())
		{
			m_type = iter.value();
		}
		else
		{
			m_type = PT_INT;
		}
	}

	QString CPropItem::caption() const
	{
		return m_caption;
	}

	void CPropItem::setCaption(const QString& caption)
	{
		m_caption = caption;
	}

	qreal CPropItem::minimum() const
	{
		return m_min;
	}

	void CPropItem::setMinimum(qreal value)
	{
		m_min = value;
	}

	qreal CPropItem::maximum() const
	{
		return m_max;
	}

	void CPropItem::setMaximum(qreal value)
	{
		m_max = value;
	}

	qreal CPropItem::step() const
	{
		return m_step;
	}

	void CPropItem::setStep(qreal value)
	{
		m_step = value;
	}

	int CPropItem::decimals() const
	{
		return m_decimals;
	}

	void CPropItem::setDecimals(int value)
	{
		m_decimals = value;
	}

	QStringList CPropItem::enumNames() const
	{
		return m_enum_names;
	}

	void CPropItem::setEnumNames(const QStringList& names)
	{
		m_enum_names = names;
	}

	void CPropItem::setEnumNames(const QString& names)
	{
		m_enum_names = names.split(";");
	}

	QVariant CPropItem::initValue() const
	{
		return m_init_value;
	}

	void CPropItem::setInitValue(const QVariant& value)
	{
		m_init_value = value;
	}
}
