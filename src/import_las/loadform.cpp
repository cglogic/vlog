#include "loadform.h"

namespace General
{
	CLoadForm::CLoadForm()
	{
		setupUi();
	}

	CLoadForm::~CLoadForm()
	{
	}

	int CLoadForm::pixmapWidth()
	{
		return m_width->value();
	}

	int CLoadForm::pixmapHeight()
	{
		return m_height->value();
	}

	QString CLoadForm::filename()
	{
		return m_filename->text();
	}

	void CLoadForm::setupUi()
	{
		m_width = new QSpinBox();
		m_width->setRange(1, 100000);
		m_width->setValue(1500);

		m_height = new QSpinBox();
		m_height->setRange(1, 100000);
		m_height->setValue(40000);

		m_filename = new QLineEdit();

		connect(m_filename, SIGNAL(returnPressed()), this, SLOT(slotOpenFile()));

		QFormLayout *form_layout = new QFormLayout();

		form_layout->addRow(new QLabel("Pixmap width:"), m_width);
		form_layout->addRow(new QLabel("Pixmap height:"), m_height);
		form_layout->addRow(new QLabel("File:"), m_filename);

		QPushButton *ok_button = new QPushButton(tr("Ok"));
		QPushButton *cancel_button = new QPushButton(tr("Cancel"));

		connect(ok_button, SIGNAL(clicked()), this, SLOT(accept()));
		connect(cancel_button, SIGNAL(clicked()), this, SLOT(reject()));

		QHBoxLayout *button_layout = new QHBoxLayout();

		button_layout->addStretch();
		button_layout->addWidget(ok_button);
		button_layout->addWidget(cancel_button);

		QVBoxLayout *layout = new QVBoxLayout();

		layout->addLayout(form_layout);
		layout->addLayout(button_layout);

		setLayout(layout);
	}

	void CLoadForm::slotOpenFile()
	{
		QString fname = QFileDialog::getOpenFileName(0, tr("Open LAS"), QDir::currentPath(), "LAS file (*.las)");

		if (!fname.isEmpty())
		{
			m_filename->setText(fname);
		}
	}
}
