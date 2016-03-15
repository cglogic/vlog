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

#include "range.h"

#include <qmath.h>

namespace General
{
	CRange::CRange(QGraphicsScene* sc)
		: QAbstractGraphicsShapeItem(), m_scene(sc), m_free_angle(false), m_direction(DirectionHorizontal),
		  m_show_text(true), m_start(0.0), m_stop(100.0), m_divide(1), m_digit_delim(1), m_digit_prec(3),
		  m_digit_step(0.01), m_digit_null(-999.999)
	{
	}

	CRange::CRange(EDirection dr, const QPointF& psf, const QPointF& psl, QGraphicsScene* sc)
		: QAbstractGraphicsShapeItem(), m_scene(sc), m_free_angle(false), m_direction(DirectionHorizontal),
		  m_show_text(true), m_start(0.0), m_stop(100.0), m_divide(1), m_digit_delim(1), m_digit_prec(3),
		  m_digit_step(0.01), m_digit_null(-999.999)
	{
		init(dr, psf, psl);
	}

	CRange::~CRange()
	{
		emit signalDeleted(this);

		m_scene->removeItem(m_point_first);

		delete m_point_first;
		m_point_first = 0;

		m_scene->removeItem(m_point_second);

		delete m_point_second;
		m_point_second = 0;
	}

	void CRange::init(EDirection dr, const QPointF& psf, const QPointF& psl)
	{
		setName("Depth");
		setIcon(QIcon(":/icons/chart_down_color.png"));
		CObject::setCaption(tr("Depth"));
		setPos(psf);
		setDirection(dr);

		QPointF ps = mapFromScene(psl);

		ps.setY(0.0);

		if (ps.x() == 0.0)
		{
			ps.setX(0.1);
		}

		m_line = new QGraphicsLineItem(this);
		m_line->setPos(QPointF(0.0, 0.0));
		m_line->setLine(QLineF(0.0, 0.0, ps.x(), ps.y()));
		m_line->setPen(m_pen);
		m_line->setZValue(20.0);

		m_point_first = new CPoint(psf, 5.0);
		m_scene->addItem(m_point_first);

		m_point_second = new CPoint(psl, 5.0);
		m_scene->addItem(m_point_second);

		m_caption = new QGraphicsSimpleTextItem(this);
		m_caption->setPen(QPen(QColor(0, 0, 160, 0)));
		m_caption->setBrush(QBrush(QColor(0, 0, 160, 150), Qt::SolidPattern));
		QFont fnt;
		fnt.setPixelSize(22);
		m_caption->setFont(fnt);

		calculate();
		updateBoundingRect();
		update();

		connect(m_point_first, SIGNAL(signalMove(CPoint*, const QPointF&)), this, SLOT(slotFirstPointMove(CPoint*, const QPointF&)));
		connect(m_point_second, SIGNAL(signalMove(CPoint*, const QPointF&)), this, SLOT(slotSecondPointMove(CPoint*, const QPointF&)));

		connect(m_point_first, SIGNAL(signalMoved(CPoint*, const QPointF&)), this, SLOT(slotFirstPointMoved(CPoint*, const QPointF&)));
		connect(m_point_second, SIGNAL(signalMoved(CPoint*, const QPointF&)), this, SLOT(slotSecondPointMoved(CPoint*, const QPointF&)));

		connect(m_point_first, SIGNAL(signalDelete(CPoint*)), this, SLOT(slotRemove(CPoint*)));
		connect(m_point_second, SIGNAL(signalDelete(CPoint*)), this, SLOT(slotRemove(CPoint*)));
	}

	//

	void CRange::show()
	{
		m_point_first->show();
		m_point_second->show();

		QGraphicsItem::show();
	}

	void CRange::hide()
	{
		m_point_first->hide();
		m_point_second->hide();

		QGraphicsItem::hide();
	}

	void CRange::activate()
	{
		if (!isActivated())
		{
			CObject::activate();

			m_line->setPen(m_pen);

			if (!isLocked())
			{
				m_point_first->show();
				m_point_second->show();
			}

			update();

			emit signalActivated();
		}
	}

	void CRange::deactivate()
	{
		if (isActivated())
		{
			CObject::deactivate();

			m_line->setPen(m_pen);

			if (!isLocked())
			{
				m_point_first->hide();
				m_point_second->hide();
			}

			update();

			emit signalDeactivated();
		}
	}

