#pragma once

#include <QPixmap>

namespace Interface
{
	class CImportData
	{
	public:
		CImportData();
		virtual ~CImportData();

		// get pixmap (your Captain Obvious)
		virtual const QPixmap& getPixmap() = 0;

	};
}
