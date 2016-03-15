#include "parameter.h"

namespace General
{
	CParameter::CParameter(CRange* dp, QGraphicsScene* sc)
		: QGraphicsRectItem(), m_scene(sc), m_depth(dp), m_unit("M")
	{
	}

	CParameter::CParameter(const QPointF& psf, const QPointF& psl, CRange* dp, QGraphicsScene* sc)
		: QGraphicsRectItem(), m_scene(sc), m_depth(dp), m_unit("M")
	{
		init(psf, psl);
	}

	CParameter::~CParameter()
	{
		emit signalDeleted(this);

		m_scene->removeItem(m_range);

		delete m_range;
		m_range = 0;
	}

	void CParameter::init(const QPointF& psf, const QPointF& psl)
	{
		setName("Parameter");
		setIcon(QIcon(":/icons/chart_curve.png"));

		setFlag(ItemClipsChildrenToShape, true);

		m_range = new CRange(m_depth->direction() == CRange::DirectionHorizontal ? CRange::DirectionVertical : CRange::DirectionHorizontal, psf, psl, m_scene);
		m_range->setTextVisible(false);

		m_scene->addItem(m_range);

		setZValue(30.0);

		slotRangeMove(m_range, m_range->scenePos());

		QPen pen = m_pen;
		pen.setStyle(Qt::DotLine);

		setPen(pen);

		connect(m_range, SIGNAL(signalActivate(CObject*)), this, SLOT(slotRangeActivate(CObject*)));
		connect(m_range, SIGNAL(signalActivated()), this, SLOT(slotRangeActivated()));
		connect(m_range, SIGNAL(signalDeactivated()), this, SLOT(slotRangeDeactivated()));
		connect(m_range, SIGNAL(signalDelete(CRange*)), this, SLOT(slotRangeDelete(CRange*)));
		connect(m_range, SIGNAL(signalMove(CRange*, const QPointF&)), this, SLOT(slotRangeMove(CRange*, const QPointF&)));
	}

	//

	void CParameter::activate()
	{
		if (!isActivated())
		{
			m_range->activate();

			CObject::activate();

			QPen pen = m_pen;
			pen.setStyle(Qt::DotLine);

			setPen(pen);

			show();

			emit signalActivated();
		}
	}

	void CParameter::deactivate()
	{
		if (isActivated())
		{
			m_range->deactivate();

			CObject::deactivate();

			QPen pen = m_pen;
			pen.setStyle(Qt::DotLine);

			setPen(pen);

			hide();

			emit signalDeactivated();
		}
	}

	void CParameter::lock()
	{
		CObject::lock();

		if (isActivated())
		{
			m_range->lock();

			for (t_curves_iter iter = m_curves.begin(); iter != m_curves.end(); ++iter)
			{
				(*iter)->hide();
			}
		}
	}

	void CParameter::unlock()
	{
		CObject::unlock();

		if (isActivated())
		{
			if (m_curves.size() <= 0)
			{
				m_range->unlock();
			}

			for (t_curves_iter iter = m_curves.begin(); iter != m_curves.end(); ++iter)
			{
				(*iter)->show();
			}
		}
	}

	//

	void CParameter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
	{
		painter->setRenderHint(QPainter::Antialiasing);

		QPen pen = m_pen;
		pen.setStyle(Qt::DotLine);

		painter->setPen(pen);

		QPainterPath path;

		for (t_curves_iter iter = m_curves.begin() + 1; iter != m_curves.end() && m_curves.size() > 1; ++iter)
		{
			path.moveTo(QPointF(mapFromItem((*(iter - 1)), (*(iter - 1))->lastPoint()->pos()).x(),
								mapFromItem((*(iter - 1)), (*(iter - 1))->lastPoint()->pos()).y()));
			path.lineTo(QPointF(mapFromItem((*(iter)), (*(iter))->firstPoint()->pos()).x(),
								mapFromItem((*(iter)), (*(iter))->firstPoint()->pos()).y()));
		}

		painter->drawPath(path);

		QGraphicsRectItem::paint(painter, option, widget);
	}

	CRange* CParameter::getRange()
	{
		return m_range;
	}

	CRange* CParameter::getDepth()
	{
		return m_depth;
	}

	//

