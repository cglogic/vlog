#include "curve.h"

namespace General
{
	CCurve::ESort CCurve::m_sort;

	const QImage* CCurve::m_logging_image;

	CCurve::CCurve(QGraphicsItem *parent, QGraphicsScene* scene)
		: QAbstractGraphicsShapeItem(parent), m_scene(scene), m_spline_type(TypeCatmullRom), m_draw_step(1.0),
		  m_range_start(0), m_range_stop(100), m_painting(false), m_brush_cursor(0), m_paint_target(0), m_target_value(80),
		  m_target_coeff(2.0), m_target_color(QColor(0, 0, 0)), m_target_scatter(5), m_brush_radius(8.0), m_line_bias(1.0),
		  m_point_gap(6.0), m_image_angle(0.0)
	{
	}

	CCurve::CCurve(QGraphicsItem *parent, const QPointF& ps, const QPointF& psf, const QPointF& psl, QGraphicsScene* scene)
		: QAbstractGraphicsShapeItem(parent), m_scene(scene), m_spline_type(TypeCatmullRom), m_draw_step(1.0),
		  m_range_start(0), m_range_stop(100), m_painting(false), m_brush_cursor(0), m_paint_target(0), m_target_value(80),
		  m_target_coeff(2.0), m_target_color(QColor(0, 0, 0)), m_target_scatter(5), m_brush_radius(8.0), m_line_bias(1.0),
		  m_point_gap(6.0), m_image_angle(0.0)
	{
		init(ps);

		addPoint(psf);
		addPoint(psl + (m_sort == SortHorizontal ? QPointF(0.001, 0.0) : QPointF(0.0, 0.001)));
	}

	CCurve::~CCurve()
	{
		if (m_brush_cursor != 0)
		{
			delete m_brush_cursor;

			m_brush_cursor = 0;
		}

		emit signalDeleted(this);
	}

	void CCurve::init(const QPointF& ps)
	{
		setName("Curve");
		setIcon(QIcon(":/icons/draw_wave.png"));
		setCaption(tr("Curve"));
		setPos(ps);
		setZValue(40.0);

		m_brush_cursor = new QGraphicsEllipseItem(-m_brush_radius, -m_brush_radius, m_brush_radius * 2.0, m_brush_radius * 2.0);
		m_brush_cursor->setBrush(QBrush(QColor(50, 250, 50, 100)));
		m_brush_cursor->setPen(QPen(QColor(0, 255, 0, 200)));

		QPixmap pixmap(32, 32);
		pixmap.fill(Qt::transparent);

		m_blank_cursor = QCursor(pixmap);

		m_old_cursor = scene()->views().at(0)->viewport()->cursor();
	}

	//

	void CCurve::activate()
	{
		if (!isActivated())
		{
			CObject::activate();

			setPen(m_pen);

			if (!isLocked())
			{
				for(t_points_iter iter = m_points.begin(); iter != m_points.end(); ++iter)
				{
					(*iter)->show();
				}
			}

			parentItem()->show();

			update();

			setAcceptHoverEvents(true);

			emit signalActivated();
		}
	}

	void CCurve::deactivate()
	{
		if (isActivated())
		{
			CObject::deactivate();

			setPen(m_pen);

			if (!isLocked())
			{
				for(t_points_iter iter = m_points.begin(); iter != m_points.end(); ++iter)
				{
					(*iter)->hide();
				}
			}

			parentItem()->hide();

			update();

			setAcceptHoverEvents(false);

			emit signalDeactivated();
		}
	}

	void CCurve::lock()
	{
		CObject::lock();

		if (isActivated())
		{
			for(t_points_iter iter = m_points.begin(); iter != m_points.end(); ++iter)
			{
				(*iter)->hide();
			}
		}
	}

	void CCurve::unlock()
	{
		CObject::unlock();

		if (isActivated())
		{
			for(t_points_iter iter = m_points.begin(); iter != m_points.end(); ++iter)
			{
				(*iter)->show();
			}
		}
	}

	//

	void CCurve::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
	{
		Q_UNUSED(option);
		Q_UNUSED(widget);

		painter->setRenderHint(QPainter::Antialiasing);

		painter->setPen(m_pen);

		painter->drawPath(m_path);
	}

