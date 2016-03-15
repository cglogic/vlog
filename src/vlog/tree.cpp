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

#include "tree.h"

namespace General
{
	CTree::CTree()
	{
		m_item_delegate = new CTreeItemDelegate(this);
		setItemDelegate(m_item_delegate);

		setColumnCount(2);
		setUniformRowHeights(true);

		setSelectionMode(QAbstractItemView::SingleSelection);
		setSelectionBehavior(QAbstractItemView::SelectRows);

		header()->hide();
		header()->setStretchLastSection(false);
		header()->setResizeMode(0, QHeaderView::Stretch);
		header()->setResizeMode(1, QHeaderView::Fixed);
		header()->resizeSection(1, 18);

		viewport()->setAttribute(Qt::WA_Hover);

		connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(slotItemPressed(QModelIndex)));
		connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(slotItemClicked(QModelIndex)));
	}

	CTree::~CTree()
	{
	}

	void CTree::slotItemPressed(QModelIndex index)
	{
		if (index.column() == 0)
		{
			selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

			emit signalItemSelect(itemFromIndex(index));
		}
		else
		{
			if (index.column() == 1)
			{
				m_item_delegate->m_pressed_index = index;
			}
		}
	}

	void CTree::slotItemClicked(QModelIndex index)
	{
		if (index.column() == 1)
		{
			emit signalItemDelete(itemFromIndex(index));

			// work around a bug in itemviews where the delegate wouldn't get the QStyle::State_MouseOver
			QPoint curs_pos = QCursor::pos();
			QMouseEvent e(QEvent::MouseMove, viewport()->mapFromGlobal(curs_pos), curs_pos, Qt::NoButton, 0, 0);
			QCoreApplication::sendEvent(viewport(), &e);
		}
	}
}