	void CRange::lock()
	{
		CObject::lock();

		if (isActivated())
		{
			m_point_first->hide();
			m_point_second->hide();
		}
	}

	void CRange::unlock()
	{
		CObject::unlock();

		if (isActivated())
		{
			m_point_first->show();
			m_point_second->show();
		}
	}

	void CRange::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
	{
		Q_UNUSED(option);
		Q_UNUSED(widget);

		painter->setRenderHint(QPainter::Antialiasing);
		painter->setRenderHint(QPainter::TextAntialiasing);
		painter->setRenderHint(QPainter::HighQualityAntialiasing);

		painter->setPen(m_pen);

		painter->drawPath(m_path);

		if (m_show_text)
		{
			painter->setPen(QPen(QColor(0, 0, 160, 0)));
			painter->setBrush(QBrush(QColor(0, 0, 160, 150), Qt::SolidPattern));

			painter->drawPath(m_path_text);
		}
	}

	QRectF CRange::boundingRect() const
	{
		return m_rect;
	}

	QLineF CRange::sceneLine()
	{
		return QLineF(mapToScene(m_line->line().p1()), mapToScene(m_line->line().p2()));
	}

	void CRange::setCaption(const QString& caption)
	{
		m_caption->setText(caption);

		if (m_caption->text() != QString())
		{
			m_caption->setPos(((m_rect.width() + (m_rect.x() * 2.0)) / 2.0) -
							  ((m_caption->boundingRect().width() + m_caption->boundingRect().x()) / 2.0),
							  -(m_caption->boundingRect().height() + m_caption->boundingRect().y())
							  -((m_caption->boundingRect().height() + m_caption->boundingRect().y()) / 5.0));

			m_caption->show();
		}
		else
		{
			m_caption->hide();
		}

		updateBoundingRect();
		update();
	}

	void CRange::setTextVisible(bool sh)
	{
		m_show_text = sh;

		calculate();
		update();
		updateBoundingRect();
		update();
	}

	bool CRange::freeAngle() const
	{
		return m_free_angle;
	}

	void CRange::setFreeAngle(bool fa)
	{
		m_free_angle = fa;
	}

	CRange::EDirection CRange::direction() const
	{
		return m_direction;
	}

	bool CRange::setDirection(EDirection dr)
	{
		if (m_direction != dr)
		{
			if (dr == DirectionHorizontal)
			{
				setRotation(0.0);
			}
			else
			{
				setRotation(-90.0);
			}

			m_direction = dr;

			return true;
		}

		return false;
	}

	//

	QVariant CRange::propertyValue(const QString& name) const
	{
		if (name == "start")
		{
			return m_start;
		}
		else if (name == "stop")
		{
			return m_stop;
		}
		else if (name == "divide")
		{
			return m_divide;
		}
		else if (name == "digit_start")
		{
			return m_start;
		}
		else if (name == "digit_stop")
		{
			return m_stop;
		}
		else if (name == "digit_delim")
		{
			return m_digit_delim;
		}
		else if (name == "digit_prec")
		{
			return m_digit_prec;
		}
		else if (name == "digit_step")
		{
			return m_digit_step;
		}
		else if (name == "digit_null")
		{
			return m_digit_null;
		}

		return CObject::propertyValue(name);
	}

	void CRange::setPropertyValue(const QString& name, const QVariant& value)
	{
		if (name == "start")
		{
			m_start = value.toDouble();

			if (m_start > m_stop)
			{
				m_stop = m_start;
			}

			calculate();
			update();
			updateBoundingRect();
			update();

			emit signalPropertiesUpdated(this);
		}
		else if (name == "stop")
		{
			m_stop = value.toDouble();

			if (m_stop < m_start)
			{
				m_start = m_stop;
			}

			calculate();
			update();
			updateBoundingRect();
			update();

			emit signalPropertiesUpdated(this);
		}
		else if (name == "divide")
		{
			m_divide = value.toDouble();

			calculate();
			update();
			updateBoundingRect();
			update();
		}
		else if (name == "digit_start")
		{
		}
		else if (name == "digit_stop")
		{
		}
		else if (name == "digit_delim")
		{
			m_digit_delim = value.toInt();
		}
		else if (name == "digit_prec")
		{
			m_digit_prec = value.toInt();

			emit signalPropAttrUpdate(this, "digit_start", "decimals", m_digit_prec);
			emit signalPropAttrUpdate(this, "digit_stop", "decimals", m_digit_prec);
			emit signalPropAttrUpdate(this, "digit_step", "decimals", m_digit_prec);
			emit signalPropAttrUpdate(this, "digit_null", "decimals", m_digit_prec);
		}
		else if (name == "digit_step")
		{
			m_digit_step = value.toDouble();
		}
		else if (name == "digit_null")
		{
			m_digit_null = value.toDouble();
		}
		else
		{
			CObject::setPropertyValue(name, value);
		}
	}

