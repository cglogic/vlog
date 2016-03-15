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

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QTabWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

namespace General
{
	class CPreferencesDialog
		: public QDialog
	{
		Q_OBJECT

	public:
		CPreferencesDialog();
		virtual ~CPreferencesDialog();

		QString getLanguage() const;
		void setLanguage(const QString& language);

		bool isAutosave() const;
		void setAutosave(bool enable);

		int getAutosaveTime() const;
		void setAutosaveTime(int time);

	private slots:
		void slotOk();
		void slotCancel();

		void slotAutosaveToggled(bool value);

	private:
		QComboBox* m_language_combobox;
		QWidget* createInterfaceWidget();

		QCheckBox *m_autosave_checkbox;
		QSpinBox *m_autosave_spinbox;
		QWidget* createProjectsWidget();

		QTabWidget *m_tab_widget;

		QPushButton *m_ok_button;
		QPushButton *m_cancel_button;

		void setupUi();

	};
}
