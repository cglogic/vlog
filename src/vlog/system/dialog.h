#pragma once

#include <QString>
#include <QMessageBox>
#include <QObject>
#include <QFileDialog>

namespace General
{
	class CDialog
	{
	public:
		enum EDialogResult
		{
			DR_YES,
			DR_NO
		};

		static void information(const QString& text, const QString& caption = QObject::tr("Information"));
		static void warning(const QString& text, const QString& caption = QObject::tr("Warning"));
		static void error(const QString& text, const QString& caption = QObject::tr("Error"));

		static EDialogResult question(const QString& text, const QString& caption = QObject::tr("Question"));

		static QString openFile(const QString& patterns, const QString& caption = QObject::tr("Open file"), const QString& dir = "");
		static QString saveFile(const QString& patterns, const QString& caption = QObject::tr("Save file"));

		static QString openDirectory(const QString& caption = QObject::tr("Select directory"), const QString& start_dir = "");

	private:
		// disable creating
		CDialog();
		~CDialog();

		// disable copying
		CDialog(const CDialog& dialog);
		const CDialog& operator= (const CDialog& dialog);

	};
}
