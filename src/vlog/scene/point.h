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