	CCurve* CParameter::directAddCurve()
	{
		CCurve *cv = new CCurve(this, m_scene);
		cv->init(boundingRect().bottomLeft());
		cv->setFantomParent(this);

		connect(cv, SIGNAL(signalActivated()), this, SLOT(slotCurveActivated()));
		connect(cv, SIGNAL(signalDeactivated()), this, SLOT(slotCurveDeactivated()));

		connect(cv, SIGNAL(signalPointMove(CPoint*, const QPointF&)), this, SLOT(slotPointMove(CPoint*, const QPointF&)));
		connect(cv, SIGNAL(signalDelete(CCurve*)), this, SLOT(slotCurveDelete(CCurve*)));

		connect(cv, SIGNAL(signalPropertiesUpdated(CObject*)), this, SLOT(slotCurvePropertiesUpdated(CObject*)));

		connect(cv, SIGNAL(signalHoverEnter()), this, SLOT(slotCurveHoverEnter()));
		connect(cv, SIGNAL(signalHoverLeave()), this, SLOT(slotCurveHoverLeave()));
		connect(cv, SIGNAL(signalHoverMoved(CCurve*, const QPointF&)), this, SLOT(slotCurveHoverMoved(CCurve*, const QPointF&)));

		m_curves.append(cv);

		m_range->lock();

		return cv;
	}

	void CParameter::directRemoveCurve(CCurve* cv)
	{
		m_curves.removeOne(cv);
		m_scene->removeItem(cv);

		delete cv;
	}

	void CParameter::sort()
	{
		qSort(m_curves.begin(), m_curves.end(), [](const CCurve* c0, const CCurve* c1)
			{
				return CCurve::sortType() == CCurve::SortHorizontal ? c0->firstPoint()->pos().x() < c1->firstPoint()->pos().x() : c0->firstPoint()->pos().y() < c1->firstPoint()->pos().y();
			});
	}

	CCurve* CParameter::addCurve(const QPointF& ps)
	{
		CCurve *cv = directAddCurve();

		cv->addPoint(ps);
		cv->addPoint(ps + (CCurve::sortType() == CCurve::SortHorizontal ? QPointF(0.001, 0.0) : QPointF(0.0, 0.001)));

		sort();
		update();

		return cv;
	}

	void CParameter::removeCurve(CCurve *cv)
	{
		if (m_curves.size() > 0)
		{
			t_curves_const_iter iter = qFind(m_curves, cv);

			if (iter != m_curves.end())
			{
				directRemoveCurve(cv);

				update();

				if (m_curves.size() <= 0)
				{
					m_range->unlock();
				}
			}
		}
	}

	int CParameter::curveCount() const
	{
		return m_curves.size();
	}

	CCurve* CParameter::curveAt(int i)
	{
		return m_curves.at(i);
	}

	//

	QVariant CParameter::propertyValue(const QString& name) const
	{
		if (name == "name")
		{
			return m_name;
		}
		else if (name == "unit")
		{
			return m_unit;
		}
		else if (name == "range_divide")
		{
			return m_range->propertyValue("divide");
		}
		else if (name == "brush_color")
		{
			return m_brush.color();
		}

		return CObject::propertyValue(name);
	}

	void CParameter::setPropertyValue(const QString& name, const QVariant& value)
	{
		if (name == "name")
		{
			m_name = value.toString();

			setCaption(value.toString());

			m_range->setCaption(m_name);
		}
		else if (name == "unit")
		{
			m_unit = value.toString();
		}
		else if (name == "range_divide")
		{
			m_range->setPropertyValue("divide", value);
		}
		else if (name == "brush_color")
		{
			m_brush.setColor(value.value<QColor>());
		}
		else
		{
			CObject::setPropertyValue(name, value);
		}
	}

	//

