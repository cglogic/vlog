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

#include "object.h"

namespace General
{
	CObject::CObject()
		: m_name(tr("ObjectName")), m_caption(""), m_parent(0), m_icon(QIcon(":/icons/blank.png")), m_active(false), m_locked(false),
		  m_properties_load(false)
	{
		m_pen = QPen(QColor(0, 0, 160, 127), 2);
		m_brush = QBrush(QColor(191, 191, 191, 127));
	}

	CObject::~CObject()
	{
	}

	QString CObject::name() const
	{
		return m_name;
	}

	void CObject::setName(const QString& name)
	{
		m_name = name;
	}

	QString CObject::caption() const
	{
		return m_caption;
	}

	void CObject::setCaption(const QString& caption)
	{
		m_caption = caption;

		emit signalCaptionChanged(this);
	}

	CObject* CObject::fantomParent() const
	{
		return m_parent;
	}

	void CObject::setFantomParent(CObject* parent)
	{
		m_parent = parent;
	}

	QIcon CObject::icon() const
	{
		return m_icon;
	}

	void CObject::setIcon(const QIcon& icon)
	{
		m_icon = icon;
	}

	void CObject::activate()
	{
		m_pen = QPen(QColor(160, 0, 0, 127), 2);

		m_active = true;
	}

	void CObject::deactivate()
	{
		m_pen = QPen(QColor(0, 0, 160, 127), 2);

		m_active = false;
	}

	bool CObject::isActivated() const
	{
		return m_active;
	}

	void CObject::lock()
	{
		m_locked = true;
	}

	void CObject::unlock()
	{
		m_locked = false;
	}

	bool CObject::isLocked() const
	{
		return m_locked;
	}

	QVariant CObject::propertyValue(const QString& name) const
	{
		t_shared_props_const_iter iter = m_shared_props.find(name);

		if (iter != m_shared_props.end())
		{
			return iter.value();
		}

		return QVariant();
	}

	void CObject::setPropertyValue(const QString& name, const QVariant& value)
	{
		m_shared_props[name] = value;
	}

	bool CObject::isRoot() const
	{
		return false;
	}

	QDomElement CObject::serializeProps(QDomDocument& document) const
	{
		QDomElement props = document.createElement("properties");

		return props;
	}

	void CObject::deserializeProps(const QDomElement& element)
	{
		QDomNodeList nodes = element.elementsByTagName("properties");

		if (nodes.count())
		{
			QDomElement prop_elem = nodes.at(0).toElement();
			QDomElement props = prop_elem.firstChildElement("property");

			while (!props.isNull())
			{
				QString name = props.attribute("name");
				QString value = props.attribute("value");

				setPropertyValue(name, value);

				props = props.nextSiblingElement("property");
			}
		}

		setPropertiesLoad(true);

		emit signalPropertiesUpdated(this);
	}

	QDomElement CObject::serializeSharedProps(QDomDocument& document) const
	{
		QDomElement prop_elem = document.createElement("shared_properties");

		for (t_shared_props_const_iter iter = m_shared_props.begin(); iter != m_shared_props.end(); ++iter)
		{
			QDomElement iter_elem = document.createElement("property");
			iter_elem.setAttribute("name", iter.key());
			iter_elem.setAttribute("value", iter.value().toString());
			prop_elem.appendChild(iter_elem);
		}

		return prop_elem;
	}

	void CObject::deserializeSharedProps(const QDomElement& element)
	{
		m_shared_props.clear();

		QDomNodeList nodes = element.elementsByTagName("shared_properties");

		if (nodes.count())
		{
			QDomElement prop_elem = nodes.at(0).toElement();
			QDomElement props = prop_elem.firstChildElement("property");

			while (!props.isNull())
			{
				QString name = props.attribute("name");
				QString value = props.attribute("value");

				CObject::setPropertyValue(name, value);

				props = props.nextSiblingElement("property");
			}
		}

		setPropertiesLoad(true);

		emit signalPropertiesUpdated(this);
	}

	bool CObject::isPropertiesLoaded() const
	{
		return m_properties_load;
	}

	void CObject::setPropertiesLoad(bool load)
	{
		m_properties_load = load;
	}
}
