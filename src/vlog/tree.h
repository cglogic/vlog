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
