#pragma once

#include "config.h"

#include "window.h"
#include "system/log.h"
#include "system/config.h"
#include "system/dialog.h"

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QStyleFactory>

namespace General
{
	// Приложение
	class CApplication
	{
	public:
		CApplication(int argc, char** argv);
		~CApplication();

		// entry point
		int run();

	private:
		// disable copying
		CApplication(const CApplication& app);
		CApplication& operator=(const CApplication& rhs);

		QTranslator m_translator;
		QApplication m_native_app;

	};
}
