#include "preview.h"

/*#include <QDebug>

namespace General
{
	CPreview::CPreview(QWidget* parent)
		: QGraphicsView(parent), m_offset(50.0), m_param(nullptr)
	{
		setupUi();
	}

	CPreview::~CPreview()
	{
	}

	void CPreview::wheelEvent(QWheelEvent* event)
	{
		qreal scale_factor = qPow(1.6, event->delta() / 240.0);
		qreal factor = this->transform().scale(scale_factor, scale_factor).mapRect(QRectF(0.0, 0.0, 1.0, 1.0)).width();

		if (factor > 0.1 && factor < 100.0)
		{
			this->setTransform(QTransform::fromScale(scale_factor, scale_factor), true);
		}
	}

	void CPreview::setupUi()
	{
		m_scene = new QGraphicsScene();

		this->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
		this->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
		this->setScene(m_scene);
		this->resetTransform();
		this->setEnabled(false);
		this->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
	}

	void CPreview::prepeareParam(CParameter* param)
	{
		m_scene->clear();

		m_param_start = int(0x0fffffff);
		m_param_stop = int(0xf0000000);

		bool curve_exists = false;

		for (int i = 0; i < param->curveCount(); ++i)
		{
			CCurve *curve = param->curveAt(i);

			m_param_start = qMin(m_param_start, curve->propertyValue("range_start").toDouble());
			m_param_stop = qMax(m_param_stop, curve->propertyValue("range_stop").toDouble());

			tryConnection(curve);

			curve_exists = true;
		}

		if (curve_exists)
		{
			drawParam(param, m_null, m_depth_start, m_depth_stop, m_depth_step, m_param_start, m_param_stop);
		}
	}

	void CPreview::drawParam(CParameter* param, double null, double depth_start, double depth_stop, double depth_step, double param_start, double param_stop)
	{
		depth_step = 0.1;

		QPainterPath path;

		double curr = depth_start;
		double value = null;
		double last_value = null;

		while (curr < depth_stop)
		{
			value = param->valueByDepth(curr, null);

			double x = (curr - depth_start) * (m_scene->width() - m_offset) / (depth_stop - depth_start) + m_offset / 2.0;
			double y = (value - param_stop) * (m_scene->height()) / (param_start - param_stop);

			if (value != null && last_value == null)
			{
				m_scene->addPath(path);

				path = QPainterPath();

				path.moveTo(x, y);
			}
			else if (value != null)
			{
				path.lineTo(x, y);
			}

			last_value = value;

			curr += depth_step;
		}

		m_scene->addPath(path, QPen(QColor(0, 0, 0)));
	}

	void CPreview::clearAll()
	{
		this->resetTransform();

		m_scene->clear();

		this->setEnabled(false);

		if (m_param)
		{
			disconnect(m_param->getRange(), SIGNAL(signalMove(CRange*, const QPointF&)), this, SLOT(slotRangePointMove(CRange*, const QPointF&)));
		}

		m_param = nullptr;

		clearConnections();
	}

	void CPreview::tryConnection(CCurve* curve)
	{
		for (int i = 0; i < m_curve_connections.size(); ++i)
		{
			if (m_curve_connections.at(i) == curve)
			{
				return;
			}
		}

		connect(curve, SIGNAL(signalPointCreated(CPoint*)), this, SLOT(slotPointCreated(CPoint*)));
		connect(curve, SIGNAL(signalPointDeleted(CPoint*)), this, SLOT(slotPointDeleted(CPoint*)));
		connect(curve, SIGNAL(signalPointReleased(CPoint*)), this, SLOT(slotPointMoved(CPoint*)));

		connect(curve, SIGNAL(signalPropertiesUpdated(CObject*)), this, SLOT(slotCurvePropertiesUpdated(CObject*)));
		connect(curve, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotCurveDeleted(CObject*)));

		m_curve_connections.push_back(curve);
	}

	void CPreview::clearConnections()
	{
		for (int i = 0; i < m_curve_connections.size(); ++i)
		{
			disconnect(m_curve_connections.at(i), SIGNAL(signalPointCreated(CPoint*)), this, SLOT(slotPointCreated(CPoint*)));
			disconnect(m_curve_connections.at(i), SIGNAL(signalPointDeleted(CPoint*)), this, SLOT(slotPointDeleted(CPoint*)));
			disconnect(m_curve_connections.at(i), SIGNAL(signalPointReleased(CPoint*)), this, SLOT(slotPointMoved(CPoint*)));

			disconnect(m_curve_connections.at(i), SIGNAL(signalPropertiesUpdated(CObject*)), this, SLOT(slotCurvePropertiesUpdated(CObject*)));
			disconnect(m_curve_connections.at(i), SIGNAL(signalDeleted(CObject*)), this, SLOT(slotCurveDeleted(CObject*)));
		}

		m_curve_connections.clear();
	}

	void CPreview::slotObjectSelected(CObject* object)
	{
		CParameter *param = qobject_cast<CParameter*>(object);

		if (!param)
		{
			CCurve *curve = qobject_cast<CCurve*>(object);

			if (!curve)
			{
				clearAll();

				return;
			}

			param = qobject_cast<CParameter*>(curve->fantomParent());

			if (!param)
			{
				clearAll();

				return;
			}
		}

		CRange *depth = param->getDepth();

		if (!depth)
		{
			clearAll();

			return;
		}

		if (m_param == param)
		{
			prepeareParam(param);

			return;
		}

		clearAll();

		connect(param->getRange(), SIGNAL(signalMove(CRange*, const QPointF&)), this, SLOT(slotRangePointMove(CRange*, const QPointF&)));

		m_depth_start = depth->propertyValue("start").toDouble();
		m_depth_stop = depth->propertyValue("stop").toDouble();

		m_null = depth->propertyValue("digit_null").toDouble();
		m_depth_step = depth->propertyValue("digit_step").toDouble();

		m_scene->clear();

		QRectF scene_rect;

		if (param->getDepth()->direction() == CRange::DirectionHorizontal)
		{
			scene_rect = QRectF(0.0, 0.0, qAbs(depth->secondPointPos().x() - depth->firstPointPos().x() + m_offset), qAbs(param->getRange()->secondPointPos().y() - param->getRange()->firstPointPos().y() + m_offset));
		}
		else
		{
			scene_rect = QRectF(0.0, 0.0, qAbs(depth->secondPointPos().y() - depth->firstPointPos().y() + m_offset), qAbs(param->getRange()->secondPointPos().x() - param->getRange()->firstPointPos().x() + m_offset));
		}

		m_scene->setSceneRect(scene_rect);

		this->setEnabled(true);

		m_param = param;

		prepeareParam(param);
	}

	void CPreview::slotPointMove(CParameter* param)
	{
		Q_UNUSED(param);

		prepeareParam(m_param);
	}

	void CPreview::slotPointMoved(CPoint* pt)
	{
		Q_UNUSED(pt);

		prepeareParam(m_param);
	}

	void CPreview::slotPointCreated(CPoint* pt)
	{
		Q_UNUSED(pt);

		prepeareParam(m_param);
	}

	void CPreview::slotPointDeleted(CPoint* pt)
	{
		Q_UNUSED(pt);

		prepeareParam(m_param);
	}

	void CPreview::slotCurvePropertiesUpdated(CObject* object)
	{
		Q_UNUSED(object);

		prepeareParam(m_param);
	}

	void CPreview::slotCurveDeleted(CObject* object)
	{
		Q_UNUSED(object);

		clearConnections();

		prepeareParam(m_param);
	}

	void CPreview::slotRangePointMove(CRange* rn, const QPointF& ps)
	{
		Q_UNUSED(rn);
		Q_UNUSED(ps);

		CRange *depth = m_param->getDepth();

		if (!depth)
		{
			return;
		}

		QRectF scene_rect;

		if (m_param->getDepth()->direction() == CRange::DirectionHorizontal)
		{
			scene_rect = QRectF(0.0, 0.0, qAbs(depth->secondPointPos().x() - depth->firstPointPos().x() + m_offset), qAbs(m_param->getRange()->secondPointPos().y() - m_param->getRange()->firstPointPos().y() + m_offset));
		}
		else
		{
			scene_rect = QRectF(0.0, 0.0, qAbs(depth->secondPointPos().y() - depth->firstPointPos().y() + m_offset), qAbs(m_param->getRange()->secondPointPos().x() - m_param->getRange()->firstPointPos().x() + m_offset));
		}

		m_scene->setSceneRect(scene_rect);
	}
}*/
