#pragma once

#include "../config.h"

#include "../interface/export_data.h"

#include "../system/config.h"

#include "pixmap.h"
#include "range.h"
#include "parameter.h"

#include <QList>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsAnchor>
#include <QScrollBar>
#include <QTimerEvent>
#include <QtXml/QtXml>

namespace General
{
	class CScene
		: public QGraphicsScene
	{
		Q_OBJECT

	public:
		CScene(QWidget* parent = 0);
		virtual ~CScene();

		enum EMode
		{
			ModeNone		= 0x00000000,
			ModePixmap		= 0x02000000,
			ModeDepth		= 0x04000000,
			ModeParameter	= 0x08000000,
			ModeCurve		= 0x10000000,
			ModePoint		= 0x20000000
		};

		enum EObject
		{
			ObjectNone,
			ObjectPixmap,
			ObjectDepth,
			ObjectParameter,
			ObjectCurve
		};

		EMode mode() const;
		void setMode(EMode mode, bool enable = true);

		CPixmap* setPixmap(const QPixmap& pixmap);
		// delete
		CPixmap* pixmap();

		CRange* setDepth(const QPointF& ps);
		// delete
		CRange* depth();

		CParameter* addParameter(const QPointF& ps);
		CParameter* directAddParameter(const QPointF& psf, const QPointF& psl);

		// delete
		CParameter* parameter(int n);

		bool exportAction(Interface::CExportData* data, CObject* project);

		void activateObject(CObject* ob);

		CObject* activeObject();

		int parameterCount() const;
		CParameter* parameterAt(int i);

		void globalKeyPressEvent(QKeyEvent* event);
		void globalKeyReleaseEvent(QKeyEvent* event);

		void serialize(QDomDocument& document, const QString& path);
		void deserialize(const QDomElement& element);

	signals:
		void signalObjectActivated(CObject* ob);
		void signalObjectCreated(CObject* ob);
		void signalObjectDeleted(CObject* ob);
		void signalObjectCaptionChanged(CObject* ob);

		void signalSetStatusText(const QString& tx);

	public slots:
		void slotExternalObjectActivation(CObject* ob);
		void slotExternalObjectDeletion(CObject* ob);

		void slotObjectActivate(CObject* ob);
		void slotObjectDeleted(CObject* ob);
		void slotObjectCaptionChanged(CObject* ob);

		void slotCurveDelete(CParameter* pr, CCurve* cv);
		void slotParameterDelete(CParameter* pr);
		void slotDepthDelete(CRange* rn);

		bool slotZoomChange(int delta);

		void slotSetStatusText(const QString& tx);

	protected:
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

		virtual void keyPressEvent(QKeyEvent* event);
		virtual void keyReleaseEvent(QKeyEvent* event);

		virtual void timerEvent(QTimerEvent* event);

	private:
		typedef QList <CParameter*> t_parameters;
		typedef t_parameters::iterator t_parameters_iter;
		typedef t_parameters::const_iterator t_parameters_const_iter;

		EMode m_mode;

		CObject *m_active_object;

		CPixmap *m_pixmap;
		CRange *m_depth;

		t_parameters m_parameters;

		qreal m_border;

		bool m_timer_started;
		int m_timer_id;

		int m_scroll_x;
		int m_scroll_y;

		bool m_alt_pressed;
	};
}
