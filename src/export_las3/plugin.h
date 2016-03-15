#pragma once

#include "interface/export_plugin.h"
#include "plugin_data.h"

#include <QObject>
#include <QAction>
#include <QStringList>
#include <QtPlugin>
#include <QFileDialog>

namespace General
{
	class CExportLas3Plugin
		: public QObject, public Interface::CExportPlugin
	{
		Q_OBJECT
		Q_INTERFACES(Interface::CExportPlugin)

	public:
		CExportLas3Plugin();
		virtual ~CExportLas3Plugin();

		// get plugin info
		virtual QString getInfoHeader() const;
		virtual QString getUid() const;

		// register in application
		virtual QAction* createMenuAction();
		virtual QStringList getFileExtensions() const;

		// property template
		virtual QString propertyTemplate() const;

		// get handled export data
		virtual Interface::CExportData* getCleanDataHandle() const;

		// exporting
		virtual void process(const QString& filename = "");

	private:
		CExportLas3Data *m_data;

	};
}
