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
