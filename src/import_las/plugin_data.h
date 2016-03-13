#pragma once

#include "interface/import_data.h"

#include "loadform.h"

#include <QString>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QMap>
#include <QPaintEngine>
#include <QPaintEngineState>
#include <QPainterPath>
#include <limits>

namespace General
{
	class CImportLasData
		: public Interface::CImportData
	{
	public:
		enum ESectionType
		{
			ST_NONE,
			ST_VERSION,
			ST_WELL,
			ST_PARAMETER,
			ST_CURVE,
			ST_ASCII
		};

		CImportLasData();
		virtual ~CImportLasData();

		// get pixmap (your Captain Obvious)
		virtual const QPixmap& getPixmap();

		// import not canceled
		virtual bool accepted() const;

		// load (aka open&read)
		void load();

	private:
		QPixmap m_pixmap;

		bool m_accepted;

		typedef QVector<double> t_value;
		typedef QVector<t_value> t_values;

	};
}
