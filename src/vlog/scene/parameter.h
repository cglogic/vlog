#pragma once

#include "../config.h"

#include "../object.h"
#include <QGraphicsRectItem>

#include "range.h"
#include "curve.h"

#include <QGraphicsSceneMouseEvent>
#include <QtXml/QtXml>

namespace General
{
	// Измеренный параметр
	class CParameter
		: public CObject, public QGraphicsRectItem
	{
		Q_OBJECT

	public:
		CParameter(CRange* dp, QGraphicsScene* sc);
		CParameter(const QPointF& psf, const QPointF& psl, CRange* dp, QGraphicsScene* sc);
		virtual ~CParameter();

		void init(const QPointF& psf, const QPointF& psl);

		virtual void activate();
		virtual void deactivate();

		virtual void lock();
		virtual void unlock();

		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

		CRange* getRange();
		CRange* getDepth();

		CCurve* addCurve(const QPointF& ps);
		void removeCurve(CCurve* cv);

		int curveCount() const;
		CCurve* curveAt(int i);

		virtual QVariant propertyValue(const QString& name) const;
		virtual void setPropertyValue(const QString& name, const QVariant& value);

		virtual QDomElement serializeProps(QDomDocument& document) const;

		QDomElement serialize(QDomDocument& document) const;
		void deserialize(const QDomElement& element);

		qreal valueByDepth(qreal depth, qreal null);

		CCurve* directAddCurve();
		void directRemoveCurve(CCurve* cv);

		void sort();

	signals:
		void signalActivated();
		void signalDeactivated();
		void signalDeleted(CObject* ob);

		void signalActivate(CObject* ob);
		bool signalDelete(CParameter* pr);

		QPointF signalMove(CParameter* pr, const QPointF& ps);

		void signalLeftButtonPress(CParameter* pr);
		void signalRightButtonPress(CParameter* pr);

		void signalLeftButtonRelease(CParameter* pr);
		void signalRightButtonRelease(CParameter* pr);

		void signalSetStatusText(const QString& tx);

		void signalPointMove(CParameter* pr);

	public slots:
		void slotRangeActivated();
		void slotRangeDeactivated();

		void slotRangeActivate(CObject* ob);
		bool slotRangeDelete(CRange* rn);
		void slotRangeMove(CRange* rn, const QPointF& ps);

		void slotCurveActivated();
		void slotCurveDeactivated();
		bool slotCurveDelete(CCurve* cv);
		void slotCurvePropertiesUpdated(CObject* object);

		QPointF slotPointMove(CPoint* pt, const QPointF& ps);

		void slotCurveHoverEnter();
		void slotCurveHoverLeave();
		void slotCurveHoverMoved(CCurve* cv, const QPointF& ps);

	protected:
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

	private:
		typedef QList <CCurve*> t_curves;
		typedef t_curves::iterator t_curves_iter;
		typedef t_curves::const_iterator t_curves_const_iter;

		QGraphicsScene *m_scene;

		CRange *m_depth;

		CRange *m_range;

		t_curves m_curves;

		QString m_name;
		QString m_unit;

	};
}
