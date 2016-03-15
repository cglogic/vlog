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
