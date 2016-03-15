#pragma once

#include "export_data.h"

#include <QObject>
#include <QAction>
#include <QStringList>
#include <QFile>

namespace Interface
{
	class CExportPlugin
	{
	public:
		CExportPlugin();
		virtual ~CExportPlugin();

		// get plugin info
		virtual QString getUid() const = 0;
		virtual QString getInfoHeader() const = 0;

		// register in application
		virtual QAction* createMenuAction() = 0;
		virtual QStringList getFileExtensions() const = 0;

		// property template
		virtual QString propertyTemplate() const;

		// get handled export data
		virtual CExportData* getCleanDataHandle() const = 0;

		// exporting
		virtual void process(const QString& filename = "") = 0;

	private:
		// disable copying
		CExportPlugin(const CExportPlugin& plugin);
		CExportPlugin& operator=(const CExportPlugin& rhs);

	};
}

Q_DECLARE_INTERFACE(Interface::CExportPlugin, "com.company.vanamingo.CExportPlugin/1.0")
