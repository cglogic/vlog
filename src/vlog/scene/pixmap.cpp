#include "pixmap.h"

#include <qmath.h>

#include <QGraphicsView>

#ifdef VL_USE_OPENMP
#	include <omp.h>
#endif

namespace General
{
	CPixmap::CPixmap(QGraphicsScene* sc)
		: QGraphicsPixmapItem(), m_scene(sc), m_curve(0), m_angle(0.0), m_divide(1), m_is_saved(false), m_opencl_enable(false), m_coords(nullptr)
	{
	}

	CPixmap::CPixmap(const QPixmap& pm, QGraphicsScene* sc)
		: QGraphicsPixmapItem(), m_scene(sc), m_curve(0), m_angle(0.0), m_divide(1), m_is_saved(false), m_opencl_enable(false), m_coords(nullptr)
	{
		init(pm);
	}

	CPixmap::~CPixmap()
	{
		emit signalDeleted(this);
	}

	void CPixmap::init(const QPixmap& pm)
	{
#ifdef VL_USE_OPENCL
		if (m_opencl_enable)
		{
			m_opencl.destroy();

			m_opencl_enable = false;
		}
#endif // VL_USE_OPENCL

		setName("Log");
		setIcon(QIcon(":/icons/table.png"));
		setCaption(tr("Log"));

		setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
		setFlag(ItemClipsToShape, true);

		m_original_pixmap = pm;
		m_image = m_original_pixmap.toImage();

		m_image.convertToFormat(QImage::Format_RGB32);

		setPixmap(pm);

		setPos(0.0, 0.0);
		setOffset(QPointF(-(boundingRect().width() / 2.0), -(boundingRect().height() / 2.0)));
		setTransformationMode(Qt::SmoothTransformation);
		setZValue(0.0);

		m_rect = boundingRegion(sceneTransform()).boundingRect();

		if (m_rect.width() > m_rect.height())
		{
			m_rect.setHeight(m_rect.width());
		}
		else
		{
			m_rect.setWidth(m_rect.height());
		}

		m_rect.moveCenter(QPointF(0.0, 0.0));

		//m_scene->setSceneRect(boundingRegion(sceneTransform()).boundingRect());
		//m_scene->views().first()->centerOn(0.0, 0.0);

		createCurve();

		calculate();

		m_is_saved = false;

#ifdef VL_USE_OPENCL
		int width = m_image.width();
		int height = m_image.height();

		delete[] m_coords;

		m_coords = new int[width > height ? width : height];

		m_original_image = m_original_pixmap.toImage();
		m_original_image.convertToFormat(QImage::Format_RGB32);

		unsigned char *source = m_original_image.bits();
		unsigned char *dest = m_image.bits();

		m_opencl_enable = m_opencl.initialize(COpenCLFramework::TD_GPU, source, dest, width, height);
#endif // VL_USE_OPENCL
	}

	void CPixmap::directCreateCurve()
	{
		if (m_curve != 0)
		{
			delete m_curve;
			m_curve = 0;
		}

		CCurve::setSortType(boundingRect().width() >= boundingRect().height() ? CCurve::SortHorizontal : CCurve::SortVertical);

		m_curve = new CCurve(this, m_scene);
		m_curve->init(QPointF(0.0, 0.0));

		connect(m_curve, SIGNAL(signalPointMove(CPoint*, const QPointF&)), this, SLOT(slotPointMove(CPoint*, const QPointF&)));
		connect(m_curve, SIGNAL(signalPointMoved(CPoint*, const QPointF&)), this, SLOT(slotPointMoved(CPoint*, const QPointF&)));
		connect(m_curve, SIGNAL(signalPointDeleted(CPoint*)), this, SLOT(slotPointDeleted(CPoint*)));
		connect(m_curve, SIGNAL(signalPointPressed(CPoint*)), this, SLOT(slotPointPressed(CPoint*)));
		connect(m_curve, SIGNAL(signalPointReleased(CPoint*)), this, SLOT(slotPointReleased(CPoint*)));
		connect(m_curve, SIGNAL(signalDelete(CCurve*)), this, SLOT(slotCurveDelete(CCurve*)));
	}

