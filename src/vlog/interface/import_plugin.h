#pragma once

#include "import_data.h"

#include <QObject>
#include <QAction>
#include <QStringList>

namespace Interface
{
	// Интерфейс плагина импорта
	class CImportPlugin
	{
	public:
		CImportPlugin();
		virtual ~CImportPlugin();

		// get plugin info
		virtual QString getInfoHeader() const = 0;

		// register in application
		virtual QAction* createMenuAction() = 0;
		virtual QStringList getFileExtensions() const = 0;

		// get import data
		virtual CImportData* process(const QString& filename = "") = 0;

	private:
		// disable copying
		CImportPlugin(const CImportPlugin& plugin);
		CImportPlugin& operator=(const CImportPlugin& rhs);

	};
}

Q_DECLARE_INTERFACE(Interface::CImportPlugin, "com.company.vanamingo.CImportPlugin/1.0")
