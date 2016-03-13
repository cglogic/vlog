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
