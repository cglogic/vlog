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

#include "dialog.h"

namespace General
{
	void CDialog::information(const QString& text, const QString& caption)
	{
		QMessageBox::information(0, caption, text);
	}

	void CDialog::warning(const QString& text, const QString& caption)
	{
		QMessageBox::warning(0, caption, text);
	}

	void CDialog::error(const QString& text, const QString& caption)
	{
		QMessageBox::critical(0, caption, text);
	}

	CDialog::EDialogResult CDialog::question(const QString& text, const QString& caption)
	{
		EDialogResult result = DR_NO;

		QMessageBox *msg_box = new QMessageBox(caption, text, QMessageBox::Question, QMessageBox::Yes, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton);

		if (msg_box->exec() == QMessageBox::Yes)
		{
			result = DR_YES;
		}

		delete msg_box;

		return result;
	}

	QString CDialog::openFile(const QString& patterns, const QString& caption, const QString& dir)
	{
		QString filename = QFileDialog::getOpenFileName(0, caption, dir, patterns);

		return filename;
	}

	QString CDialog::saveFile(const QString& patterns, const QString& caption)
	{
		QString filename = QFileDialog::getSaveFileName(0, caption, QDir::currentPath(), patterns);

		return filename;
	}

	QString CDialog::openDirectory(const QString& caption, const QString& start_dir)
	{
		QString dir = QFileDialog::getExistingDirectory(0, caption, start_dir);

		return dir;
	}
}
