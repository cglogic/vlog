#include "point.h"

namespace General
{
	CPoint::CPoint(QGraphicsItem* parent)
		: QGraphicsEllipseItem(parent)
	{
	}

	CPoint::CPoint(const QPointF& ps, qreal rd, QGraphicsItem* parent)
		: QGraphicsEllipseItem(parent)
	{
		init(ps, rd);
	}

	CPoint::~CPoint()
	{
		emit signalDeleted(this);
	}

	void CPoint::init(const QPointF& ps, qreal rd)
	{
		m_radius = rd;

		setPos(ps);
		setRect(-m_radius, -m_radius, 2.0 * m_radius, 2.0 * m_radius);

		setFlag(ItemIsMovable, true);
		setFlag(ItemSendsScenePositionChanges, true);

		setPen(m_pen);
		setBrush(m_brush);
		setZValue(50.0);
	}

	//

	void CPoint::activate()
	{
		if (!isActivated())
		{
			CObject::activate();

			setPen(QPen(QColor(0, 160, 0, 127), 2));

			emit signalActivated();
		}
	}

	void CPoint::deactivate()
	{
		if (isActivated())
		{
			CObject::deactivate();

			setPen(m_pen);

			emit signalDeactivated();
		}
	}

	void CPoint::lock()
	{
		CObject::lock();
	}

	void CPoint::unlock()
	{
		CObject::unlock();
	}

	//

	QVariant CPoint::itemChange(GraphicsItemChange change, const QVariant& value)
	{
		if (change == ItemPositionChange && scene())
		{
			return emit signalMove(this, value.toPointF());
		}
		else if (change == ItemPositionHasChanged && scene())
		{
			return emit signalMoved(this, value.toPointF());
		}

		return QGraphicsItem::itemChange(change, value);
	}

	void CPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
	{
		QGraphicsItem::mouseMoveEvent(event);
	}

	void CPoint::mousePressEvent(QGraphicsSceneMouseEvent* event)
	{
		bool pass = false;

		if (event->button() == Qt::LeftButton)
		{
			activate();

			emit signalLeftButtonPress(this);
		}

		if (event->button() == Qt::RightButton)
		{
			emit signalRightButtonPress(this);

			pass = !emit signalDelete(this);
		}

		if (pass)
		{
			QGraphicsItem::mousePressEvent(event);
		}
	}

	void CPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton)
		{
			deactivate();

			emit signalLeftButtonRelease(this);
		}

		if (event->button() == Qt::RightButton)
		{
			emit signalRightButtonRelease(this);
		}

		QGraphicsItem::mouseReleaseEvent(event);
	}

	void CPoint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
	{
		// may be deleted...
		QGraphicsItem::mouseDoubleClickEvent(event);

		if (event->button() == Qt::LeftButton)
		{
			emit signalLeftDoubleClick(this);
		}

		if (event->button() == Qt::RightButton)
		{
			emit signalRightDoubleClick(this);
		}
	}

	QDomElement CPoint::serializeProps(QDomDocument& document) const
	{
		QDomElement props = document.createElement("properties");

		return props;
	}

	QDomElement CPoint::serialize(QDomDocument& document) const
	{
		QDomElement elem = document.createElement("point");

		// element
		QDomElement elem_pos = document.createElement("pos");
		elem_pos.setAttribute("x", pos().x());
		elem_pos.setAttribute("y", pos().y());
		elem.appendChild(elem_pos);

		// properties
		QDomElement props = serializeProps(document);
		elem.appendChild(props);

		// shared properties
		QDomElement shared_props = serializeSharedProps(document);
		elem.appendChild(shared_props);

		return elem;
	}

	void CPoint::deserialize(const QDomElement& element)
	{
		QDomNodeList nodes = element.elementsByTagName("point");

		if (nodes.count())
		{
			QDomElement point_elem = nodes.at(0).toElement();

			deserializeSharedProps(point_elem);

			deserializeProps(point_elem);

			// element
			nodes = point_elem.elementsByTagName("pos");
			if (nodes.count())
			{
				QDomElement elem_pos = nodes.at(0).toElement();
			}
		}
	}
}
