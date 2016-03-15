#pragma once

#include "../config.h"

#include "../object.h"
#include "point.h"

#include <qmath.h>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QList>
#include <QVector>
#include <QCursor>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QAbstractGraphicsShapeItem>
#include <QtXml/QtXml>

#ifdef _MSC_VER
#	define AE_CPU AE_INTEL
#endif

#ifdef __GNUC__
//#	define __MMX__
//#	define __SSE__
//#	define AE_CPU AE_INTEL
#endif

#include "alglib/interpolation.h"

namespace General
{
	// Кривая
	class CCurve
		: public CObject, public QAbstractGraphicsShapeItem
	{
		Q_OBJECT

	public:
		CCurve(QGraphicsItem* parent, QGraphicsScene* scene);
		CCurve(QGraphicsItem* parent, const QPointF& ps, const QPointF& psf, const QPointF& psl, QGraphicsScene* scene);
		virtual ~CCurve();

		void init(const QPointF& ps);

		virtual void activate();
		virtual void deactivate();

		virtual void lock();
		virtual void unlock();

		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
		virtual QRectF boundingRect() const;

		enum ESort
		{
			SortVertical,
			SortHorizontal
		};

		static CCurve::ESort sortType();
		static void setSortType(CCurve::ESort st);
		void setAproxType(int tp);

		CPoint* addPoint(const QPointF& ps);
		void removePoint(CPoint* pt);
		QPointF movePoint(CPoint* pt, const QPointF& ps);

		int pointCount() const;
		CPoint* pointAt(int i);

		const CPoint* firstPoint() const;
		const CPoint* lastPoint() const;

		bool isPaintingEnabled() const;
		void setPaintingEnable(bool enable);

		enum EType
		{
			TypeAkima		= 0,
			TypeCatmullRom	= 1,
			TypeLinear		= 2
		};

		QGraphicsItem* brushCursor() const;
		void brushRadiusChanded(int delta);

		void cursorPosChanged(const QPointF& pos);

		static void setLoggingImage(const QImage& image);

		void painting(const QPointF& pos);
		void paintingAuto(const QPointF& start);

		void pipette(const QPointF& pos);

		virtual QVariant propertyValue(const QString& name) const;
		virtual void setPropertyValue(const QString& name, const QVariant& value);

		virtual QDomElement serializeProps(QDomDocument& document) const;

		QDomElement serialize(QDomDocument& document) const;
		void deserialize(const QDomElement& element);

		qreal coordByCoord(qreal cd) const;

		CPoint* directAddPoint(const QPointF& ps);
		void directRemovePoint(CPoint* pt);

		void sort();
		void calculate();
		void updateBoundingRect();

	signals:
		void signalActivated();
		void signalDeactivated();
		void signalDeleted(CObject* ob);

		void signalActivate(CObject* ob);
		bool signalDelete(CCurve* cv);

		void signalLeftButtonPress(CCurve* cv);
		void signalRightButtonPress(CCurve* cv);

		void signalLeftButtonRelease(CCurve* cv);
		void signalRightButtonRelease(CCurve* cv);

		QPointF signalPointMove(CPoint* pt, const QPointF& ps);
		bool signalPointDelete(CPoint* pt);

		QPointF signalPointMoved(CPoint* pt, const QPointF& ps);

		void signalPointCreated(CPoint* pt);
		void signalPointDeleted(CPoint* pt);

		void signalHoverEnter();
		void signalHoverLeave();
		void signalHoverMoved(CCurve* cv, const QPointF& ps);

		void signalPointPressed(CPoint* pt);
		void signalPointReleased(CPoint* pt);

	public slots:
		QPointF slotPointMove(CPoint* pt, const QPointF& ps);
		QPointF slotPointMoved(CPoint* pt, const QPointF& ps);

		bool slotPointDelete(CPoint* pt);
		void slotPointDeleted(CObject* ob);

		void slotPointLeftDoubleClick(CPoint* pt);

		void slotImageRotate(qreal angle);

	protected:
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);

	private:
		typedef QList <CPoint*> t_points;
		typedef t_points::iterator t_points_iter;
		typedef t_points::const_iterator t_points_const_iter;

		static ESort m_sort;

		QGraphicsScene *m_scene;

		QRectF m_rect;

		alglib::spline1dinterpolant m_interpolant;

		// Range
		t_points m_points;

		//
		QPainterPath m_path;

		EType m_spline_type;
		double m_draw_step;
		double m_range_start;
		double m_range_stop;

		bool m_painting;
		QCursor m_old_cursor;
		QCursor m_blank_cursor;

		QGraphicsEllipseItem *m_brush_cursor;

		int m_paint_target;
		int m_target_value;
		double m_target_coeff;
		mutable QColor m_target_color;
		int m_target_scatter;

		double m_brush_radius;
		double m_line_bias;
		double m_point_gap;

		QPointF m_last_cursor_pos;
		static const QImage *m_logging_image;
		qreal m_image_angle;

		bool passImagePixel(QPointF pf) const;
		void removeInLinePoints();

		bool pointInLine(const QPointF& p0, const QPointF& p1, const QPointF& p) const;

	};
}
