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
