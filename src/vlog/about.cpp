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

#include "about.h"

namespace General
{
	CAboutDialog::CAboutDialog()
	{
		setupUi();

		QObject::connect(pushButton_Ok, SIGNAL(clicked()), this, SLOT(accept()));

		QMetaObject::connectSlotsByName(this);
	}

	CAboutDialog::~CAboutDialog()
	{
	}

	void CAboutDialog::setupUi()
	{
		if (this->objectName().isEmpty())
		{
			this->setObjectName(QString::fromUtf8("aboutDialog"));
		}

		this->resize(407, 146);
		this->setMinimumSize(QSize(407, 146));
		this->setMaximumSize(QSize(407, 146));
		QIcon icon;
		icon.addFile(QString::fromUtf8(":/icons/16/icons/16/Information_16x16.png"), QSize(), QIcon::Normal, QIcon::Off);
		this->setWindowIcon(icon);
		this->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
		this->setModal(true);
		horizontalLayout = new QHBoxLayout(this);
		horizontalLayout->setSpacing(5);
#ifndef Q_OS_MAC
		horizontalLayout->setContentsMargins(9, 9, 9, 9);
#endif
		horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
		label_Image = new QLabel(this);
		label_Image->setObjectName(QString::fromUtf8("label_Image"));
		label_Image->setMinimumSize(QSize(128, 128));
		label_Image->setMaximumSize(QSize(128, 128));
		label_Image->setPixmap(QPixmap(QString::fromUtf8(":/icons/line_chart.png")));
		label_Image->setAlignment(Qt::AlignCenter);

		horizontalLayout->addWidget(label_Image);

		frame_Main = new QFrame(this);
		frame_Main->setObjectName(QString::fromUtf8("frame_Main"));
		frame_Main->setMinimumSize(QSize(256, 128));
		frame_Main->setMaximumSize(QSize(256, 128));
		frame_Main->setFrameShape(QFrame::NoFrame);
		frame_Main->setFrameShadow(QFrame::Plain);
		verticalLayout = new QVBoxLayout(frame_Main);
		verticalLayout->setSpacing(5);
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		textBrowser = new QTextBrowser(frame_Main);
		textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
		textBrowser->setEnabled(true);
		textBrowser->setMinimumSize(QSize(0, 0));
		textBrowser->setMaximumSize(QSize(16777215, 16777215));
		textBrowser->setFocusPolicy(Qt::NoFocus);
		textBrowser->setContextMenuPolicy(Qt::NoContextMenu);
		textBrowser->setAutoFillBackground(false);
		textBrowser->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
		textBrowser->setFrameShape(QFrame::NoFrame);
		textBrowser->setFrameShadow(QFrame::Plain);
		textBrowser->setLineWrapMode(QTextEdit::WidgetWidth);
		textBrowser->setReadOnly(true);
		textBrowser->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
		textBrowser->setOpenExternalLinks(true);

		verticalLayout->addWidget(textBrowser);

		frame_Button = new QFrame(frame_Main);
		frame_Button->setObjectName(QString::fromUtf8("frame_Button"));
		frame_Button->setMinimumSize(QSize(0, 23));
		frame_Button->setMaximumSize(QSize(16777215, 23));
		frame_Button->setFrameShape(QFrame::NoFrame);
		frame_Button->setFrameShadow(QFrame::Plain);
		horizontalLayout_2 = new QHBoxLayout(frame_Button);
		horizontalLayout_2->setSpacing(0);
		horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
		horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
		frame_Space = new QFrame(frame_Button);
		frame_Space->setObjectName(QString::fromUtf8("frame_Space"));
		frame_Space->setMinimumSize(QSize(0, 23));
		frame_Space->setMaximumSize(QSize(16777215, 23));
		frame_Space->setFrameShape(QFrame::NoFrame);
		frame_Space->setFrameShadow(QFrame::Plain);

		horizontalLayout_2->addWidget(frame_Space);

		pushButton_Ok = new QPushButton(frame_Button);
		pushButton_Ok->setObjectName(QString::fromUtf8("pushButton_Ok"));
		pushButton_Ok->setMinimumSize(QSize(75, 23));
		pushButton_Ok->setMaximumSize(QSize(75, 23));

		horizontalLayout_2->addWidget(pushButton_Ok);
		verticalLayout->addWidget(frame_Button);
		horizontalLayout->addWidget(frame_Main);

		retranslateUi();
	}

	void CAboutDialog::retranslateUi()
	{
		this->setWindowTitle(QApplication::translate("aboutDialog", "About VLog", 0, QApplication::UnicodeUTF8));
		label_Image->setText(QString());
		textBrowser->setHtml(QApplication::translate("aboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600; color:#414141;\">VLog version 1.0"
#ifdef VL_DEMO_BUILD
" DEMO"
#endif
"</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600; color:#414141;\">Copiright 2013 CLogic Ltd.</span></p>\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#414141;\">&nbsp;</span></p>\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"mailto:vlog@clogic.com.ua\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">vlog@clogic.com.ua</span></a></p>\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://www.clogic.com.ua/products/vlog\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">http://www.clogic.com.ua</span></a></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; text-decoration: underline; color:#0000ff;\"><br /></p>\n"
"</body></html>", 0, QApplication::UnicodeUTF8)); /*<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600; color:#414141;\">Developers: gor, whiplash</span></p>*/
		pushButton_Ok->setText(QApplication::translate("aboutDialog", "Ok", 0, QApplication::UnicodeUTF8));
	}
}