	QRectF CCurve::boundingRect() const
	{
		return m_rect;
	}

	//

	CCurve::ESort CCurve::sortType()
	{
		return m_sort;
	}

	void CCurve::setSortType(ESort st)
	{
		m_sort = st;
	}

	//

	void CCurve::updateBoundingRect()
	{
		prepareGeometryChange();

		if (!m_points.isEmpty())
		{
			if (m_sort == SortHorizontal)
			{
				m_rect = QRectF(QPointF(m_points.first()->pos().x(), mapFromParent(parentItem()->boundingRect().topLeft()).y()),
								QPointF(m_points.last()->pos().x(), mapFromParent(parentItem()->boundingRect().bottomRight()).y()));
			}
			else
			{
				m_rect = QRectF(QPointF(mapFromParent(parentItem()->boundingRect().topLeft()).x(), m_points.first()->pos().y()),
								QPointF(mapFromParent(parentItem()->boundingRect().bottomRight()).x(), m_points.last()->pos().y()));
			}
		}
		else
		{
			if (m_sort == SortHorizontal)
			{
				m_rect = QRectF(QPointF(0.0, mapFromParent(parentItem()->boundingRect().topLeft()).y()),
								QPointF(0.0, mapFromParent(parentItem()->boundingRect().bottomRight()).y()));
			}
			else
			{
				m_rect = QRectF(QPointF(mapFromParent(parentItem()->boundingRect().topLeft()).x(), 0.0),
								QPointF(mapFromParent(parentItem()->boundingRect().bottomRight()).x(), 0.0));
			}
		}
	}

	//

	CPoint* CCurve::directAddPoint(const QPointF& ps)
	{
		CPoint *pt = new CPoint(ps, 2.0, this);

		m_points.append(pt);

		connect(pt, SIGNAL(signalDelete(CPoint*)), this, SLOT(slotPointDelete(CPoint*)));
		connect(pt, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotPointDeleted(CObject*)));
		connect(pt, SIGNAL(signalMoved(CPoint*, const QPointF&)), this, SLOT(slotPointMoved(CPoint*, const QPointF&)));

		connect(pt, SIGNAL(signalMove(CPoint*, const QPointF&)), this, SLOT(slotPointMove(CPoint*, const QPointF&)));

		connect(pt, SIGNAL(signalLeftDoubleClick(CPoint*)), this, SLOT(slotPointLeftDoubleClick(CPoint*)));

		connect(pt, SIGNAL(signalLeftButtonPress(CPoint*)), this, SIGNAL(signalPointPressed(CPoint*)));
		connect(pt, SIGNAL(signalLeftButtonRelease(CPoint*)), this, SIGNAL(signalPointReleased(CPoint*)));

