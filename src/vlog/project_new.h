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

#include "system/dialog.h"

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDir>

namespace General
{
	class CProjectNewDialog
		: public QDialog
	{
		Q_OBJECT

	public:
		CProjectNewDialog();
		virtual ~CProjectNewDialog();

		QString projectName() const;
		void setProjectName(const QString& name);

		QString projectPath() const;
		void setProjectPath(const QString& path);

	private slots:
		void slotSettingsChanged(const QString& value);
		void slotPath();
		void slotOk();
		void slotCancel();

	private:
		QLineEdit *m_name_edit;
		QLineEdit *m_path_edit;
		QLabel *m_error_label;
		QPushButton *m_path_button;
		QPushButton *m_ok_button;
		QPushButton *m_cancel_button;
		void setupUi();

	};
}
