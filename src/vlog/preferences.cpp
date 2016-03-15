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

#include "preferences.h"

namespace General
{
	CPreferencesDialog::CPreferencesDialog()
	{
		setupUi();

		setWindowTitle(tr("Preferences"));
		setMinimumSize(300, 350);
	}

	CPreferencesDialog::~CPreferencesDialog()
	{
	}

	QString CPreferencesDialog::getLanguage() const
	{
		return m_language_combobox->currentText();
	}

	void CPreferencesDialog::setLanguage(const QString& language)
	{
		for (int i = 0; i < m_language_combobox->count(); ++i)
		{
			if (m_language_combobox->itemText(i) == language)
			{
				m_language_combobox->setCurrentIndex(i);

				break;
			}
		}
	}

	bool CPreferencesDialog::isAutosave() const
	{
		return m_autosave_checkbox->isChecked();
	}

	void CPreferencesDialog::setAutosave(bool enable)
	{
		m_autosave_checkbox->setChecked(enable);
	}

	int CPreferencesDialog::getAutosaveTime() const
	{
		return m_autosave_spinbox->value();
	}

	void CPreferencesDialog::setAutosaveTime(int time)
	{
		m_autosave_spinbox->setValue(time);
	}

	void CPreferencesDialog::slotOk()
	{
		accept();
	}

	void CPreferencesDialog::slotCancel()
	{
		reject();
	}

	void CPreferencesDialog::slotAutosaveToggled(bool value)
	{
		m_autosave_spinbox->setEnabled(value);
	}

	QWidget* CPreferencesDialog::createInterfaceWidget()
	{
		m_language_combobox = new QComboBox();
		m_language_combobox->setFixedWidth(150);
		m_language_combobox->addItem(tr("English"));
		m_language_combobox->addItem(tr("Russian"));
		//m_language_combobox->addItem(tr("Ukrainian"));

		QHBoxLayout *language_layout = new QHBoxLayout();
		language_layout->addWidget(new QLabel(tr("Language:")));
		language_layout->addWidget(m_language_combobox);
		language_layout->addStretch();

		QFormLayout *general_layout = new QFormLayout();
		general_layout->addRow(language_layout);

		QGroupBox *general_groupbox = new QGroupBox(tr("Appearance"));
		general_groupbox->setLayout(general_layout);

		QVBoxLayout *layout = new QVBoxLayout();
		layout->addWidget(general_groupbox);
		layout->addStretch();

		QWidget *widget = new QWidget();
		widget->setLayout(layout);

		return widget;
	}

	QWidget* CPreferencesDialog::createProjectsWidget()
	{
		m_autosave_checkbox = new QCheckBox(tr("Enable project autosave every"));
		connect(m_autosave_checkbox, SIGNAL(toggled(bool)), this, SLOT(slotAutosaveToggled(bool)));

		m_autosave_spinbox = new QSpinBox();
		m_autosave_spinbox->setMinimum(1);
		m_autosave_spinbox->setMaximum(60);
		m_autosave_spinbox->setValue(5);
		m_autosave_spinbox->setEnabled(false);

		QHBoxLayout *autosave_layout = new QHBoxLayout();
		autosave_layout->addWidget(m_autosave_checkbox);
		autosave_layout->addWidget(m_autosave_spinbox);
		autosave_layout->addWidget(new QLabel(tr("min.")));
		autosave_layout->addStretch();

		QFormLayout *general_layout = new QFormLayout();
		general_layout->addRow(autosave_layout);

		QGroupBox *general_groupbox = new QGroupBox(tr("General"));
		general_groupbox->setLayout(general_layout);

		QVBoxLayout *layout = new QVBoxLayout();
		layout->addWidget(general_groupbox);
		layout->addStretch();

		QWidget *widget = new QWidget();
		widget->setLayout(layout);

		return widget;
	}

	void CPreferencesDialog::setupUi()
	{
		m_ok_button = new QPushButton(tr("Ok"), this);
		connect(m_ok_button, SIGNAL(clicked()), this, SLOT(slotOk()));

		m_cancel_button = new QPushButton(tr("Cancel"), this);
		connect(m_cancel_button, SIGNAL(clicked()), this, SLOT(slotCancel()));

		QHBoxLayout *button_layout = new QHBoxLayout();
		button_layout->addStretch();
		button_layout->addWidget(m_ok_button);
		button_layout->addWidget(m_cancel_button);

		m_tab_widget = new QTabWidget();
		m_tab_widget->addTab(createInterfaceWidget(), tr("Interface"));
		m_tab_widget->addTab(createProjectsWidget(), tr("Projects"));

		QVBoxLayout *layout = new QVBoxLayout();
		layout->addWidget(m_tab_widget);
		layout->addLayout(button_layout);

		setLayout(layout);
	}
}