	qreal CParameter::valueByDepth(qreal depth, qreal null)
	{
		qreal value = null;
		qreal coord = m_depth->coordByValue(depth);

		for (t_curves_iter iter = m_curves.begin(); iter != m_curves.end(); ++iter)
		{
			qreal first = m_depth->direction() == CRange::DirectionHorizontal
					? (*(iter))->firstPoint()->scenePos().x()
					: (*(iter))->firstPoint()->scenePos().y();
			qreal last = m_depth->direction() == CRange::DirectionHorizontal
					? (*(iter))->lastPoint()->scenePos().x()
					: (*(iter))->lastPoint()->scenePos().y();

			qreal min = qMin(first, last);
			qreal max = qMax(first, last);

			if (coord >= min && coord <= max)
			{
				qreal point_coord = (*(iter))->coordByCoord(coord);

				value = m_range->valueByCoord(point_coord,
								(*(iter))->propertyValue("range_start").toDouble(),
								(*(iter))->propertyValue("range_stop").toDouble());

				// !!! DEBUG !!!
				/*qreal x = m_depth->coordByValue(depth);
				qreal y = m_range->coordByValue(value, (*(iter))->propertyValue("range_start").toDouble(), (*(iter))->propertyValue("range_stop").toDouble());

				CPoint *point = new CPoint(QPointF(m_depth->direction() == CRange::DirectionHorizontal ? x : y, m_depth->direction() == CRange::DirectionHorizontal ? y : x), 0.05);
				m_scene->addItem(point);*/
				// !!! DEBUG !!!

				break;
			}
		}

		return value;
	}

	//

	void CParameter::slotRangeActivated()
	{
		activate();
	}

	void CParameter::slotRangeDeactivated()
	{
		deactivate();
	}

	void CParameter::slotRangeActivate(CObject* ob)
	{
		Q_UNUSED(ob);

		emit signalActivate(this);
	}

	bool CParameter::slotRangeDelete(CRange* rn)
	{
		Q_UNUSED(rn);

		return emit signalDelete(this);
	}

	void CParameter::slotRangeMove(CRange* rn, const QPointF& pt)
	{
		Q_UNUSED(rn);
		Q_UNUSED(pt);

		setPos(m_range->sceneLine().p1());

		QLineF line_h;
		QLineF line_v;

		if (m_depth->direction() == CRange::DirectionHorizontal)
		{
			line_h = m_depth->sceneLine();
			line_v = m_range->sceneLine();
		}
		else
		{
			line_h = m_range->sceneLine();
			line_v = m_depth->sceneLine();
		}

		setRect(QRectF(mapFromScene(QPointF(
				qMin(line_h.p1().x(), line_h.p2().x()),
				qMin(line_v.p1().y(), line_v.p2().y()))),
				mapFromScene(QPointF(
				qMax(line_h.p1().x(), line_h.p2().x()),
				qMax(line_v.p1().y(), line_v.p2().y())))));

		emit signalPropertiesUpdated(this);
	}

	void CParameter::slotCurveActivated()
	{
		//

		m_range->setTextVisible(true);
	}

	void CParameter::slotCurveDeactivated()
	{
		//

		m_range->setTextVisible(false);
	}

	bool CParameter::slotCurveDelete(CCurve* cv)
	{
		slotCurveDeactivated();

		removeCurve(cv);

		update();

		return true;
	}

	void CParameter::slotCurvePropertiesUpdated(CObject* object)
	{
		m_range->setPropertyValue("start", object->propertyValue("range_start"));
		m_range->setPropertyValue("stop", object->propertyValue("range_stop"));
	}

	QPointF CParameter::slotPointMove(CPoint *pt, const QPointF &ps)
	{
		QPointF nps = ps;
		QRectF rc = rect();

		t_curves_const_iter iter = qFind(m_curves, pt->parentItem());

		if (iter - 1 != m_curves.end())
		{
			if (iter != m_curves.begin())
			{
				if (m_depth->direction() == CRange::DirectionHorizontal)
				{
					rc.setLeft(mapFromItem(*(iter - 1), (*(iter - 1))->lastPoint()->pos()).x());
				}
				else
				{
					rc.setTop(mapFromItem(*(iter - 1), (*(iter - 1))->lastPoint()->pos()).y());
				}
			}

			if (iter + 1 != m_curves.end())
			{
				if (m_depth->direction() == CRange::DirectionHorizontal)
				{
					rc.setRight(mapFromItem(*(iter + 1), (*(iter + 1))->firstPoint()->pos()).x());
				}
				else
				{
					rc.setBottom(mapFromItem(*(iter + 1), (*(iter + 1))->firstPoint()->pos()).y());
				}
			}
		}

		if (!rc.contains(nps))
		{
			nps.setX(qMin(rc.right(), qMax(nps.x(), rc.left())));
			nps.setY(qMin(rc.bottom(), qMax(nps.y(), rc.top())));
		}

		update();

		emit signalPointMove(this);

		return nps;
	}

