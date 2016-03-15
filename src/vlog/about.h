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

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

namespace General
{
	class CAboutDialog
		: public QDialog
	{
		Q_OBJECT

	public:
		CAboutDialog(void);
		virtual ~CAboutDialog(void);

	protected:
		void setupUi(void);
		void retranslateUi(void);

	private:
		QHBoxLayout *horizontalLayout;
		QLabel *label_Image;
		QFrame *frame_Main;
		QVBoxLayout *verticalLayout;
		QTextBrowser *textBrowser;
		QFrame *frame_Button;
		QHBoxLayout *horizontalLayout_2;
		QFrame *frame_Space;
		QPushButton *pushButton_Ok;

	};
}