		return pt;
	}

	void CCurve::directRemovePoint(CPoint* pt)
	{
		t_points_iter iter = qFind(m_points.begin(), m_points.end(), pt);

		iter = m_points.erase(iter);
		m_scene->removeItem(pt);
		delete pt;
	}

	CPoint* CCurve::addPoint(const QPointF& ps)
	{
		QPointF lps = mapFromScene(ps);

		// Проверка на совпадение
		for(t_points_const_iter iter = m_points.begin(); iter != m_points.end(); ++iter)
		{
			if (m_sort == SortHorizontal)
			{
				if ((*(iter))->pos().x() - 0.000001 <= lps.x() && (*(iter))->pos().x() + 0.000001 >= lps.x())
				{
					return 0;
				}
			}
			else
			{
				if ((*(iter))->pos().y() - 0.000001 <= lps.y() && (*(iter))->pos().y() + 0.000001 >= lps.y())
				{
					return 0;
				}
			}
		}

		// Проверка пройдена успешно
		CPoint *pt = directAddPoint(lps);

		sort();
		//updateBoundingRect();
		calculate();
		update();

		emit signalPointCreated(pt);

		return pt;
	}

	void CCurve::removePoint(CPoint* pt)
	{
		directRemovePoint(pt);

		updateBoundingRect();
		calculate();
		update();
	}

	QPointF CCurve::movePoint(CPoint* pt, const QPointF& ps)
	{
		Q_UNUSED(pt);

		QPointF nps = ps;

		sort();
		updateBoundingRect();
		calculate();
		update();

		return nps;
	}

	int CCurve::pointCount() const
	{
		return m_points.size();
	}

	CPoint* CCurve::pointAt(int i)
	{
		return m_points.at(i);
	}

	const CPoint* CCurve::firstPoint() const
	{
		return m_points.first();
	}

	const CPoint* CCurve::lastPoint() const
	{
		return m_points.last();
	}

	bool CCurve::isPaintingEnabled() const
	{
		return m_painting;
	}

	void CCurve::setPaintingEnable(bool enable)
	{
		if (m_painting == enable)
		{
			return;
		}

		if (enable)
		{
			m_old_cursor = this->scene()->views().at(0)->cursor();

			scene()->views().at(0)->viewport()->setCursor(m_blank_cursor);

			//m_brush_cursor->setPos(this->scene()->views().at(0)->cursor().pos());

			scene()->addItem(m_brush_cursor);

			m_brush_cursor->setPos(m_last_cursor_pos);
		}
		else
		{
			scene()->removeItem(m_brush_cursor);

			scene()->views().at(0)->viewport()->setCursor(m_old_cursor);
		}

		m_painting = enable;
	}

	QGraphicsItem* CCurve::brushCursor() const
	{
		return m_brush_cursor;
	}

	void CCurve::brushRadiusChanded(int delta)
	{
		m_brush_radius += delta > 0.0 ? 1.0 : -1.0;

		m_brush_radius = qMin(50.0, qMax(1.0, m_brush_radius));

		m_brush_cursor->setRect(-m_brush_radius, -m_brush_radius, m_brush_radius * 2.0, m_brush_radius * 2.0);

		emit signalPropertiesUpdated(this);
	}

	void CCurve::cursorPosChanged(const QPointF& pos)
	{
		m_last_cursor_pos = pos;
	}

	void CCurve::setLoggingImage(const QImage& image)
	{
		m_logging_image = &image;
	}

	void CCurve::painting(const QPointF& pos)
	{
		QVector<QPoint> vert;
		QPointF one;
		qreal sum = 0.0;
		bool is_exists = false;

		if (m_sort == SortHorizontal)
		{
			for (int off_y = pos.y() - m_brush_radius; off_y < pos.y() + m_brush_radius; ++off_y)
			{
				if (passImagePixel(QPointF(pos.x(), off_y)))
				{
					vert.push_back(QPoint(pos.x(), off_y));
				}
			}
		}
		else
		{
			for (int off_x = pos.x() - m_brush_radius; off_x < pos.x() + m_brush_radius; ++off_x)
			{
				if (passImagePixel(QPointF(off_x, pos.y())))
				{
					vert.push_back(QPoint(off_x, pos.y()));
				}
			}
		}

		if (vert.size())
		{
			for (int i = 0; i < vert.size(); ++i)
			{
				sum += m_sort == SortHorizontal ? vert.at(i).y() : vert.at(i).x();
			}

			one = m_sort == SortHorizontal ? QPointF(pos.x(), sum / vert.size()) : QPointF(sum / vert.size(), pos.y());
		}

		for (int i = 0; i < m_points.size(); ++i)
		{
			QPointF p = m_points.at(i)->scenePos();

			if (((one.x() > p.x() - m_point_gap) && (one.x() < p.x() + m_point_gap)) &&
				((one.y() > p.y() - m_point_gap) && (one.y() < p.y() + m_point_gap)))
			{
				is_exists = true;

				break;
			}
		}

		if (!is_exists && vert.size() > 0)
		{
			addPoint(one);
		}

		removeInLinePoints();
	}

	void CCurve::paintingAuto(const QPointF& start)
	{
		QVector<QPoint> points;
		QVector<QPoint> all_points;

		QPointF last;

		int last_index = -1;
		int curr_index;

		bool loop = true;

		QPointF curr = start;
		QPointF end = lastPoint()->scenePos();

		qreal sum = 0.0;
		qreal length = 0.0;

		if (m_sort == SortHorizontal)
		{
			while (curr.x() < end.x() && loop)
			{
				points.clear();
				curr_index = 0;

				curr.setX(curr.x() + 1.0);

				for (int off_y = curr.y() - m_brush_radius; off_y < curr.y() + m_brush_radius; ++off_y, ++curr_index)
				{
					if (passImagePixel(QPointF(curr.x(), off_y)))
					{
						if (last_index == -1 || (curr_index - last_index <= 2))
						{
							points.push_back(QPoint(curr.x(), off_y));

							last_index = curr_index;
						}
						else
						{
							loop = false;

							break;
						}
					}
				}

				sum = 0.0;

				if (points.size())
				{
					for (int i = 0; i < points.size(); ++i)
					{
						sum += points.at(i).y();
					}

					curr = QPoint(points.at(0).x(), sum / points.size());

					all_points.push_back(curr.toPoint());

					last = curr;
				}
				else
				{
					length = qSqrt(qPow(last.x() - curr.x(), 2.0) + qPow(last.y() - curr.y(), 2.0));

					if (length > m_brush_radius)
					{
						loop = false;
					}
				}
			}
		}
		else
		{
			while (curr.y() < end.y() && loop)
			{
				points.clear();
				curr_index = 0;

				curr.setY(curr.y() + 1.0);

				for (int off_x = curr.x() - m_brush_radius; off_x < curr.x() + m_brush_radius; ++off_x, ++curr_index)
				{
					if (passImagePixel(QPointF(off_x, curr.y())))
					{
						if (last_index == -1 || (curr_index - last_index <= 2))
						{
							points.push_back(QPoint(off_x, curr.y()));

							last_index = curr_index;
						}
						else
						{
							loop = false;

							break;
						}
					}
				}

				sum = 0.0;

				if (points.size())
				{
					for (int i = 0; i < points.size(); ++i)
					{
						sum += points.at(i).x();
					}

					curr = QPoint(sum / points.size(), points.at(0).y());

					all_points.push_back(curr.toPoint());

					last = curr;
				}
				else
				{
					length = qSqrt(qPow(last.x() - curr.x(), 2.0) + qPow(last.y() - curr.y(), 2.0));

					if (length > m_brush_radius)
					{
						loop = false;
					}
				}
			}
		}

		for (int i = 0; i < all_points.size(); ++i)
		{
			QPointF one = all_points.at(i);

			bool is_exists = false;

			for (int i = 0; i < m_points.size(); ++i)
			{
				QPointF p = m_points.at(i)->scenePos();

				if (((one.x() > p.x() - m_point_gap) && (one.x() < p.x() + m_point_gap)) &&
					((one.y() > p.y() - m_point_gap) && (one.y() < p.y() + m_point_gap)))
				{
					is_exists = true;

					break;
				}
			}

			if (!is_exists )
			{
				addPoint(one);
			}
		}

		removeInLinePoints();
	}

	void CCurve::pipette(const QPointF& pos)
	{
		qreal angle = m_image_angle * M_PI / 180.0;

		qreal tx, ty;

		tx = pos.x() * qCos(angle) - pos.y() * qSin(angle);
		ty = pos.x() * qSin(angle) + pos.y() * qCos(angle);

		QPoint p = QPoint(m_logging_image->width() / 2.0 + tx, m_logging_image->height() / 2.0 + ty);

		m_target_color = m_logging_image->pixel(p);

		emit signalPropertiesUpdated(this);
	}

	//

	QVariant CCurve::propertyValue(const QString& name) const
	{
		if (name == "spline_type")
		{
			return m_spline_type;
		}
		else if (name == "draw_step")
		{
			return m_draw_step;
		}
		else if (name == "range_start")
		{
			return m_range_start;
		}
		else if (name == "range_stop")
		{
			return m_range_stop;
		}
		else if (name == "brush_radius")
		{
			return m_brush_radius;
		}
		else if (name == "paint_target")
		{
			return m_paint_target;
		}
		else if (name == "target_value")
		{
			return m_target_value;
		}
		else if (name == "target_coeff")
		{
			return m_target_coeff;
		}
		else if (name == "target_color")
		{
			return m_target_color;
		}
		else if (name == "target_scatter")
		{
			return m_target_scatter;
		}
		else if (name == "line_bias")
		{
			return m_line_bias;
		}
		else if (name == "point_gap")
		{
			return m_point_gap;
		}

		return CObject::propertyValue(name);
	}

	void CCurve::setPropertyValue(const QString& name, const QVariant& value)
	{
		if (name == "spline_type")
		{
			m_spline_type = (EType)value.toInt();

			calculate();
			update();
		}
		else if (name == "draw_step")
		{
			m_draw_step = value.toDouble();

			calculate();
			update();
		}
		else if (name == "range_start")
		{
			m_range_start = value.toDouble();

			if (m_range_start > m_range_stop)
			{
				//m_range_start = m_range_stop;
				m_range_stop = m_range_start;
			}

			emit signalPropertiesUpdated(this);
		}
		else if (name == "range_stop")
		{
			m_range_stop = value.toDouble();

			if (m_range_stop < m_range_start)
			{
				//m_range_stop = m_range_start;
				m_range_start = m_range_stop;
			}

			emit signalPropertiesUpdated(this);
		}
		else if (name == "brush_radius")
		{
			m_brush_radius = value.toDouble();

			m_brush_cursor->setRect(-m_brush_radius, -m_brush_radius, m_brush_radius * 2.0, m_brush_radius * 2.0);
		}
		else if (name == "paint_target")
		{
			m_paint_target = value.toInt();

			emit signalPropAttrUpdate(this, "target_value", "readonly", m_paint_target == 4 || m_paint_target == 5);
			emit signalPropAttrUpdate(this, "target_coeff", "readonly", m_paint_target == 0 || m_paint_target == 4 || m_paint_target == 5);
			emit signalPropAttrUpdate(this, "target_color", "readonly", m_paint_target != 4 && m_paint_target != 5);
			emit signalPropAttrUpdate(this, "target_scatter", "readonly", m_paint_target != 4 && m_paint_target != 5);
		}
		else if (name == "target_value")
		{
			m_target_value = value.toInt();
		}
		else if (name == "target_coeff")
		{
			m_target_coeff = value.toDouble();
		}
		else if (name == "target_color")
		{
			m_target_color = value.value<QColor>();
		}
		else if (name == "target_scatter")
		{
			m_target_scatter = value.toInt();
		}
		else if (name == "line_bias")
		{
			m_line_bias = value.toDouble();
		}
		else if (name == "point_gap")
		{
			m_point_gap = value.toDouble();
		}
		else
		{
			CObject::setPropertyValue(name, value);
		}
	}

	//

	qreal CCurve::coordByCoord(qreal cd) const
	{
		qreal coord = 0.0;

		double x = m_sort == SortHorizontal ? mapFromScene(QPointF(cd, 0.0)).x() : mapFromScene(QPointF(0.0, cd)).y();
		double y = 0.0;

		y = spline1dcalc(m_interpolant, x);

		coord = m_sort == SortHorizontal ? mapToScene(QPointF(m_sort == SortHorizontal ? x : y, m_sort == SortHorizontal ? y : x)).y()
										 : mapToScene(QPointF(m_sort == SortHorizontal ? x : y, m_sort == SortHorizontal ? y : x)).x();

		return coord;
	}

	//

	bool CCurve::slotPointDelete(CPoint *pt)
	{
		if (isPaintingEnabled())
		{
			return false;
		}

		if (m_points.size() > 2)
		{
			removePoint(pt);
			calculate();

			//return emit signalPointDelete(pt);
			emit signalPointDeleted(pt);
		}
		else
		{
			scene()->views().at(0)->viewport()->setCursor(m_old_cursor);

			emit signalDelete(this);
		}

		return true;
	}

	void CCurve::slotPointDeleted(CObject* ob)
	{
		Q_UNUSED(ob);

		//emit signalPointDeleted((CPoint*)ob);
	}

	void CCurve::slotPointLeftDoubleClick(CPoint* pt)
	{
		if (isPaintingEnabled())
		{
			paintingAuto(pt->scenePos());
		}
	}

	void CCurve::slotImageRotate(qreal angle)
	{
		m_image_angle = angle;
	}

	QPointF CCurve::slotPointMoved(CPoint *pt, const QPointF& ps)
	{
		QPointF p = movePoint(pt, ps);

		emit signalHoverMoved(this, mapToScene(ps));
		emit signalPointMoved(pt, ps);

		return p;
	}

	QPointF CCurve::slotPointMove(CPoint* pt, const QPointF& ps)
	{
		QPointF nps = mapFromParent(emit signalPointMove(pt, mapToParent(ps)));

		// Проверка на совпадение
		for(t_points_const_iter iter = m_points.begin(); iter != m_points.end(); ++iter)
		{
			if ((*(iter)) != pt)
			{
				if (m_sort == SortHorizontal)
				{
					if ((*(iter))->pos().x() - 0.000001 <= nps.x() && (*(iter))->pos().x() + 0.000001 >= nps.x())
					{
						nps.setX(pt->pos().x());
					}
				}
				else
				{
					if ((*(iter))->pos().y() - 0.000001 <= nps.y() && (*(iter))->pos().y() + 0.000001 >= nps.y())
					{
						nps.setY(pt->pos().y());
					}
				}
			}
		}

		return nps;
	}

	//

	void CCurve::sort()
	{
		qSort(m_points.begin(), m_points.end(), [](const CPoint* p0, const CPoint* p1)
			{
				return m_sort == CCurve::SortHorizontal ? p0->pos().x() < p1->pos().x() : p0->pos().y() < p1->pos().y();
			});
	}

	void CCurve::calculate()
	{
		int count = m_points.count();

		if (count > 1 && m_points.first()->pos() != m_points.last()->pos())
		{
			alglib::real_1d_array xa;
			alglib::real_1d_array ya;

			xa.setlength(count);
			ya.setlength(count);

			for (int i = 0; i < count; ++i)
			{
				xa(i) = (m_sort == SortHorizontal ? m_points.at(i)->pos().x() : m_points.at(i)->pos().y());
				ya(i) = (m_sort == SortHorizontal ? m_points.at(i)->pos().y() : m_points.at(i)->pos().x());
			}

			// !!! DEBUG !!!
			/*double eq = (m_sort == SortHorizontal ? m_points.at(0)->pos().x() : m_points.at(0)->pos().y());
			for (int i = 1; i < count; ++i)
			{
				if (eq == (m_sort == SortHorizontal ? m_points.at(i)->pos().x() : m_points.at(i)->pos().y()))
				{
					qDebug() << "EQL!!! " << i - 1 << " and " << i;
				}
				eq = (m_sort == SortHorizontal ? m_points.at(i)->pos().x() : m_points.at(i)->pos().y());
			}*/
			// !!! DEBUG !!!

			if ((m_spline_type == TypeAkima && count > 5) || (m_spline_type != TypeAkima && count > 2))
			{
				switch (m_spline_type)
				{
				case TypeAkima:
					alglib::spline1dbuildakima(xa, ya, m_interpolant);

					break;
				case TypeCatmullRom:
					alglib::spline1dbuildcatmullrom(xa, ya, m_interpolant);

					break;

				case TypeLinear:
				default:
					alglib::spline1dbuildlinear(xa, ya, m_interpolant);

					break;
				}
			}
			else
			{
				alglib::spline1dbuildlinear(xa, ya, m_interpolant);
			}

			QPainterPath path;

			double i = xa[0];
			double j = 0.0;

			j = alglib::spline1dcalc(m_interpolant, i);

			path.moveTo(m_sort == SortHorizontal ? i : j, m_sort == SortHorizontal ? j : i);

			for (i += m_draw_step; i < xa[count - 1]; i += m_draw_step)
			{
				j = alglib::spline1dcalc(m_interpolant, i);

				path.lineTo(m_sort == SortHorizontal ? i : j, m_sort == SortHorizontal ? j : i);
			}

			i = xa[count - 1];
			j = alglib::spline1dcalc(m_interpolant, i);

			path.lineTo(m_sort == SortHorizontal ? i : j, m_sort == SortHorizontal ? j : i);

			m_path.swap(path);
		}
	}

	//

	bool CCurve::passImagePixel(QPointF pf) const
	{
		qreal angle = m_image_angle * M_PI / 180.0;

		qreal tx = pf.x() * qCos(angle) - pf.y() * qSin(angle);
		qreal ty = pf.x() * qSin(angle) + pf.y() * qCos(angle);

		QPoint p(m_logging_image->width() / 2.0 + tx, m_logging_image->height() / 2.0 + ty);

		if (m_paint_target == 0) // gray
		{
			if (m_target_value > qGray(m_logging_image->pixel(p)))
			{
				return true;
			}
		}
		else if (m_paint_target == 1) // red
		{
			int r = qRed(m_logging_image->pixel(p));
			int g = qGreen(m_logging_image->pixel(p)) * m_target_coeff;
			int b = qBlue(m_logging_image->pixel(p)) * m_target_coeff;

			if (r > g && r > b && r > m_target_value)
			{
				return true;
			}
		}
		else if (m_paint_target == 2) // green
		{
			int r = qRed(m_logging_image->pixel(p)) * m_target_coeff;
			int g = qGreen(m_logging_image->pixel(p));
			int b = qBlue(m_logging_image->pixel(p)) * m_target_coeff;

			if (g > r && g > b && g > m_target_value)
			{
				return true;
			}
		}
		else if (m_paint_target == 3) // blue
		{
			int r = qRed(m_logging_image->pixel(p)) * m_target_coeff;
			int g = qGreen(m_logging_image->pixel(p)) * m_target_coeff;
			int b = qBlue(m_logging_image->pixel(p));

			if (b > r && b > g && b > m_target_value)
			{
				return true;
			}
		}
		else if (m_paint_target == 4) // pipette
		{
			int r = qRed(m_logging_image->pixel(p));
			int g = qGreen(m_logging_image->pixel(p));
			int b = qBlue(m_logging_image->pixel(p));

			int rc = qRed(m_target_color.rgb());
			int gc = qGreen(m_target_color.rgb());
			int bc = qBlue(m_target_color.rgb());

			if ((r - m_target_scatter) < rc && (r + m_target_scatter) > rc &&
				(g - m_target_scatter) < gc && (g + m_target_scatter) > gc &&
				(b - m_target_scatter) < bc && (b + m_target_scatter) > bc)
			{
				return true;
			}
		}
		else if (m_paint_target == 5) // adaptive
		{
			int r = qRed(m_logging_image->pixel(p));
			int g = qGreen(m_logging_image->pixel(p));
			int b = qBlue(m_logging_image->pixel(p));

			int rc = qRed(m_target_color.rgb());
			int gc = qGreen(m_target_color.rgb());
			int bc = qBlue(m_target_color.rgb());

			if ((r - m_target_scatter) < rc && (r + m_target_scatter) > rc &&
				(g - m_target_scatter) < gc && (g + m_target_scatter) > gc &&
				(b - m_target_scatter) < bc && (b + m_target_scatter) > bc)
			{
				m_target_color = QColor(r, g, b);

				return true;
			}
		}

		return false;
	}

	//

	void CCurve::removeInLinePoints()
	{
		CPoint *p0 = m_points.at(0);
		CPoint *p1 = m_points.at(1);

		for (int i = 2; i < m_points.size() - 1; ++i)
		{
			if (!pointInLine(p0->scenePos(), p1->scenePos(), m_points.at(i)->scenePos()))
			{
				p0 = p1;
				p1 = m_points.at(i);
			}
			else
			{
				removePoint(p1);

				p1 = m_points.at(i);
			}
		}
	}

	//

	bool CCurve::pointInLine(const QPointF& p0, const QPointF& p1, const QPointF& p) const
	{
		// line: ax + by + c = 0

		qreal a = p0.y() - p1.y();
		qreal b = p1.x() - p0.x();
		qreal c = p0.x() * p1.y() - p1.x() * p0.y();

		qreal y = (a * p.x() + c) / -b;

		if (qAbs(p.y() - y) < m_line_bias)
		{
			return true;
		}

		return false;
	}

	//

	void CCurve::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
	{
		QGraphicsItem::mouseMoveEvent(event);
	}

	void CCurve::mousePressEvent(QGraphicsSceneMouseEvent* event)
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

			if (isPaintingEnabled())
			{
			}
			else if (isActivated() && m_points.size() <= 2)
			{
				scene()->views().at(0)->viewport()->setCursor(m_old_cursor);

				emit signalDelete(this);
			}
		}

		//QGraphicsItem::mousePressEvent(event);
	}

	void CCurve::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
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

	void CCurve::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
	{
		emit signalHoverEnter();

		QGraphicsItem::hoverEnterEvent(event);
	}

	void CCurve::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
	{
		emit signalHoverLeave();

		QGraphicsItem::hoverLeaveEvent(event);
	}

	void CCurve::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
	{
		emit signalHoverMoved(this, mapToScene(event->pos()));

		QGraphicsItem::hoverMoveEvent(event);
	}

	//

	QDomElement CCurve::serializeProps(QDomDocument& document) const
	{
		QDomElement props = document.createElement("properties");

		// property
		QDomElement prop_spline_type = document.createElement("property");
		prop_spline_type.setAttribute("name", "spline_type");
		prop_spline_type.setAttribute("value", QString::number(m_spline_type));
		props.appendChild(prop_spline_type);

		// property
		QDomElement prop_draw_step = document.createElement("property");
		prop_draw_step.setAttribute("name", "draw_step");
		prop_draw_step.setAttribute("value", QString::number(m_draw_step));
		props.appendChild(prop_draw_step);

		// property
		QDomElement prop_range_start = document.createElement("property");
		prop_range_start.setAttribute("name", "range_start");
		prop_range_start.setAttribute("value", QString::number(m_range_start));
		props.appendChild(prop_range_start);

		// property
		QDomElement prop_range_stop = document.createElement("property");
		prop_range_stop.setAttribute("name", "range_stop");
		prop_range_stop.setAttribute("value", QString::number(m_range_stop));
		props.appendChild(prop_range_stop);

		// property
		QDomElement prop_brush_radius = document.createElement("property");
		prop_brush_radius.setAttribute("name", "brush_radius");
		prop_brush_radius.setAttribute("value", QString::number(m_brush_radius));
		props.appendChild(prop_brush_radius);

		// property
		QDomElement prop_paint_target = document.createElement("property");
		prop_paint_target.setAttribute("name", "paint_target");
		prop_paint_target.setAttribute("value", QString::number(m_paint_target));
		props.appendChild(prop_paint_target);

		// property
		QDomElement prop_target_value = document.createElement("property");
		prop_target_value.setAttribute("name", "target_value");
		prop_target_value.setAttribute("value", QString::number(m_target_value));
		props.appendChild(prop_target_value);

		// property
		QDomElement prop_target_coeff = document.createElement("property");
		prop_target_coeff.setAttribute("name", "target_coeff");
		prop_target_coeff.setAttribute("value", QString::number(m_target_coeff));
		props.appendChild(prop_target_coeff);

		// property
		QDomElement prop_line_bias = document.createElement("property");
		prop_line_bias.setAttribute("name", "line_bias");
		prop_line_bias.setAttribute("value", QString::number(m_line_bias));
		props.appendChild(prop_line_bias);

		// property
		QDomElement prop_point_gap = document.createElement("property");
		prop_point_gap.setAttribute("name", "point_gap");
		prop_point_gap.setAttribute("value", QString::number(m_point_gap));
		props.appendChild(prop_point_gap);

		return props;
	}

	QDomElement CCurve::serialize(QDomDocument& document) const
	{
		QDomElement elem = document.createElement("curve");

		// properties
		QDomElement props = serializeProps(document);
		elem.appendChild(props);

		// shared properties
		QDomElement shared_props = serializeSharedProps(document);
		elem.appendChild(shared_props);

		// element
		QDomElement elem_points = document.createElement("points");
		elem.appendChild(elem_points);
		for (t_points_const_iter iter = m_points.begin(); iter != m_points.end(); ++iter)
		{
			QDomElement elem_point = (*iter)->serialize(document);
			elem_points.appendChild(elem_point);
		}

		return elem;
	}

	void CCurve::deserialize(const QDomElement& element)
	{
		deserializeSharedProps(element);

		deserializeProps(element);

		// element
		QDomNodeList point_nodes = element.elementsByTagName("points");

		if (point_nodes.count())
		{
			QDomElement point_elem = point_nodes.at(0).toElement().firstChildElement("point");

			while (!point_elem.isNull())
			{
				QDomElement point_p = point_elem.elementsByTagName("pos").at(0).toElement();

				CPoint *point = directAddPoint(QPointF(point_p.attribute("x").toDouble(), point_p.attribute("y").toDouble()));

				point->deserialize(point_elem);

				point_elem = point_elem.nextSiblingElement("point");
			}
		}
	}
}
