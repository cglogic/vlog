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
#include "point.h"

#include <QRectF>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QtXml/QtXml>

namespace General
{
	class CPoint;

	// Шкала
	class CRange
		: public CObject, public QAbstractGraphicsShapeItem
	{
		Q_OBJECT

	public:
		enum EDirection
		{
			DirectionVertical,
			DirectionHorizontal
		};

		CRange(QGraphicsScene* sc);
		CRange(EDirection dr, const QPointF& psf, const QPointF& psl, QGraphicsScene* sc);
		virtual ~CRange();

		void init(EDirection dr, const QPointF& psf, const QPointF& psl);

		void show();
		void hide();

		virtual void activate();
		virtual void deactivate();

		virtual void lock();
		virtual void unlock();

		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
		virtual QRectF boundingRect() const;

		QLineF sceneLine();

		void setCaption(const QString& caption);
		void setTextVisible(bool sh);

		bool freeAngle() const;
		void setFreeAngle(bool fa);

		EDirection direction() const;
		bool setDirection(EDirection dr);

		virtual QVariant propertyValue(const QString& name) const;
		virtual void setPropertyValue(const QString& name, const QVariant& value);

		virtual QDomElement serializeProps(QDomDocument& document) const;

		QDomElement serialize(QDomDocument& document) const;
		void deserialize(const QDomElement& element);

		qreal percentByValue(qreal value, qreal min, qreal max) const;
		qreal valueByPercent(qreal percent, qreal min, qreal max) const;

		qreal valueByCoord(qreal coord, qreal start, qreal stop) const;
		qreal coordByValue(qreal value, qreal start, qreal stop) const;

		qreal valueByCoord(qreal coord) const;
		qreal coordByValue(qreal value) const;

		QPointF firstPointPos() const;
		QPointF secondPointPos() const;

	signals:
		void signalActivated();
		void signalDeactivated();
		void signalDeleted(CObject* ob);

		void signalActivate(CObject* ob);
		bool signalDelete(CRange* rn);

		QPointF signalMove(CRange* rn, const QPointF& ps);

		void signalLeftButtonPress(CRange* rn);
		void signalRightButtonPress(CRange* rn);

		void signalLeftButtonRelease(CRange* rn);
		void signalRightButtonRelease(CRange* rn);

	public slots:
		QPointF slotFirstPointMove(CPoint*, const QPointF& ps);
		QPointF slotSecondPointMove(CPoint*, const QPointF& ps);

		QPointF slotFirstPointMoved(CPoint*, const QPointF& ps);
		QPointF slotSecondPointMoved(CPoint*, const QPointF& ps);

		bool slotRemove(CPoint *pt);

	protected:
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

		void updateBoundingRect();

	private:
		QGraphicsScene *m_scene;
		QRectF m_rect;

		bool m_free_angle;
		EDirection m_direction;

		QGraphicsLineItem *m_line;

		CPoint *m_point_first;
		CPoint *m_point_second;

		QGraphicsSimpleTextItem *m_caption;

		//
		QPainterPath m_path;
		QPainterPath m_path_text;

		void calculate();

		bool m_show_text;

		//
		double m_start;
		double m_stop;
		int m_divide;

		//
		int m_digit_delim;
		int m_digit_prec;
		double m_digit_step;
		double m_digit_null;

	};
}
