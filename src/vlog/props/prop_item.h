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

#pragma once

#include "../config.h"

#include "qtvariantproperty.h"

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVector>
#include <QMap>

namespace General
{
	class CPropItem
	{
	public:
		enum EPropType
		{
			PT_GROUP,
			PT_BOOL,
			PT_INT,
			PT_DOUBLE,
			PT_STRING,
			PT_DATE,
			PT_POINT,
			PT_SIZE,
			PT_ENUM,
			PT_FONT,
			PT_COLOR
		};

		CPropItem();
		~CPropItem();

		int childCount() const;
		CPropItem* childAt(int index);
		void addChild(CPropItem* child);

		int handleCount() const;
		QString handleAt(int index);
		void addHandle(const QString& handle);

		QtVariantProperty* editorProperty();
		void setEditorProperty(QtVariantProperty* prop);

		bool isReadOnly() const;
		void setReadOnly(bool readonly);

		bool isExpanded() const;
		void setExpanded(bool expanded);

		QString name() const;
		void setName(const QString& name);

		EPropType type() const;
		void setType(EPropType type);
		void setType(const QString& type);

		QString caption() const;
		void setCaption(const QString& caption);

		qreal minimum() const;
		void setMinimum(qreal value);

		qreal maximum() const;
		void setMaximum(qreal value);

		qreal step() const;
		void setStep(qreal value);

		int decimals() const;
		void setDecimals(int value);

		QStringList enumNames() const;
		void setEnumNames(const QStringList& names);
		void setEnumNames(const QString& names);

		QVariant initValue() const;
		void setInitValue(const QVariant& value);

	private:
		QMap<QString, EPropType> m_type_names;

		QVector<CPropItem*> m_childs;

		QVector<QString> m_handles;

		QtVariantProperty *m_prop;
		bool m_readonly;
		bool m_expanded;
		QString m_name;
		EPropType m_type;
		QString m_caption;
		qreal m_min, m_max, m_step;
		int m_decimals;
		QStringList m_enum_names;
		QVariant m_init_value;

	};
}
