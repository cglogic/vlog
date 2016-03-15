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

#include "tree_item_delegate.h"

namespace General
{
	CTreeItemDelegate::CTreeItemDelegate(QObject* parent)
		: QStyledItemDelegate(parent)
	{
	}

	CTreeItemDelegate::~CTreeItemDelegate()
	{
	}

	void CTreeItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		if (option.state & QStyle::State_MouseOver)
		{
			if ((QApplication::mouseButtons() & Qt::LeftButton) == 0)
			{
				m_pressed_index = QModelIndex();
			}

			QBrush brush = option.palette.alternateBase();

			if (index == m_pressed_index)
			{
				brush = option.palette.dark();
			}

			painter->fillRect(option.rect, brush);
		}

		QStyledItemDelegate::paint(painter, option, index);

		if (index.column() == 1 && option.state & QStyle::State_MouseOver)
		{
			QIcon icon = QIcon(":/icons/cross_small.png");

			QRect rc(option.rect.right() - option.rect.height(), option.rect.top(),
					 option.rect.height(), option.rect.height());

			icon.paint(painter, rc, Qt::AlignRight | Qt::AlignVCenter);
		}
	}
}
