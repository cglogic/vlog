/****************************************************************************
 * Copyright (C) 2012-2013 Ievgen Polyvanyi
 * Copyright (C) 2012-2016 Oleg Gushchenkov
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
****************************************************************************/

#include "view.h"

#include <qmath.h>

#include <QScrollBar>

//#include <QGLWidget>

namespace General
{
	CView::CView(QWidget* parent)
		: QGraphicsView(parent), m_middle_pressed(false), m_control_pressed(false)
	{
		setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
		//setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
		//setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	}

	CView::~CView()
	{
	}

	//

	void CView::wheelEvent(QWheelEvent *event)
	{
		if (m_control_pressed)
		{
			bool not_locked = emit signalZoomChange(event->delta());

			if (not_locked)
			{
				qreal scale_factor = qPow(1.6, event->delta() / 240.0);
				qreal factor = transform().scale(scale_factor, scale_factor).mapRect(QRectF(0.0, 0.0, 1.0, 1.0)).width();

				if (factor > 0.1 && factor < 100.0)
				{
					setTransform(QTransform::fromScale(scale_factor, scale_factor), true);
				}
			}
		}
		else
		{
			QScrollBar *hbar = horizontalScrollBar();
			QScrollBar *vbar = verticalScrollBar();

			QRectF rc = scene()->sceneRect();

			if (rc.width() >= rc.height())
			{
				hbar->setValue(hbar->value() - 2 * event->delta());
			}
			else
			{
				vbar->setValue(vbar->value() - 2 * event->delta());
			}
		}

		event->accept();
	}

	void CView::mouseMoveEvent(QMouseEvent* event)
	{
		if (m_middle_pressed)
		{
			QScrollBar *hbar = horizontalScrollBar();
			QScrollBar *vbar = verticalScrollBar();

			delta = event->pos() - delta;

			hbar->setValue(hbar->value() - delta.x());
			vbar->setValue(vbar->value() - delta.y());

			delta = event->pos();
		}

		QGraphicsView::mouseMoveEvent(event);
	}

	void CView::mousePressEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::MiddleButton)
		{
			m_middle_pressed = true;

			delta = event->pos();

			m_old_cursor = viewport()->cursor();

			viewport()->setCursor(Qt::SizeAllCursor);
		}

		QGraphicsView::mousePressEvent(event);
	}

	void CView::mouseReleaseEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::MiddleButton)
		{
			m_middle_pressed = false;

			delta = QPoint();

			viewport()->setCursor(m_old_cursor);
		}

		QGraphicsView::mouseReleaseEvent(event);
	}

	void CView::keyPressEvent(QKeyEvent* event)
	{
		if (event->modifiers() & Qt::ControlModifier)
		{
			m_control_pressed = true;
		}

		QGraphicsView::keyPressEvent(event);
	}

	void CView::keyReleaseEvent(QKeyEvent* event)
	{
		if (event->modifiers() | ~Qt::ControlModifier)
		{
			m_control_pressed = false;
		}

		QGraphicsView::keyReleaseEvent(event);
	}
}