	//

	qreal CRange::percentByValue(qreal value, qreal min, qreal max) const
	{
		qreal percent = 0.0;

		qreal pmin = qMin(min, max);
		qreal pmax = qMax(min, max);

		if (value > pmin && value < pmax)
		{
			qreal nvl = value - pmin;
			qreal nmn = pmin - pmin;
			qreal nmx = pmax - pmin;

			qreal op = (nmx - nmn) / 100.0;

			percent = nvl / op;

			if (min > max)
			{
				percent = 100.0 - percent;
			}
		}
		else if (value <= min)
		{
			percent = 0.0;
		}
		else if (value >= max)
		{
			percent = 100.0;
		}

		return percent;
	}

	qreal CRange::valueByPercent(qreal percent, qreal min, qreal max) const
	{
		qreal value = 0.0;

		qreal pmin = qMin(min, max);
		qreal pmax = qMax(min, max);

		if (percent > 0.0 && percent < 100.0)
		{
			qreal nmn = pmin - pmin;
			qreal nmx = pmax - pmin;

			qreal op = (nmx - nmn) / 100.0;

			value = percent * op;

			if (min < max)
			{
				value = pmin + value;
			}

			if (min > max)
			{
				value = pmax - value;
			}
		}
		else if (percent <= 0.0)
		{
			value = min;
		}
		else if (percent >= 100.0)
		{
			value = max;
		}

		return value;
	}

	qreal CRange::valueByCoord(qreal coord, qreal start, qreal stop) const
	{
		qreal value = 0.0;

		qreal cf = m_direction == DirectionHorizontal ? m_point_first->scenePos().x() : m_point_first->scenePos().y();
		qreal cs = m_direction == DirectionHorizontal ? m_point_second->scenePos().x() : m_point_second->scenePos().y();

		qreal min = qMin(cf, cs);
		qreal max = qMax(cf, cs);

		if (coord > min && coord < max)
		{
			qreal pc = percentByValue(coord, cf, cs);

			value = valueByPercent(pc, start, stop);
		}
		else if (coord <= min)
		{
			value = cf < cs ? start : stop;
		}
		else if (coord >= max)
		{
			value = cf < cs ? stop : start;
		}

		return value;
	}

	qreal CRange::valueByCoord(qreal coord) const
	{
		return valueByCoord(coord, m_start, m_stop);
	}

	qreal CRange::coordByValue(qreal value, qreal start, qreal stop) const
	{
		qreal coord = 0.0;

		qreal cf = m_direction == DirectionHorizontal ? m_point_first->scenePos().x() : m_point_first->scenePos().y();
		qreal cs = m_direction == DirectionHorizontal ? m_point_second->scenePos().x() : m_point_second->scenePos().y();

		if (value > start && value < stop)
		{
			qreal pv = percentByValue(value, start, stop);

			coord = valueByPercent(pv, cf, cs);
		}
		else if (value <= start)
		{
			coord = cf;
		}
		else if (value >= stop)
		{
			coord = cs;
		}

		return coord;
	}

	qreal CRange::coordByValue(qreal value) const
	{
		return coordByValue(value, m_start, m_stop);
	}

	//

	QPointF CRange::firstPointPos() const
	{
		return m_point_first->pos();
	}

	QPointF CRange::secondPointPos() const
	{
		return m_point_second->pos();
	}

	//

	QPointF CRange::slotFirstPointMove(CPoint* pt, const QPointF& ps)
	{
		Q_UNUSED(pt);

		return ps;
	}

