#pragma once

#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QDialog>
#include <QFileDialog>

namespace General
{
	class CLoadForm
		: public QDialog
	{
		Q_OBJECT

	public:
		CLoadForm();
		virtual ~CLoadForm();

		int pixmapWidth();
		int pixmapHeight();
		QString filename();

	private:
		QSpinBox *m_width;
		QSpinBox *m_height;
		QLineEdit *m_filename;

		void setupUi();

	private slots:
		void slotOpenFile();

	};
}
