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
