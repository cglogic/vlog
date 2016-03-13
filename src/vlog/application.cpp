#include "application.h"

namespace General
{
	CApplication::CApplication(int argc, char** argv)
		: m_native_app(argc, argv)
	{
		QApplication::setOrganizationName("KSoft");
		QApplication::setApplicationName("VLog");
		QApplication::setApplicationVersion("0.2.0");
	}

	CApplication::~CApplication()
	{
	}

	int CApplication::run()
	{
		QTextCodec* tc = QTextCodec::codecForName("UTF-8");
		QTextCodec::setCodecForTr(tc);
		QTextCodec::setCodecForCStrings(tc);
		QTextCodec::setCodecForLocale(tc);

		if (!CConfig::getInstance()->load())
		{
			QFile config(":files/config.xml");

			if (!CConfig::getInstance()->load(&config))
			{
				CLog::getInstance()->error("Configuration not loaded");
				CDialog::error("Configuration not loaded");

				return -1;
			}

			CConfig::getInstance()->save();

			CLog::getInstance()->warning("Can't load config.xml. Configuration load by default");
		}

		QString language = CConfig::getInstance()->getSection("interface")->getParamString("language").toLower();
		m_translator.load("languages/" + language + ".qm");
		m_native_app.installTranslator(&m_translator);

		CLog::getInstance()->information(QObject::tr("Configuration loaded"));

		CWindow window;
		window.show();

		int ret_code = m_native_app.exec();

		CConfig::getInstance()->freeInstance();
		CLog::getInstance()->freeInstance();

		return ret_code;
	}
}
