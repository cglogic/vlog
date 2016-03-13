#pragma once

#include "interface/import_plugin.h"
#include "plugin_data.h"

#include <QObject>
#include <QAction>
#include <QStringList>
#include <QtPlugin>
#include <QPixmap>
#include <QImageReader>

namespace General
{
	class CImportLasPlugin
		: public QObject, public Interface::CImportPlugin
	{
		Q_OBJECT
		Q_INTERFACES(Interface::CImportPlugin)

	public:
		CImportLasPlugin();
		virtual ~CImportLasPlugin();

		// get plugin info
		virtual QString getInfoHeader() const;

		// register in application
		virtual QAction* createMenuAction();
		virtual QStringList getFileExtensions() const;

		// get import data
		virtual Interface::CImportData* process(const QString& filename = "");

	};
}
