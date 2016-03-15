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

#include "tree_item_delegate.h"

#include <QTreeWidget>
#include <QHeaderView>
#include <QMouseEvent>

namespace General
{
	class CTree
		: public QTreeWidget
	{
		Q_OBJECT

	public:
		CTree();
		virtual ~CTree();

	signals:
		void signalItemSelect(QTreeWidgetItem* item);
		void signalItemDelete(QTreeWidgetItem* item);

	private slots:
		void slotItemPressed(QModelIndex index);
		void slotItemClicked(QModelIndex index);

	private:
		CTreeItemDelegate *m_item_delegate;

	};
}
