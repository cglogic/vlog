#include "project_new.h"

namespace General
{
	CProjectNewDialog::CProjectNewDialog()
	{
		setupUi();

		setWindowTitle(tr("VLog"));
		setMinimumWidth(400);

		m_name_edit->setText(tr("Project0001"));
		m_name_edit->setFocus();
		m_name_edit->selectAll();

		m_path_edit->setText(QDir::homePath() + "/vlog");
	}

	CProjectNewDialog::~CProjectNewDialog()
	{
	}

	QString CProjectNewDialog::projectName() const
	{
		return m_name_edit->text();
	}

	void CProjectNewDialog::setProjectName(const QString &name)
	{
		m_name_edit->setText(name);
	}

	QString CProjectNewDialog::projectPath() const
	{
		return m_path_edit->text();
	}

	void CProjectNewDialog::setProjectPath(const QString &path)
	{
		m_path_edit->setText(path);
	}

	void CProjectNewDialog::slotSettingsChanged(const QString& value)
	{
		Q_UNUSED(value);

		m_ok_button->setEnabled(false);
		m_error_label->setText("");

		if (m_name_edit->text().isEmpty())
		{
			m_error_label->setText(tr("Project name must not be empty"));
			m_name_edit->setFocus();

			return;
		}

		if  (m_path_edit->text().isEmpty())
		{
			m_error_label->setText(tr("Path must not be empty"));
			m_path_edit->setFocus();

			return;
		}

		if (!QDir(m_path_edit->text()).exists())
		{
			m_error_label->setText(tr("Path must exists"));
			m_path_edit->setFocus();

			return;
		}

		QString symb = "/:*?\"<>|\\";
		foreach (QChar c, symb)
		{
			if (m_name_edit->text().contains(c))
			{
				m_error_label->setText(tr("Project name must not containts '\\, /, :, *, ?, \", <, >, |' symbols"));
				m_name_edit->setFocus();

				return;
			}
		}

		m_ok_button->setEnabled(true);
	}

	void CProjectNewDialog::slotPath()
	{
		QString dir = CDialog::openDirectory(QObject::tr("Select directory"), QDir::homePath() + "/vlog");
		if (!dir.isEmpty())
		{
			m_path_edit->setText(dir);
		}
	}

	void CProjectNewDialog::slotOk()
	{
		accept();
	}

	void CProjectNewDialog::slotCancel()
	{
		reject();
	}

	void CProjectNewDialog::setupUi()
	{
		m_name_edit = new QLineEdit();
		connect(m_name_edit, SIGNAL(textChanged(const QString&)), this, SLOT(slotSettingsChanged(const QString&)));

		QHBoxLayout *name_layout = new QHBoxLayout();
		name_layout->addWidget(new QLabel(tr("Project name:")));
		name_layout->addWidget(m_name_edit);

		m_path_edit = new QLineEdit();
		connect(m_path_edit, SIGNAL(textChanged(const QString&)), this, SLOT(slotSettingsChanged(const QString&)));

		m_path_button = new QPushButton(tr("..."));
		m_path_button->setFixedWidth(30);
		connect(m_path_button, SIGNAL(clicked()), this, SLOT(slotPath()));

		QHBoxLayout *path_layout = new QHBoxLayout();
		path_layout->addWidget(new QLabel(tr("Path:")));
		path_layout->addWidget(m_path_edit);
		path_layout->addWidget(m_path_button);

		QVBoxLayout *settings_layout = new QVBoxLayout();
		settings_layout->addLayout(name_layout);
		settings_layout->addLayout(path_layout);

		QGroupBox *settings_groupbox = new QGroupBox(tr("Settings"));
		settings_groupbox->setLayout(settings_layout);

		m_ok_button = new QPushButton(tr("Ok"));
		connect(m_ok_button, SIGNAL(clicked()), this, SLOT(slotOk()));

		m_cancel_button = new QPushButton(tr("Cancel"));
		connect(m_cancel_button, SIGNAL(clicked()), this, SLOT(slotCancel()));

		QHBoxLayout *button_layout = new QHBoxLayout();
		button_layout->addStretch();
		button_layout->addWidget(m_ok_button);
		button_layout->addWidget(m_cancel_button);

		QHBoxLayout *title_layout = new QHBoxLayout();
		title_layout->addStretch();
		title_layout->addWidget(new QLabel(tr("<h3>New project</h3>")));
		title_layout->addStretch();

		m_error_label = new QLabel();
		m_error_label->setStyleSheet("QLabel { color : red; font : bold }");

		QHBoxLayout *error_layout = new QHBoxLayout();
		error_layout->addWidget(m_error_label);

		QVBoxLayout *layout = new QVBoxLayout();
		layout->addLayout(title_layout);
		layout->addWidget(settings_groupbox);
		layout->addLayout(error_layout);
		layout->addStretch();
		layout->addLayout(button_layout);

		setLayout(layout);
	}

}