	QPointF CRange::slotSecondPointMove(CPoint* pt, const QPointF& ps)
	{
		Q_UNUSED(pt);

		// Rotate
		if (m_free_angle)
		{
			QPointF p1(QPointF(1.0, 0.0));
			QPointF p2(ps - m_point_first->pos() + QPointF(1.0, 0.0));

			qreal angle = (180.0 / M_PI) * qAcos(((p1.x() * p2.x() + p1.y() * p2.y()) / (qSqrt(p1.x() * p1.x() + p1.y() * p1.y()) * qSqrt(p2.x() * p2.x() + p2.y() * p2.y()))));
			qreal correct = 15.0;

			if (angle < 45.0 - correct || angle > 135.0 + correct)
			{
				setDirection(DirectionHorizontal);
			}

			if (angle > 45.0 + correct && angle < 135.0 - correct)
			{
				setDirection(DirectionVertical);
			}
		}

		// Lock size
		QPointF p = mapFromScene(ps);

		p.setY(m_line->line().p2().y());

		if (p.x() >= 0.0 && p.x() < 0.1)
		{
			p.setX(0.1);
		}

		if (p.x() <= 0.0 && p.x() > -0.1)
		{
			p.setX(-0.1);
		}

		return mapToScene(p);
	}

	QPointF CRange::slotFirstPointMoved(CPoint* pt, const QPointF& ps)
	{
		Q_UNUSED(pt);

		setPos(ps);

		m_point_second->setPos(mapToScene(m_line->line().p2()));

		//updatePath();

		emit signalMove(this, ps);

		update();

		return ps;
	}

	QPointF CRange::slotSecondPointMoved(CPoint* pt, const QPointF& ps)
	{
		Q_UNUSED(pt);

		m_line->setLine(QLineF(m_line->line().p1(), mapFromScene(ps)));

		calculate();

		emit signalMove(this, ps);

		emit signalPropertiesUpdated(this);

		update();

		setCaption(m_caption->text());

		return ps;
	}

	bool CRange::slotRemove(CPoint* pt)
	{
		Q_UNUSED(pt);

		if (isActivated() && !isLocked())
		{
			emit signalDelete(this);
		}

		return true;
	}

	//

	void CRange::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
	{
		QGraphicsItem::mouseMoveEvent(event);
	}

	void CRange::mousePressEvent(QGraphicsSceneMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton)
		{
			emit signalLeftButtonPress(this);

			if (!isActivated())
			{
				emit signalActivate(this);
			}
		}

