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
