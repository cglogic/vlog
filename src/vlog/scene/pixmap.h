﻿#pragma once

#include "../config.h"

#include "../object.h"
#include <QGraphicsPixmapItem>

#include <QGraphicsSceneMouseEvent>

#include "point.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include <QGraphicsScene>
#include "curve.h"

#ifdef VL_USE_OPENCL
#include "focl/focl.h"
#endif // VL_USE_OPENCL

namespace General
{
	// Отсканированный каротаж
	class CPixmap
		: public CObject, public QGraphicsPixmapItem
	{
		Q_OBJECT

	public:
		CPixmap(QGraphicsScene* sc);
		CPixmap(const QPixmap& pm, QGraphicsScene* sc);
		virtual ~CPixmap();

		void init(const QPixmap& pm);
		void calculate();

		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

		virtual void activate();
		virtual void deactivate();

		virtual void lock();
		virtual void unlock();

		qreal rotateAngle() const;

		const QImage& image();

		CCurve* curve();
		void curveActivate(bool act);
		CPoint* addPoint(const QPointF& ps);

		virtual QVariant propertyValue(const QString& name) const;
		virtual void setPropertyValue(const QString& name, const QVariant& value);

		virtual QDomElement serializeProps(QDomDocument& document) const;

		QDomElement serialize(QDomDocument& document, const QString& path)/* const*/;
		void deserialize(const QDomElement& element);

	signals:
		void signalActivated();
		void signalDeactivated();
		void signalDeleted(CObject* ob);

		void signalActivate(CObject* ob);
		bool signalDelete(CPixmap* pm);

		QPointF signalMove(CPixmap* pm, const QPointF& ps);

		bool signalLeftButtonPress(CPixmap* pm);
		bool signalRightButtonPress(CPixmap* pm);

		bool signalLeftButtonRelease(CPixmap* pm);
		bool signalRightButtonRelease(CPixmap* pm);

		bool signalRotate(qreal angle);

	public slots:
		QPointF slotPointMove(CPoint *pt, const QPointF &ps);

		void slotPointDeleted(CPoint* pt);

		QPointF slotPointMoved(CPoint *pt, const QPointF &ps);
		void slotPointPressed(CPoint* pt);
		void slotPointReleased(CPoint* pt);

		bool slotCurveDelete(CCurve* cv);

	protected:
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

		void directCreateCurve();
		void createCurve();
		void removeCurve();

	private:
		QGraphicsScene *m_scene;
		QRectF m_rect;

		QPixmap m_original_pixmap;
		QImage m_original_image;
		QImage m_image;
		CCurve *m_curve;

		//
		QPainterPath m_path;

		//
		double m_angle;
		int m_divide;

		bool m_is_saved;

		bool m_opencl_enable;

#ifdef VL_USE_OPENCL
		COpenCLFramework m_opencl;
#endif // VL_USE_OPENCL

		int *m_coords;

	};
}