	void CParameter::slotCurveHoverEnter()
	{
	}

	void CParameter::slotCurveHoverLeave()
	{
		emit signalSetStatusText(QString());
	}

	void CParameter::slotCurveHoverMoved(CCurve* cv, const QPointF& ps)
	{
		qreal dp = m_depth->valueByCoord(m_depth->direction() == CRange::DirectionHorizontal ? ps.x() : ps.y());
		qreal pr = m_range->valueByCoord(m_depth->direction() == CRange::DirectionHorizontal ? ps.y() : ps.x(),
										 cv->propertyValue("range_start").toDouble(),
										 cv->propertyValue("range_stop").toDouble());

		emit signalSetStatusText(QString("Depth: %1\t\tValue: %2").arg(dp, 0, 'g', -1, QLatin1Char(' ')).arg(pr, 0, 'g', -1, QLatin1Char(' ')));
	}

	//

	void CParameter::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
	{
		QGraphicsItem::mouseMoveEvent(event);
	}

	void CParameter::mousePressEvent(QGraphicsSceneMouseEvent* event)
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
		}

		//QGraphicsItem::mousePressEvent(event);
	}

	void CParameter::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton)
		{
			emit signalLeftButtonRelease(this);
		}

		if (event->button() == Qt::RightButton)
		{
			emit signalRightButtonRelease(this);
		}

		//QGraphicsItem::mouseReleaseEvent(event);
	}

	QDomElement CParameter::serializeProps(QDomDocument& document) const
	{
		QDomElement props = document.createElement("properties");

		// property
		QDomElement prop_name = document.createElement("property");
		prop_name.setAttribute("name", "name");
		prop_name.setAttribute("value", m_name);
		props.appendChild(prop_name);

		// property
		QDomElement prop_stop = document.createElement("property");
		prop_stop.setAttribute("name", "unit");
		prop_stop.setAttribute("value", m_unit);
		props.appendChild(prop_stop);

		// property
		QDomElement prop_range_divide = document.createElement("property");
		prop_range_divide.setAttribute("name", "range_divide");
		prop_range_divide.setAttribute("value", m_range->propertyValue("divide").toInt());
		props.appendChild(prop_range_divide);

		// property
		QDomElement prop_brush_color = document.createElement("property");
		prop_brush_color.setAttribute("name", "brush_color");
		prop_brush_color.setAttribute("value", QVariant(m_brush.color()).toString());
		props.appendChild(prop_brush_color);

		return props;
	}

	QDomElement CParameter::serialize(QDomDocument& document) const
	{
		QDomElement elem = document.createElement("parameter");

		QDomElement elem_p0 = document.createElement("point0");
		elem_p0.setAttribute("x", m_range->firstPointPos().x());
		elem_p0.setAttribute("y", m_range->firstPointPos().y());
		elem.appendChild(elem_p0);

		QDomElement elem_p1 = document.createElement("point1");
		elem_p1.setAttribute("x", m_range->secondPointPos().x());
		elem_p1.setAttribute("y", m_range->secondPointPos().y());
		elem.appendChild(elem_p1);

		// properties
		QDomElement props = serializeProps(document);
		elem.appendChild(props);

		// shared properties
		QDomElement shared_props = serializeSharedProps(document);
		elem.appendChild(shared_props);

		// element
		QDomElement elem_curves = document.createElement("curves");
		elem.appendChild(elem_curves);
		for (t_curves_const_iter iter = m_curves.begin(); iter != m_curves.end(); ++iter)
		{
			QDomElement elem_curve = (*iter)->serialize(document);
			elem_curves.appendChild(elem_curve);
		}

		return elem;
	}

	void CParameter::deserialize(const QDomElement& element)
	{
		deserializeSharedProps(element);

		deserializeProps(element);

		//element
		QDomNodeList curve_nodes = element.elementsByTagName("curves");

		if (curve_nodes.count())
		{
			QDomElement curve_elem = curve_nodes.at(0).toElement().firstChildElement("curve");

			while (!curve_elem.isNull())
			{
				// curve
				CCurve *curve = this->directAddCurve();

				curve->deserialize(curve_elem);

				curve->sort();
				curve->calculate();
				curve->updateBoundingRect();
				curve->update();

				curve_elem = curve_elem.nextSiblingElement("curve");
			}
		}
	}
}
