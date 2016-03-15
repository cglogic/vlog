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

#include "log.h"

namespace General
{
	CLog::CLog(const QString& filename)
		: m_filename(filename), m_file(0), m_stream(0)
	{
		create();
	}

	CLog::~CLog()
	{
		destroy();
	}

	CLog* CLog::getInstance()
	{
		static CLog* instance = new CLog("application.log");

		return instance;
	}

	void CLog::freeInstance()
	{
		delete this;
	}

	void CLog::information(const QString& message)
	{
		writeMessage(MT_INFO, message);

		m_file->flush();
	}

	void CLog::warning(const QString& message)
	{
		writeMessage(MT_WARN, message);

		m_file->flush();
	}

	void CLog::error(const QString& message)
	{
		writeMessage(MT_ERR, message);

		m_file->flush();
	}

	void CLog::create()
	{
		m_file = new QFile(m_filename);
		if (!m_file->open(QIODevice::WriteOnly | QIODevice::Text))
		{
			return;
		}

		m_stream = new QTextStream(m_file);
		m_stream->setCodec("UTF-8");

		writeHeader();
	}

	void CLog::destroy()
	{
		writeFooter();

		m_file->close();

		delete m_stream;
		delete m_file;
	}

	QString CLog::getTimeStamp()
	{
		QDateTime dt = QDateTime::currentDateTime();

		return dt.toString("yyyy/MM/dd hh:mm:ss");
	}

	void CLog::writeHeader()
	{
		writeMessage(MT_INFO, QObject::tr("α"));

		m_stream->flush();
		m_file->flush();
	}

	void CLog::writeFooter()
	{
		writeMessage(MT_INFO, QObject::tr("Ω"));

		m_stream->flush();
		m_file->flush();
	}

	void CLog::writeMessage(EMessageType type, const QString& message)
	{
		(*m_stream) << getTimeStamp() << "  ";

		switch (type)
		{
		case MT_INFO:
			(*m_stream) << QObject::tr("Info       ");
			break;

		case MT_WARN:
			(*m_stream) << QObject::tr("Warning    ");
			break;

		case MT_ERR:
			(*m_stream) << QObject::tr("Error      ");
			break;

		default:
			(*m_stream) << QObject::tr("Unknown    ");
			break;
		}

		(*m_stream) << message << ".\n";

		m_stream->flush();
		m_file->flush();
	}
}