	void CPixmap::createCurve()
	{
		directCreateCurve();

		m_curve->addPoint(CCurve::sortType() == CCurve::SortHorizontal ? QPointF(boundingRect().left(), boundingRect().top() + boundingRect().height() / 2.0)
																	   : QPointF(boundingRect().left() + boundingRect().width() / 2.0, boundingRect().top()));
		m_curve->addPoint(CCurve::sortType() == CCurve::SortHorizontal ? QPointF(boundingRect().right(), boundingRect().top() + boundingRect().height() / 2.0)
																	   : QPointF(boundingRect().left() + boundingRect().width() / 2.0, boundingRect().bottom()));

		m_curve->sort();
		m_curve->calculate();
		m_curve->updateBoundingRect();
		m_curve->update();

		//calculate();

		m_curve->activate();
		m_curve->lock();
		m_curve->hide();
	}

	void CPixmap::removeCurve()
	{
		if (m_curve != 0)
		{
			delete m_curve;
			m_curve = 0;

			calculate();
			update();
		}
	}

	//

	void CPixmap::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
	{
		QGraphicsPixmapItem::paint(painter, option, widget);

		if (!isLocked() && isActivated())
		{
			painter->setRenderHint(QPainter::Antialiasing);
			painter->setRenderHint(QPainter::TextAntialiasing);
			painter->setRenderHint(QPainter::SmoothPixmapTransform);
			painter->setRenderHint(QPainter::HighQualityAntialiasing);

			QTransform old_trans = painter->transform();
			QTransform new_trans = old_trans;

			new_trans.rotate(-m_angle);

			painter->resetTransform();
			painter->setTransform(new_trans);

			painter->setPen(QPen(QColor(0, 100, 100, 255)));

			painter->drawPath(m_path);

			painter->resetTransform();
			painter->setTransform(old_trans);

			/*if (m_curve != 0)
			{
				m_curve->update();
			}*/
		}
	}

	//

	void CPixmap::activate()
	{
		if (!isActivated())
		{
			CObject::activate();

			if (!isLocked())
			{
				if (m_curve != 0)
				{
					//m_curve->show();
					//m_curve->unlock();
					m_curve->activate();
				}
			}

			update();

			emit signalActivated();
		}
	}

	void CPixmap::deactivate()
	{
		if (isActivated())
		{
			CObject::deactivate();

			if (!isLocked())
			{
				if (m_curve != 0)
				{
					m_curve->deactivate();
					//m_curve->lock();
					//m_curve->hide();
				}
			}

			update();

			emit signalDeactivated();
		}
	}

	void CPixmap::lock()
	{
		CObject::lock();

		/*if (m_curve != 0)
		{
		}*/
	}

	void CPixmap::unlock()
	{
		CObject::unlock();
	}

	//

	qreal CPixmap::rotateAngle() const
	{
		return -m_angle;
	}

	const QImage& CPixmap::image()
	{
		return m_image;
	}

	//

	CCurve* CPixmap::curve()
	{
		return m_curve;
	}

	CPoint* CPixmap::addPoint(const QPointF& ps)
	{
		CPoint *pt = m_curve->addPoint(ps);

		//calculate();
		m_curve->updateBoundingRect();

		return pt;
	}

	void CPixmap::curveActivate(bool act)
	{
		if (m_curve != 0)
		{
			if (act && !isLocked())
			{
				CCurve::setSortType(boundingRect().width() >= boundingRect().height() ? CCurve::SortHorizontal : CCurve::SortVertical);

				m_curve->show();
				m_curve->unlock();
			}
			else
			{
				m_curve->lock();
				m_curve->hide();
			}
		}
	}

	//

	QVariant CPixmap::propertyValue(const QString& name) const
	{
		if (name == "rotate_angle")
		{
			return m_angle;
		}
		else if (name == "divide")
		{
			return m_divide;
		}
		else if (name == "spline_type")
		{
			return m_curve->propertyValue(name);
		}

		return CObject::propertyValue(name);
	}

	void CPixmap::setPropertyValue(const QString& name, const QVariant& value)
	{
		if (name == "rotate_angle")
		{
			removeCurve();

			m_angle = value.toDouble();

			setRotation(m_angle);

			createCurve();

			calculate();
			update();

			emit signalRotate(-m_angle);
		}
		else if (name == "divide")
		{
			m_divide = value.toInt();

			calculate();
			update();
		}
		else if (name == "spline_type")
		{
			m_curve->setPropertyValue(name, value);

			calculate();
			update();
		}
		else
		{
			CObject::setPropertyValue(name, value);
		}
	}

	//

	QPointF CPixmap::slotPointMove(CPoint *pt, const QPointF &ps)
	{
		Q_UNUSED(pt);

		QPointF nps = ps;
		QRectF rc = boundingRect();

		//nps.setX(pt->pos().x());

		if (!rc.contains(nps))
		{
			nps.setX(qMin(rc.right(), qMax(nps.x(), rc.left())));
			nps.setY(qMin(rc.bottom(), qMax(nps.y(), rc.top())));
		}

		return nps;
	}

