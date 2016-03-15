#pragma once

#include "interface/import_data.h"

#include <QString>
#include <QPixmap>

namespace General
{
	class CImportImageData
		: public Interface::CImportData
	{
	public:
		CImportImageData();
		virtual ~CImportImageData();

		// get pixmap (your Captain Obvious)
		virtual const QPixmap& getPixmap();

		// load image from file
		void loadFromFile(const QString& filename);

	private:
		QPixmap m_pixmap;

	};
}
