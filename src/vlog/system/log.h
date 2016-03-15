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

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

namespace General
{
	class CLog
	{
	public:
		enum EMessageType
		{
			MT_INFO,
			MT_WARN,
			MT_ERR
		};

		CLog(const QString& filename);
		~CLog();

		static CLog* getInstance();
		void freeInstance();

		void information(const QString& message);
		void warning(const QString& message);
		void error(const QString& message);

	private:
		CLog(const CLog& log);
		const CLog& operator= (const CLog& rhs);

		QString m_filename;
		QFile *m_file;
		QTextStream *m_stream;

		void create();
		void destroy();

		QString getTimeStamp();

		void writeHeader();
		void writeFooter();
		void writeMessage(EMessageType type, const QString& message);

	};
}
