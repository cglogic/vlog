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
