#pragma once

#include "config.h"

#include <QGraphicsView>

#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>

namespace General
{
	class CView
		: public QGraphicsView
	{
		Q_OBJECT

	public:
		CView(QWidget* parent = 0);
		virtual ~CView();

	signals:
		bool signalZoomChange(int delta);

	protected:
		virtual void wheelEvent(QWheelEvent* event);

		virtual void mouseMoveEvent(QMouseEvent* event);
		virtual void mousePressEvent(QMouseEvent* event);
		virtual void mouseReleaseEvent(QMouseEvent* event);

		virtual void keyPressEvent(QKeyEvent* event);
		virtual void keyReleaseEvent(QKeyEvent* event);

	private:
		bool m_middle_pressed;
		QPoint delta;

		QCursor m_old_cursor;

		bool m_control_pressed;

	};
}
