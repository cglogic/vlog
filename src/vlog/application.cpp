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
