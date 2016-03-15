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

#include "config.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QList>
#include <QMap>
#include <QVariant>

#include <QPen>
#include <QBrush>
#include <QIcon>

#include <QtXml/QtXml>

namespace General
{
	class CObject
		: public QObject
	{
		Q_OBJECT

	public:
		CObject();
		virtual ~CObject();

		QString name() const;
		void setName(const QString& name);

		QString caption() const;
		void setCaption(const QString& caption);

		CObject* fantomParent() const;
		void setFantomParent(CObject* parent);

		QIcon icon() const;
		void setIcon(const QIcon& icon);

		virtual void activate();
		virtual void deactivate();
		bool isActivated() const;

		virtual void lock();
		virtual void unlock();
		bool isLocked() const;

		virtual QVariant propertyValue(const QString& name) const;
		virtual void setPropertyValue(const QString& name, const QVariant& value);

		virtual bool isRoot() const;

		virtual QDomElement serializeProps(QDomDocument& document) const;
		void deserializeProps(const QDomElement& element);

		QDomElement serializeSharedProps(QDomDocument& document) const;
		void deserializeSharedProps(const QDomElement& element);

		bool isPropertiesLoaded() const;
		void setPropertiesLoad(bool load);

	signals:
		void signalActivated(CObject* object);
		void signalPropertiesUpdated(CObject* object);
		void signalPropAttrUpdate(CObject* object, const QString& prop, const QString& attrib, const QVariant& value);
		void signalCaptionChanged(CObject* object);

	protected:
		QPen m_pen;
		QBrush m_brush;

	private:
		QString m_name;
		QString m_caption;
		CObject* m_parent;
		QIcon m_icon;

		bool m_active;
		bool m_locked;

		typedef QMap<QString, QVariant> t_shared_props;
		typedef t_shared_props::iterator t_shared_props_iter;
		typedef t_shared_props::const_iterator t_shared_props_const_iter;

		t_shared_props m_shared_props;

		bool m_properties_load;

	};
}
