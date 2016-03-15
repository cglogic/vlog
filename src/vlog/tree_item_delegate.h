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
