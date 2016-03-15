#pragma once

#include "config.h"

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

namespace General
{
	class CAboutDialog
		: public QDialog
	{
		Q_OBJECT

	public:
		CAboutDialog(void);
		virtual ~CAboutDialog(void);

	protected:
		void setupUi(void);
		void retranslateUi(void);

	private:
		QHBoxLayout *horizontalLayout;
		QLabel *label_Image;
		QFrame *frame_Main;
		QVBoxLayout *verticalLayout;
		QTextBrowser *textBrowser;
		QFrame *frame_Button;
		QHBoxLayout *horizontalLayout_2;
		QFrame *frame_Space;
		QPushButton *pushButton_Ok;

	};
}
