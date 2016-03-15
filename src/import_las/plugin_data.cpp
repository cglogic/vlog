#include "plugin_data.h"

#include <QDebug>

namespace General
{
	CImportLasData::CImportLasData()
		: m_accepted(false)
	{
	}

	CImportLasData::~CImportLasData()
	{
	}

	const QPixmap& CImportLasData::getPixmap()
	{
		return m_pixmap;
	}

	void CImportLasData::load()
	{
		//QFile out("d:/test.txt");
		//out.open(QIODevice::WriteOnly);
		//QTextStream strout(&out);

		m_accepted = false;

		CLoadForm load_form;

		if (load_form.exec() == QDialog::Accepted)
		{
			m_pixmap = QPixmap();

			QFile file(load_form.filename());

			if (file.open(QIODevice::ReadOnly))
			{
				QString first;
				QString line;
				QTextStream stream(&file);

				ESectionType section_type = ST_NONE;

				double null;
				t_value param_min;
				t_value param_max;
				t_values values;

				while (!stream.atEnd())
				{
					line = stream.readLine();
					first = line.mid(0, 1);

					if (first == "#" || line.isEmpty())
					{
						continue;
					}
					else if (first == "~")
					{
						if (line.toUpper().contains("VERSION"))
						{
							section_type = ST_VERSION;
						}
						else if (line.toUpper().contains("WELL"))
						{
							section_type = ST_WELL;
						}
						else if (line.toUpper().contains("CURVE"))
						{
							section_type = ST_CURVE;
						}
						else if (line.toUpper().contains("PARAMETER"))
						{
							section_type = ST_PARAMETER;
						}
						else if (line.toUpper().contains("ASCII") || line.mid(1, 2) == "A ")
						{
							section_type = ST_ASCII;
						}
						else
						{
							section_type = ST_NONE;
						}
					}
					else
					{
						switch (section_type)
						{
						case ST_WELL:
							{
								QRegExp regexp("-?\\d*\\.\\d+", Qt::CaseInsensitive);

								if (regexp.indexIn(line) > -1)
								{
									QString prop = line.toUpper().trimmed().mid(0, 4);

									if (prop == "NULL")
									{
										null = regexp.cap(0).toDouble();
									}
								}

								break;
							}

						case ST_ASCII:
							{
								QString data = line.trimmed();

								while (data.contains("  "))
								{
									data.replace("  ", " ");
								}

								QStringList datas = data.split(" ");

								if (datas.size() > param_min.size() || datas.size() > param_max.size())
								{
									param_min.resize(datas.size());
									param_max.resize(datas.size());

									for (int i = 0; i < datas.size(); ++i)
									{
										param_min[i] = static_cast<double>(INT_MAX);
										param_max[i] = static_cast<double>(INT_MIN);
									}
								}

								t_value value;

								for (int i = 0; i < datas.size(); ++i)
								{
									double v = datas.at(i).toDouble();

									value.push_back(v);

									if (v > null - 0.00001 && v < null + 0.00001)
									{
										continue;
									}

									if (v < param_min.at(i))
									{
										param_min[i] = v;
									}

									if (v > param_max.at(i))
									{
										param_max[i] = v;
									}
								}

								values.push_back(value);

								break;
							}

						default:
							{
								break;
							}
						}
					}
				}

				/*strout << null << "\n";

				for (int i = 0; i < param_min.size(); ++i)
				{
					strout << param_min.at(i) << " ";
				}

				strout << "\n";

				for (int i = 0; i < param_max.size(); ++i)
				{
					strout << param_max.at(i) << " ";
				}*/

				file.close();

				m_pixmap = QPixmap(load_form.pixmapWidth(), load_form.pixmapHeight());
				m_pixmap.fill(QColor(220, 220, 220));

				if (values.size() > 0)
				{
					QPainter painter(&m_pixmap);

					float x, y;

					QVector<QPainterPath*> paths;
					QMap<int, bool> nulls;

					for (int i = 0; i < param_min.size() - 1; ++i)
					{
						paths.push_back(new QPainterPath());
						nulls.insert(i + 1, true);
					}

					for (int i = 0; i < values.size(); ++i)
					{
						t_value value = values.at(i);

						y = (value.at(0) * (load_form.pixmapHeight() - 1)) / (param_max.at(0) - param_min.at(0));

						for (int j = 1; j < value.size(); ++j)
						{
							x = (value.at(j) * (load_form.pixmapWidth() - 1)) / (param_max.at(j) - param_min.at(j));

							if (value.at(j) > null - 0.00001 && value.at(j) < null + 0.00001)
							{
								nulls[j] = true;
							}
							else
							{
								if (nulls[j] == true)
								{
									//QPainterPath *path = paths.at(j - 1);
									//painter.drawPath(*path);

									//delete path;

									//paths[j - 1] = new QPainterPath();
									paths.at(j - 1)->moveTo(x, y);

									nulls[j] = false;
								}
								else
								{
									paths.at(j - 1)->lineTo(x, y);
								}
							}
						}
					}

					for (int i = 0; i < paths.size(); ++i)
					{
						QPainterPath *path = paths.at(i);

						painter.drawPath(*path);

						delete path;
					}

					painter.end();
				}

				m_accepted = true;
			}
		}

		//out.close();
	}

	bool CImportLasData::accepted() const
	{
		return m_accepted;
	}
}
