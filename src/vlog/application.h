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
