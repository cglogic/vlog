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

#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>

namespace General
{
	class CTreeItemDelegate
		: public QStyledItemDelegate
	{
		Q_OBJECT

	public:
		explicit CTreeItemDelegate(QObject* parent = 0);
		virtual ~CTreeItemDelegate();

		void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

		mutable QModelIndex m_pressed_index;

	};
}
