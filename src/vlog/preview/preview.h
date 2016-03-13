#pragma once

/*#include "../config.h"

#include "../object.h"
#include "../scene/parameter.h"
#include "../scene/range.h"
#include "../scene/curve.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QSplitter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QEvent>
#include <QWheelEvent>
#include <QFont>
#include <QFontMetrics>
#include <QScrollBar>

namespace General
{
	class CPreview
		: public QGraphicsView
	{
		Q_OBJECT

	public:
		explicit CPreview(QWidget* parent = 0);
		virtual ~CPreview();

	protected:
		virtual void wheelEvent(QWheelEvent* event);

	private:
		QGraphicsScene *m_scene;

		QScrollBar *m_param_hscroll;
		QScrollBar *m_param_vscroll;

		void setupUi();

		double m_offset;

		double m_depth_start;
		double m_depth_stop;

		double m_null;
		double m_depth_step;

		double m_param_start;
		double m_param_stop;

		CParameter *m_param;

		void prepeareParam(CParameter* param);
		void drawParam(CParameter* param, double null, double depth_start, double depth_stop, double depth_step, double param_start, double param_stop);

		void clearAll();

		QVector<CCurve*> m_curve_connections;

		void tryConnection(CCurve *curve);
		void clearConnections();

	public slots:
		void slotObjectSelected(CObject* object);

		void slotPointMove(CParameter* param);
		void slotPointMoved(CPoint* pt);
		void slotPointCreated(CPoint* pt);
		void slotPointDeleted(CPoint* pt);

		void slotCurvePropertiesUpdated(CObject* object);
		void slotCurveDeleted(CObject* object);

		void slotRangePointMove(CRange* rn, const QPointF& ps);

	};
}*/