	QPointF CPixmap::slotPointMoved(CPoint *pt, const QPointF &ps)
	{
		Q_UNUSED(pt);

		if (m_opencl_enable)
		{
			calculate();
			update();
		}

		return ps;
	}

	void CPixmap::slotPointPressed(CPoint* pt)
	{
		Q_UNUSED(pt);
	}

	void CPixmap::slotPointReleased(CPoint* pt)
	{
		Q_UNUSED(pt);

		calculate();
		update();
	}

	void CPixmap::slotPointDeleted(CPoint* pt)
	{
		Q_UNUSED(pt);

		if (!m_opencl_enable)
		{
			calculate();
			update();
		}
	}

	bool CPixmap::slotCurveDelete(CCurve* cv)
	{
		Q_UNUSED(cv);

		//removeCurve();

		//return true;
		return false;
	}

	//

	void CPixmap::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
	{
		QGraphicsItem::mouseMoveEvent(event);
	}

	void CPixmap::mousePressEvent(QGraphicsSceneMouseEvent* event)
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

			if (isActivated())
			{
				if (emit signalDelete(this))
				{
					event->accept();
				}
			}
		}

		QGraphicsItem::mousePressEvent(event);
	}

	void CPixmap::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton)
		{
			emit signalLeftButtonRelease(this);
		}

		if (event->button() == Qt::RightButton)
		{
			emit signalRightButtonRelease(this);
		}

		QGraphicsItem::mouseReleaseEvent(event);
	}

	//

	void CPixmap::calculate()
	{
		QPainterPath path;

		qreal step = (m_rect.right() - m_rect.left()) / m_divide;

		for (qreal i = m_rect.left() + step; i < m_rect.right() - (step / 2.0); i += step)
		{
			path.moveTo(i, m_rect.bottom());
			path.lineTo(i, m_rect.top());
		}

		for (qreal j = m_rect.top() + step; j <= m_rect.bottom() - (step / 2.0); j += step)
		{
			path.moveTo(m_rect.left(), j);
			path.lineTo(m_rect.right(), j);
		}

		m_path.swap(path);

		m_scene->setSceneRect(boundingRegion(sceneTransform()).boundingRect());

		//

		if (m_curve != 0 && m_curve->pointCount() > 1)
		{
#ifdef VL_USE_OPENCL
			if (m_opencl_enable)
			{
				int width = m_original_image.width();
				int height = m_original_image.height();

				if (width > height)
				{
					for (int xi = 0; xi < width; ++xi)
					{
						m_coords[xi] = qRound(mapFromScene(QPointF(0.0, m_curve->coordByCoord(mapToScene(QPointF(xi + offset().x(), 0.0)).x()))).y());
					}
				}
				else
				{
					for (int yi = 0; yi < height; ++yi)
					{
						m_coords[yi] = qRound(mapFromScene(QPointF(m_curve->coordByCoord(mapToScene(QPointF(0.0, yi + offset().y())).y()), 0.0)).x());
					}
				}

				m_opencl_enable = m_opencl.execute(m_coords);
			}
			else
			{
#endif // VL_USE_OPENCL
				QImage image_original = m_original_pixmap.toImage();
				image_original.convertToFormat(QImage::Format_RGB32);

				int wd = image_original.width();
				int ht = image_original.height();

				int ls = image_original.bytesPerLine();
				int bs = image_original.byteCount();

				uchar *dst = m_image.bits();
				uchar *src = image_original.bits();

				const int bytePerPixel = 4;

				memcpy_s(dst, bs, src, bs);

				if (CCurve::sortType() == CCurve::SortHorizontal)
				{
#ifdef VL_USE_OPENMP
					#pragma omp parallel for
#endif
					for (int col = 0; col < wd; ++col)
					{
						qreal i = mapFromScene(QPointF(0.0, m_curve->coordByCoord(mapToScene(QPointF(col + offset().x(), 0.0)).x()))).y();

						int di = qRound(i);
						int row;

						if (di > 0)
						{
							for (row = ht - 1; row >= di; --row)
							{
								memcpy_s(dst + (ls * row) + (bytePerPixel * col), bytePerPixel, src + (ls * (row - di)) + (bytePerPixel * col), bytePerPixel);
							}

							for (; row >= 0; --row)
							{
								memcpy_s(dst + (ls * row) + (bytePerPixel * col), bytePerPixel, src + (ls * (0)) + (bytePerPixel * col), bytePerPixel);
							}
						}

						if (di < 0)
						{
							for (row = 0; row <= di + ht - 1; ++row)
							{
								memcpy_s(dst + (ls * row) + (bytePerPixel * col), bytePerPixel, src + (ls * (row - di)) + (bytePerPixel * col), bytePerPixel);
							}

							for (; row <= ht - 1; ++row)
							{
								memcpy_s(dst + (ls * row) + (bytePerPixel * col), bytePerPixel, src + (ls * (ht - 1)) + (bytePerPixel * col), bytePerPixel);
							}
						}
					}
				}
				else
				{
#ifdef VL_USE_OPENMP
					#pragma omp parallel for
#endif
					for (int row = 0; row < ht; ++row)
					{
						qreal i = mapFromScene(QPointF(m_curve->coordByCoord(mapToScene(QPointF(0.0, row + offset().y())).y()), 0.0)).x();

						int di = qRound(i);
						int col;

						if (di > 0)
						{
							for (col = wd - 1; col >= di; --col)
							{
								memcpy_s(dst + (ls * row) + (bytePerPixel * col), bytePerPixel, src + (ls * row) + (bytePerPixel * (col - di)), bytePerPixel);
							}

							for (; col >= 0; --col)
							{
								memcpy_s(dst + (ls * row) + (bytePerPixel * col), bytePerPixel, src + (ls * row) + (bytePerPixel * 0), bytePerPixel);
							}
						}

						if (di < 0)
						{
							for (col = 0; col <= di + wd - 1; ++col)
							{
								memcpy_s(dst + (ls * row) + (bytePerPixel * col), bytePerPixel, src + (ls * row) + (bytePerPixel * (col - di)), bytePerPixel);
							}

							for (; col <= wd - 1; ++col)
							{
								memcpy_s(dst + (ls * row) + (bytePerPixel * col), bytePerPixel, src + (ls * row) + (bytePerPixel * (wd - 1)), bytePerPixel);
							}
						}
					}
				}

#ifdef VL_USE_OPENCL
			}
#endif // VL_USE_OPENCL

			setPixmap(QPixmap::fromImage(m_image));
		}
	}

	//

	QDomElement CPixmap::serializeProps(QDomDocument& document) const
	{
		QDomElement props = document.createElement("properties");

		// property
		QDomElement prop_angle = document.createElement("property");
		prop_angle.setAttribute("name", "rotate_angle");
		prop_angle.setAttribute("value", m_angle);
		props.appendChild(prop_angle);

		// property
		QDomElement prop_divide = document.createElement("property");
		prop_divide.setAttribute("name", "divide");
		prop_divide.setAttribute("value", m_divide);
		props.appendChild(prop_divide);

		// property
		//QDomElement prop_spline_type = document.createElement("property");
		//prop_spline_type.setAttribute("name", "spline_type");
		//prop_spline_type.setAttribute("value", m_curve->propertyValue("spline_type"));
		//props.appendChild(prop_spline_type);

		return props;
	}

	QDomElement CPixmap::serialize(QDomDocument& document, const QString& path)// const
	{
		QFileInfo file(path + "/log.png");

		if (!m_is_saved)
		{
			m_original_pixmap.save(file.filePath(), "PNG");

			m_is_saved = true;
		}

		QDomElement elem = document.createElement("pixmap");

		// properties
		QDomElement props = serializeProps(document);
		elem.appendChild(props);

		// shared properties
		QDomElement shared_props = serializeSharedProps(document);
		elem.appendChild(shared_props);

		// element
		QDomElement elem_curve = m_curve->serialize(document);
		elem.appendChild(elem_curve);

		return elem;
	}

	void CPixmap::deserialize(const QDomElement& element)
	{
		QDomNodeList nodes = element.elementsByTagName("pixmap");

		if (nodes.count())
		{
			QDomElement pixmap_elem = nodes.at(0).toElement();

			deserializeSharedProps(pixmap_elem);

			deserializeProps(pixmap_elem);

			//element
			QDomNodeList curve_nodes = element.elementsByTagName("curve");

			if (curve_nodes.count())
			{
				QDomElement curve_elem = curve_nodes.at(0).toElement();

				directCreateCurve();

				// curve
				m_curve->deserialize(curve_elem);
			}
			else
			{
				createCurve();
			}

			m_curve->sort();
			m_curve->calculate();
			m_curve->updateBoundingRect();
			m_curve->update();

			calculate();

			m_curve->activate();
			m_curve->lock();
			m_curve->hide();

			update();
		}

		m_is_saved = true;
	}
}
