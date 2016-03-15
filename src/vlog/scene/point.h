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

#include "../object.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QtXml/QtXml>

namespace General
{
	// Опорная точка кривой
	class CPoint
		: public CObject, public QGraphicsEllipseItem
	{
		Q_OBJECT

	public:
		CPoint(QGraphicsItem* parent = 0);
		CPoint(const QPointF& ps, qreal rd, QGraphicsItem* parent = 0);
		virtual ~CPoint();

		void init(const QPointF& ps, qreal rd);

		virtual void activate();
		virtual void deactivate();

		virtual void lock();
		virtual void unlock();

		virtual QDomElement serializeProps(QDomDocument& document) const;

		QDomElement serialize(QDomDocument& document) const;
		void deserialize(const QDomElement& element);

	signals:
		void signalActivated();
		void signalDeactivated();
		void signalDeleted(CObject* ob);
		QPointF signalMoved(CPoint* pt, const QPointF& ps);

		void signalActivate(CPoint* pt);
		bool signalDelete(CPoint* pt);
		QPointF signalMove(CPoint* pt, const QPointF& ps);

		void signalLeftButtonPress(CPoint* pt);
		void signalRightButtonPress(CPoint* pt);

		void signalLeftButtonRelease(CPoint* pt);
		void signalRightButtonRelease(CPoint* pt);

		void signalLeftDoubleClick(CPoint* pt);
		void signalRightDoubleClick(CPoint* pt);

	protected:
		virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

		virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

	private:
		qreal m_radius;

	};
}
