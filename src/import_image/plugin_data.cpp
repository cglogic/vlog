#include "plugin_data.h"

namespace General
{
	CImportImageData::CImportImageData()
	{
	}

	CImportImageData::~CImportImageData()
	{
	}

	const QPixmap& CImportImageData::getPixmap()
	{
		return m_pixmap;
	}

	void CImportImageData::loadFromFile(const QString& filename)
	{
		m_pixmap = QPixmap(filename);
	}
}