		if (event->button() == Qt::RightButton)
		{
			emit signalRightButtonPress(this);

			slotRemove(0);
		}
	}

	void CRange::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton)
		{
			emit signalLeftButtonRelease(this);
		}

		if (event->button() == Qt::RightButton)
		{
			emit signalRightButtonRelease(this);
		}
	}

	//

	void CRange::updateBoundingRect()
	{
		prepareGeometryChange();

		double extend = 15.0;

		m_rect = QRectF(
			QPointF(qMin(m_line->line().p1().x(), m_line->line().p2().x()) - extend,
					qMin(m_line->line().p1().y(), m_line->line().p2().y()) - extend),
			QPointF(qMax(m_line->line().p1().x(), m_line->line().p2().x()) + extend,
					qMax(m_line->line().p1().y(), m_line->line().p2().y()) + extend));

		//m_rect = m_rect.united(m_path.boundingRect());

		if (m_show_text)
		{
			m_rect = m_rect.united(m_path_text.boundingRect());
		}
	}

	void CRange::calculate()
	{
		QPointF p1 = m_line->line().p1().x() < m_line->line().p2().x() ? m_line->line().p1() : m_line->line().p2();
		QPointF p2 = m_line->line().p1().x() > m_line->line().p2().x() ? m_line->line().p1() : m_line->line().p2();

		qreal step = (p2.x() - p1.x()) / m_divide;

		QPainterPath path;

		path.moveTo(m_line->line().p1().x(), m_line->line().p1().y() - 10.0);
		path.lineTo(m_line->line().p1().x(), m_line->line().p1().y() + 10.0);

		for (qreal i = p1.x() + step; i < p2.x() - (step / 2.0); i += step)
		{
			path.moveTo(i, p1.y() - 5.0);
			path.lineTo(i, p2.y() + 5.0);
		}

		path.moveTo(m_line->line().p2().x(), m_line->line().p2().y() - 10.0);
		path.lineTo(m_line->line().p2().x(), m_line->line().p2().y() + 10.0);

		m_path.swap(path);

		if (m_show_text)
		{
			double start = m_start;
			double stop = m_stop;
			double value_step = qAbs(m_stop - m_start) / m_divide;

			if (m_line->line().p1().x() > m_line->line().p2().x())
			{
				start = m_stop;
				stop = m_start;
				value_step = -qAbs(m_stop - m_start) / m_divide;
			}

			QPainterPath path_text;

			path_text.addText(p1.x() + 4.5, p1.y() + 15.0, QFont(), QString().setNum((int)start));

			for (qreal i = p1.x() + step, t = start + value_step; i < p2.x() - (step / 2.0); i += step, t += value_step)
			{
				path_text.addText(i + 4.5, p2.y() + 15.0, QFont(), QString().setNum((int)t));
			}

			path_text.addText(p2.x() + 4.5, p2.y() + 15.0, QFont(), QString().setNum((int)stop));

			m_path_text.swap(path_text);
		}

		//updateBoundingRect();
	}

	//

	QDomElement CRange::serializeProps(QDomDocument& document) const
	{
		QDomElement props = document.createElement("properties");

		// property
		QDomElement prop_start = document.createElement("property");
		prop_start.setAttribute("name", "start");
		prop_start.setAttribute("value", m_start);
		props.appendChild(prop_start);

		// property
		QDomElement prop_stop = document.createElement("property");
		prop_stop.setAttribute("name", "stop");
		prop_stop.setAttribute("value", m_stop);
		props.appendChild(prop_stop);

		// property
		QDomElement prop_divide = document.createElement("property");
		prop_divide.setAttribute("name", "divide");
		prop_divide.setAttribute("value", m_divide);
		props.appendChild(prop_divide);

		// property
		QDomElement prop_digit_delim = document.createElement("property");
		prop_digit_delim.setAttribute("name", "digit_delim");
		prop_digit_delim.setAttribute("value", m_digit_delim);
		props.appendChild(prop_digit_delim);

		// property
		QDomElement prop_digit_prec = document.createElement("property");
		prop_digit_prec.setAttribute("name", "digit_prec");
		prop_digit_prec.setAttribute("value", m_digit_prec);
		props.appendChild(prop_digit_prec);

		// property
		QDomElement prop_digit_step = document.createElement("property");
		prop_digit_step.setAttribute("name", "digit_step");
		prop_digit_step.setAttribute("value", m_digit_step);
		props.appendChild(prop_digit_step);

		// property
		QDomElement prop_digit_null = document.createElement("property");
		prop_digit_null.setAttribute("name", "digit_null");
		prop_digit_null.setAttribute("value", m_digit_null);
		props.appendChild(prop_digit_null);

		return props;
	}

	QDomElement CRange::serialize(QDomDocument& document) const
	{
		QDomElement elem = document.createElement("depth");

		// element
		QDomElement elem_dir = document.createElement("direction");
		elem_dir.setAttribute("value", QString::number(m_direction));
		elem.appendChild(elem_dir);

		// element
		QDomElement elem_p0 = document.createElement("point0");
		elem_p0.setAttribute("x", firstPointPos().x());
		elem_p0.setAttribute("y", firstPointPos().y());
		elem.appendChild(elem_p0);

		// element
		QDomElement elem_p1 = document.createElement("point1");
		elem_p1.setAttribute("x", secondPointPos().x());
		elem_p1.setAttribute("y", secondPointPos().y());
		elem.appendChild(elem_p1);

		// properties
		QDomElement props = serializeProps(document);
		elem.appendChild(props);

		// shared properties
		QDomElement shared_props = serializeSharedProps(document);
		elem.appendChild(shared_props);

		return elem;
	}

	void CRange::deserialize(const QDomElement& element)
	{
		QDomNodeList nodes = element.elementsByTagName("depth");

		if (nodes.count())
		{
			QDomElement depth_elem = nodes.at(0).toElement();

			deserializeSharedProps(depth_elem);

			deserializeProps(depth_elem);
		}
	}
}
