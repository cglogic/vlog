#include "scene.h"

#include "../window.h"

namespace General
{
	CScene::CScene(QWidget* parent)
		: QGraphicsScene(parent), m_mode((EMode)(ModePixmap | ModeDepth | ModeParameter | ModeCurve | ModePoint)),
		  m_active_object(0), m_pixmap(0), m_depth(0), m_border(5.0), m_timer_started(false), m_timer_id(0), m_alt_pressed(false)
	{
		m_scroll_x = CConfig::getInstance()->getSection("scene")->getParamInt("scroll_x");
		m_scroll_y = CConfig::getInstance()->getSection("scene")->getParamInt("scroll_y");
	}

	CScene::~CScene()
	{
		for (t_parameters_iter iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
		{
			CParameter *p = *iter;
			m_parameters.erase(iter);
			delete p;
		}

		if (m_depth != 0)
		{
			removeItem(m_depth);
			delete m_depth;
		}

		if (m_pixmap != 0)
		{
			removeItem(m_pixmap);
			delete m_pixmap;
		}
	}

	//

	CScene::EMode CScene::mode() const
	{
		return m_mode;
	}

	void CScene::setMode(EMode mode, bool enable)
	{
		if (enable)
		{
			m_mode = (EMode)(m_mode | mode);
		}
		else
		{
			m_mode = (EMode)(m_mode ^ mode);
		}
	}

	//

	CPixmap* CScene::setPixmap(const QPixmap& pixmap)
	{
		if (m_pixmap == 0)
		{
			m_pixmap = new CPixmap(this);

			addItem(m_pixmap);

			activateObject(m_pixmap);

			connect(m_pixmap, SIGNAL(signalActivate(CObject*)), this, SLOT(slotObjectActivate(CObject*)));
			//connect(m_pixmap, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotObjectDeleted(CObject*)));
		}
		else
		{
			emit signalObjectDeleted(m_pixmap);
		}

		m_pixmap->init(pixmap);

		emit signalObjectCreated(m_pixmap);

		CCurve::setLoggingImage(m_pixmap->image());

		return m_pixmap;
	}

	CPixmap* CScene::pixmap()
	{
		return m_pixmap;
	}

	CRange* CScene::setDepth(const QPointF& ps)
	{
		if (m_depth != 0)
		{
			removeItem(m_depth);

			delete m_depth;
		}

		m_depth = new CRange(m_pixmap->pixmap().width() < m_pixmap->pixmap().height() ? CRange::DirectionVertical : CRange::DirectionHorizontal, ps, ps, this);
		m_depth->setFreeAngle(true);

		addItem(m_depth);

		connect(m_depth, SIGNAL(signalDelete(CRange*)), this, SLOT(slotDepthDelete(CRange*)));

		m_pixmap->lock();

		return m_depth;
	}

	CRange* CScene::depth()
	{
		return m_depth;
	}

	CParameter* CScene::addParameter(const QPointF& ps)
	{
		if (m_parameters.count() == 0)
		{
			m_depth->setFreeAngle(false);

			if (m_depth->direction() == CRange::DirectionHorizontal)
			{
				CCurve::setSortType(CCurve::SortHorizontal);
			}
			else
			{
				CCurve::setSortType(CCurve::SortVertical);
			}

			m_depth->lock();
		}

		return directAddParameter(ps, ps);
	}

	CParameter* CScene::directAddParameter(const QPointF& psf, const QPointF& psl)
	{
		CParameter *param = new CParameter(psf, psl, m_depth, this);

		addItem(param);

		connect(param, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotObjectDeleted(CObject*)));
		connect(param, SIGNAL(signalCaptionChanged(CObject*)), this, SLOT(slotObjectCaptionChanged(CObject*)));
		connect(param, SIGNAL(signalDelete(CParameter*)), this, SLOT(slotParameterDelete(CParameter*)));
		connect(m_depth, SIGNAL(signalMove(CRange*, const QPointF&)), param, SLOT(slotRangeMove(CRange*, const QPointF&)));
		connect(param, SIGNAL(signalSetStatusText(const QString&)), this, SLOT(slotSetStatusText(const QString&)));

		m_parameters.push_back(param);

		return param;
	}

	CParameter* CScene::parameter(int n)
	{
		return m_parameters.at(n);
	}

	//

	void CScene::slotExternalObjectActivation(CObject* ob)
	{
		if (ob != 0)
		{
			slotObjectActivate(ob);
		}
	}

	void CScene::slotExternalObjectDeletion(CObject* ob)
	{
		if (qobject_cast<CPixmap*>(ob) != 0)
		{
		}
		else if (qobject_cast<CRange*>(ob) != 0)
		{
			slotDepthDelete(qobject_cast<CRange*>(ob));
		}
		else if (qobject_cast<CParameter*>(ob) != 0)
		{
			slotParameterDelete(qobject_cast<CParameter*>(ob));
		}
		else if (qobject_cast<CCurve*>(ob) != 0)
		{
			slotCurveDelete(qobject_cast<CParameter*>(ob->fantomParent()), qobject_cast<CCurve*>(ob));
		}
	}

	//

	void CScene::slotObjectActivate(CObject* ob)
	{
		activateObject(ob);
	}

	void CScene::slotObjectDeleted(CObject* ob)
	{
		if (m_active_object == ob)
		{
			m_active_object = 0;
		}

		emit signalObjectDeleted(ob);
	}

	void CScene::slotObjectCaptionChanged(CObject* ob)
	{
		emit signalObjectCaptionChanged(ob);
	}

	//

	void CScene::slotCurveDelete(CParameter* pr, CCurve* cv)
	{
		t_parameters_const_iter iter = qFind(m_parameters, pr);

		if (iter != m_parameters.end())
		{
			pr->removeCurve(cv);
		}
	}

	void CScene::slotParameterDelete(CParameter* pr)
	{
		t_parameters_const_iter iter = qFind(m_parameters, pr);

		if (iter != m_parameters.end())
		{
			m_parameters.removeOne(pr);

			removeItem(pr);

			delete pr;

			if (m_parameters.size() <= 0)
			{
				m_depth->unlock();
				m_pixmap->unlock();
			}
		}

		if (m_parameters.count() == 0)
		{
			m_depth->setFreeAngle(true);
		}
	}

	void CScene::slotDepthDelete(CRange* rn)
	{
		if (m_depth != 0 && m_depth == rn)
		{
			for (t_parameters_iter iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
			{
				CParameter *p = *iter;
				m_parameters.erase(iter);
				delete p;
			}

			removeItem(m_depth);
			delete m_depth;
			m_depth = 0;

			m_pixmap->unlock();
		}
	}

	bool CScene::slotZoomChange(int delta)
	{
		CCurve *curve = qobject_cast<CCurve*>(m_active_object);

		if (curve)
		{
			if (curve->isPaintingEnabled())
			{
				curve->brushRadiusChanded(delta);

				return false;
			}
		}

		return true;
	}

	void CScene::slotSetStatusText(const QString& tx)
	{
		emit signalSetStatusText(tx);
	}

	//

	bool CScene::exportAction(Interface::CExportData* data, CObject* project)
	{
		if (m_pixmap == 0 || m_depth == 0 || m_parameters.size() == 0)
		{
			CDialog::warning(tr("No data to export."), tr("Export"));

			return false;
		}

		// version section

		if (m_depth->propertyValue("digit_delim").toInt() == 0)
		{
			data->setDlm("Comma");
		}
		else if (m_depth->propertyValue("digit_delim").toInt() == 1)
		{
			data->setDlm("Space");
		}
		else if (m_depth->propertyValue("digit_delim").toInt() == 2)
		{
			data->setDlm("Tab");
		}

		// well section

		int prec = m_depth->propertyValue("digit_prec").toInt();

		double start = m_depth->propertyValue("digit_start").toDouble();
		double stop = m_depth->propertyValue("digit_stop").toDouble();
		double step = m_depth->propertyValue("digit_step").toDouble();
		double null = m_depth->propertyValue("digit_null").toDouble();

#ifdef VL_DEMO_BUILD
		if (stop > start + (1000 * step))
		{
			stop = start + (1000 * step);
		}
#endif

		data->setStart(QString::number(start, 'f', prec));
		data->setStop(QString::number(stop, 'f', prec));
		data->setStep(QString::number(step, 'f', prec));
		data->setNull(QString::number(null, 'f', prec));

		data->setCompany(project->propertyValue("company").toString());
		data->setWell(project->propertyValue("well").toString());
		data->setField(project->propertyValue("field").toString());
		data->setLocation(project->propertyValue("location").toString());
		data->setCountry(project->propertyValue("country").toString());
		data->setServiceCompany(project->propertyValue("service_company").toString());

		if (project->propertyValue("date").toDate().toString("yyyy-MM-dd") != "1900-01-01")
		{
			data->setDate(project->propertyValue("date").toDate().toString("dd/MM/yyyy"));
		}

		if (project->propertyValue("latitude").toInt())
		{
			data->setLatitude(project->propertyValue("latitude").toString());
		}

		if (project->propertyValue("longitude").toInt())
		{
			data->setLongitude(project->propertyValue("longitude").toString());
		}

		if (project->propertyValue("x").toInt())
		{
			data->setX(project->propertyValue("x").toString());
		}

		if (project->propertyValue("y").toInt())
		{
			data->setY(project->propertyValue("y").toString());
		}

		data->setLicence(project->propertyValue("licence").toString());
		data->setGeodeticDatum(project->propertyValue("geodetic_datum").toString());
		data->setHorizontalCoords(project->propertyValue("horizontal_coords").toString());
		data->setProvince(project->propertyValue("province").toString());
		data->setWell(project->propertyValue("well_id").toString());
		data->setWellID(project->propertyValue("state").toString());
		data->setCounty(project->propertyValue("county").toString());
		data->setAPI(project->propertyValue("api").toString());

		// log definition section

		data->addLogDefinition("DEPTH", "M", "", "");

		for (t_parameters_const_iter iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
		{
			data->addLogDefinition((*iter)->propertyValue("name").toString(), (*iter)->propertyValue("unit").toString(), "", "");
		}

		// log data section

		double curr = start;

		while (curr < stop)
		{
			QStringList values;

			values << QString::number(curr, 'f', prec);

			for (t_parameters_const_iter iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
			{
				// note: qreal CParameter::ordinate(qreal abscissa, qreal value_if_abscissa_not_exists);
				// 1. Получаем координату по значению глубины
				// 2. Обходим все кривые принадлежащие данному параметру в поисках нужной кривой
				// 3. Получаем координату по позиции точки на кривой
				// 4. Получаем значение параметра по координате точки
				values << QString::number((*iter)->valueByDepth(curr, null), 'f', prec);
			}

			data->addLodData(values);

			curr += step;
		}

		// Special for last value

		QStringList values;

		values << QString::number(curr, 'f', prec);

		for (t_parameters_const_iter iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
		{
			values << QString::number((*iter)->valueByDepth(stop, null));
		}

		data->addLodData(values);

		return true;
	}

	//

	void CScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
	{
		CRange *depth = qobject_cast<CRange*>(m_active_object);
		CParameter *parameter = qobject_cast<CParameter*>(m_active_object);
		CCurve *curve = qobject_cast<CCurve*>(m_active_object);

		if (depth || parameter || (curve && !curve->isPaintingEnabled()))
		{
			if (event->buttons() & Qt::LeftButton)
			{
				QRectF rect = views().at(0)->viewport()->rect().adjusted(m_border, m_border, -m_border, -m_border);
				QPointF point = views().at(0)->mapFromGlobal(QCursor::pos());

				if (!rect.contains(point))
				{
					if (!m_timer_started)
					{
						m_timer_started = true;

						m_timer_id = startTimer(20);
					}
				}
				else
				{
					if (m_timer_started)
					{
						m_timer_started = false;

						killTimer(m_timer_id);
					}
				}
			}
		}

		if (curve)
		{
			curve->cursorPosChanged(event->scenePos());

			if (curve->isPaintingEnabled())
			{
				curve->brushCursor()->setPos(event->scenePos());

				if (event->buttons() & Qt::LeftButton)
				{
					curve->painting(event->scenePos());
				}
				else if (event->buttons() & Qt::RightButton)
				{
					curve->pipette(event->scenePos());
				}
			}
		}

		QGraphicsScene::mouseMoveEvent(event);
	}

	void CScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			CCurve *curve = qobject_cast<CCurve*>(m_active_object);

			if (curve)
			{
				if (curve->isPaintingEnabled())
				{
					curve->pipette(event->scenePos());
				}
			}
		}

		if (event->buttons() & Qt::LeftButton)
		{
			// Point
			if (m_mode & ModePoint)
			{
				for (t_parameters_iter iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
				{
					for (int i = 0; i < (*iter)->curveCount(); ++i)
					{
						if (itemAt(event->scenePos()) == (*iter)->curveAt(i))
						{
							if ((*iter)->curveAt(i)->isActivated())
							{
								if (!(*iter)->curveAt(i)->isPaintingEnabled())
								{
									(*iter)->curveAt(i)->addPoint(event->scenePos());
								}
							}

							break;
						}
					}
				}
			}

			// Curve
			if (m_mode & ModeCurve)
			{
				for (t_parameters_const_iter iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
				{
					if (itemAt(event->scenePos()) == *iter)
					{
						CCurve *cv = (*iter)->addCurve(event->scenePos());

						activateObject(cv);

						connect(cv, SIGNAL(signalActivate(CObject*)), this, SLOT(slotObjectActivate(CObject*)));
						connect(cv, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotObjectDeleted(CObject*)));

						connect(m_pixmap, SIGNAL(signalRotate(qreal)), cv, SLOT(slotImageRotate(qreal)));

						cv->slotImageRotate(m_pixmap->rotateAngle());

						for (int i = 0; i < (*iter)->curveCount(); ++i)
						{
							emit signalObjectDeleted((*iter)->curveAt(i));
						}

						for (int i = 0; i < (*iter)->curveCount(); ++i)
						{
							emit signalObjectCreated((*iter)->curveAt(i));
						}

						emit signalObjectActivated(cv);

						break;
					}
				}
			}

			// Parameter
			if (m_mode & ModeParameter)
			{
				if (m_depth != 0 && itemAt(event->scenePos()) == m_pixmap)
				{
					qreal ps = m_depth->direction() == CRange::DirectionHorizontal
							? event->scenePos().x()
							: event->scenePos().y();

					qreal min_ps = m_depth->direction() == CRange::DirectionHorizontal
							? qMin(m_depth->sceneLine().x1(), m_depth->sceneLine().x2())
							: qMin(m_depth->sceneLine().y1(), m_depth->sceneLine().y2());

					qreal max_ps = m_depth->direction() == CRange::DirectionHorizontal
							? qMax(m_depth->sceneLine().x1(), m_depth->sceneLine().x2())
							: qMax(m_depth->sceneLine().y1(), m_depth->sceneLine().y2());

					if (ps < min_ps || ps > max_ps)
					{
						CParameter *pr = addParameter(event->scenePos());

						activateObject(pr);

						connect(pr, SIGNAL(signalActivate(CObject*)), this, SLOT(slotObjectActivate(CObject*)));
						connect(pr, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotObjectDeleted(CObject*)));

						emit signalObjectCreated(pr);
					}
				}
			}

			// Depth
			if (m_mode & ModeDepth && !m_alt_pressed)
			{
				if (m_pixmap != 0 && itemAt(event->scenePos()) == m_pixmap && m_depth == 0)
				{
					CRange *rn = setDepth(event->scenePos());

					activateObject(rn);

					connect(rn, SIGNAL(signalActivate(CObject*)), this, SLOT(slotObjectActivate(CObject*)));
					connect(rn, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotObjectDeleted(CObject*)));

					emit signalObjectCreated(rn);
				}
			}

			// Pixmap
			if (m_mode & ModePixmap && m_alt_pressed && m_depth == 0)
			{
				if (m_pixmap != 0 && m_pixmap->curve() != 0 && (itemAt(event->scenePos()) == m_pixmap->curve() || itemAt(event->scenePos()) == m_pixmap))
				{
					m_pixmap->addPoint(event->scenePos());
				}
			}
		}

		QGraphicsScene::mousePressEvent(event);
	}

	void CScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
	{
		QGraphicsScene::mouseReleaseEvent(event);
	}

	void CScene::keyPressEvent(QKeyEvent* event)
	{
		QGraphicsScene::keyPressEvent(event);
	}

	void CScene::keyReleaseEvent(QKeyEvent* event)
	{
		QGraphicsScene::keyReleaseEvent(event);
	}

	void CScene::timerEvent(QTimerEvent* event)
	{
		if (event->timerId() == m_timer_id)
		{
			if (QApplication::mouseButtons() & Qt::LeftButton)
			{
				QGraphicsView *view = views().at(0);

				QRectF rect = views().at(0)->viewport()->rect();
				QPointF point = views().at(0)->mapFromGlobal(QCursor::pos());

				QScrollBar *hbar = view->horizontalScrollBar();
				QScrollBar *vbar = view->verticalScrollBar();

				if (rect.bottom() - m_border < point.y())
				{
					vbar->setValue(vbar->value() + m_scroll_y);
				}

				if (rect.top() + m_border > point.y())
				{
					vbar->setValue(vbar->value() - m_scroll_y);
				}

				if (rect.left() + m_border > point.x())
				{
					hbar->setValue(hbar->value() - m_scroll_x);
				}

				if (rect.right() - m_border < point.x())
				{
					hbar->setValue(hbar->value() + m_scroll_x);
				}
			}
			else
			{
				m_timer_started = false;

				killTimer(m_timer_id);
			}
		}

		return QObject::timerEvent(event);
	}

	//

	void CScene::activateObject(CObject* ob)
	{
		if (m_active_object == ob)
		{
			return;
		}

		if (m_active_object != 0)
		{
			m_active_object->deactivate();

			CCurve *curve = qobject_cast<CCurve*>(m_active_object);

			if (curve)
			{
				curve->setPaintingEnable(false);
			}
		}

		if (ob != 0)
		{
			ob->activate();

			emit signalObjectActivated(ob);
		}

		m_active_object = ob;
	}

	CObject* CScene::activeObject()
	{
		return m_active_object;
	}

	//

	int CScene::parameterCount() const
	{
		return m_parameters.count();
	}

	CParameter* CScene::parameterAt(int i)
	{
		return m_parameters.at(i);
	}

	void CScene::globalKeyPressEvent(QKeyEvent* event)
	{
		if (event->modifiers() & Qt::AltModifier)
		{
			CCurve *curve = qobject_cast<CCurve*>(m_active_object);

			if (curve)
			{
				curve->setPaintingEnable(true);
			}

			CPixmap *pixmap = qobject_cast<CPixmap*>(m_active_object);

			if (pixmap != 0)
			{
				pixmap->curveActivate(true);
			}

			m_alt_pressed = true;
		}
	}

	void CScene::globalKeyReleaseEvent(QKeyEvent* event)
	{
		if (event->modifiers() | ~Qt::AltModifier)
		{
			CCurve *curve = qobject_cast<CCurve*>(m_active_object);

			if (curve)
			{
				curve->setPaintingEnable(false);
			}

			CPixmap *pixmap = qobject_cast<CPixmap*>(m_active_object);

			if (pixmap != 0)
			{
				pixmap->curveActivate(false);
			}

			m_alt_pressed = false;
		}
	}

	void CScene::serialize(QDomDocument& document, const QString& path)
	{
		QDomNodeList nodes = document.elementsByTagName("project");

		if (nodes.count())
		{
			QDomElement elem = document.createElement("scene");
			nodes.at(0).appendChild(elem);

			// element
			QDomElement elem_view = document.createElement("view");
			elem.appendChild(elem_view);

			QDomElement elem_hbar = document.createElement("hbar");
			elem_hbar.setAttribute("value", views().at(0)->horizontalScrollBar()->value());
			elem_view.appendChild(elem_hbar);

			QDomElement elem_vbar = document.createElement("vbar");
			elem_vbar.setAttribute("value", views().at(0)->verticalScrollBar()->value());
			elem_view.appendChild(elem_vbar);

			QDomElement elem_scale = document.createElement("scale");
			elem_scale.setAttribute("m11", views().at(0)->transform().m11());
			elem_scale.setAttribute("m22", views().at(0)->transform().m22());
			elem_view.appendChild(elem_scale);

			EObject active_object = ObjectNone;
			int active_object_index = -1;
			int active_object_index_parent = -1;

			if (qobject_cast<CPixmap*>(m_active_object))
			{
				active_object = ObjectCurve;
			}
			else if (qobject_cast<CRange*>(m_active_object))
			{
				active_object = ObjectDepth;
			}
			else if (qobject_cast<CParameter*>(m_active_object))
			{
				active_object = ObjectParameter;

				for (int i = 0; i < m_parameters.count(); ++i)
				{
					if (m_parameters.at(i) == m_active_object)
					{
						active_object_index = i;

						break;
					}
				}
			}
			else if (qobject_cast<CCurve*>(m_active_object))
			{
				active_object = ObjectCurve;

				for (int i = 0; i < m_parameters.count(); ++i)
				{
					for (int j = 0; j < m_parameters.at(i)->curveCount(); ++j)
					{
						if (m_parameters.at(i)->curveAt(j) == m_active_object)
						{
							active_object_index_parent = i;
							active_object_index = j;

							break;
						}
					}
				}
			}

			QDomElement active_elem = document.createElement("active");
			active_elem.setAttribute("object", active_object);
			active_elem.setAttribute("index", active_object_index);
			active_elem.setAttribute("index_parent", active_object_index_parent);
			elem.appendChild(active_elem);

			if (m_pixmap)
			{
				QDomElement pixmap_elem = m_pixmap->serialize(document, path);
				elem.appendChild(pixmap_elem);
			}

			if (m_depth)
			{
				QDomElement depth_elem = m_depth->serialize(document);
				elem.appendChild(depth_elem);
			}

			// element
			QDomElement elem_params = document.createElement("parameters");
			elem.appendChild(elem_params);
			for (t_parameters_const_iter iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
			{
				QDomElement elem_param = (*iter)->serialize(document);
				elem_params.appendChild(elem_param);
			}
		}
	}

	void CScene::deserialize(const QDomElement& element)
	{
		QString path;

		QDomNodeList nodes = element.elementsByTagName("project_path");
		if (nodes.count())
		{
			path = nodes.at(0).toElement().attribute("value");
		}

		nodes = element.elementsByTagName("scene");
		if (nodes.count())
		{
			QDomElement scene_elem = nodes.at(0).toElement();

			// pixmap
			QString pixmap_file = path + "/log.png";
			if (QFile(pixmap_file).exists())
			{
				CPixmap *pixmap = setPixmap(QPixmap(path + "/log.png"));
				if (pixmap)
				{
					pixmap->deserialize(scene_elem);
				}
			}
			else
			{
				return;
			}

			// depth
			nodes = scene_elem.elementsByTagName("depth");
			if (nodes.count())
			{
				QDomElement depth_elem = nodes.at(0).toElement();

				QDomElement elem_p0;
				QDomElement elem_p1;
				QDomElement elem_dir;

				nodes = depth_elem.elementsByTagName("point0");
				if (nodes.count())
				{
					elem_p0 = nodes.at(0).toElement();
				}

				nodes = depth_elem.elementsByTagName("point1");
				if (nodes.count())
				{
					elem_p1 = nodes.at(0).toElement();
				}

				nodes = depth_elem.elementsByTagName("direction");
				if (nodes.count())
				{
					elem_dir = nodes.at(0).toElement();
				}

				m_depth = new CRange(static_cast<CRange::EDirection>(elem_dir.attribute("value").toInt()), QPointF(elem_p0.attribute("x").toDouble(), elem_p0.attribute("y").toDouble()), QPointF(elem_p1.attribute("x").toDouble(), elem_p1.attribute("y").toDouble()), this);
				m_depth->setFreeAngle(true);

				addItem(m_depth);

				connect(m_depth, SIGNAL(signalDelete(CRange*)), this, SLOT(slotDepthDelete(CRange*)));

				m_pixmap->lock();

				activateObject(m_depth);

				connect(m_depth, SIGNAL(signalActivate(CObject*)), this, SLOT(slotObjectActivate(CObject*)));
				connect(m_depth, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotObjectDeleted(CObject*)));

				m_depth->deserialize(scene_elem);

				emit signalObjectCreated(m_depth);
			}
			else
			{
				return;
			}

			// parameters
			QDomNodeList param_nodes = scene_elem.elementsByTagName("parameters");
			if (param_nodes.count())
			{
				if (m_depth->direction() == CRange::DirectionHorizontal)
				{
					CCurve::setSortType(CCurve::SortHorizontal);
				}
				else
				{
					CCurve::setSortType(CCurve::SortVertical);
				}

				QDomElement param_elem = param_nodes.at(0).toElement().firstChildElement("parameter");

				while (!param_elem.isNull())
				{
					// parameter
					QDomElement param_p0 = param_elem.elementsByTagName("point0").at(0).toElement();
					QDomElement param_p1 = param_elem.elementsByTagName("point1").at(0).toElement();

					CParameter *parameter = directAddParameter(QPointF(param_p0.attribute("x").toDouble(), param_p0.attribute("y").toDouble()), QPointF(param_p1.attribute("x").toDouble(), param_p1.attribute("y").toDouble()));

					activateObject(parameter);

					connect(parameter, SIGNAL(signalActivate(CObject*)), this, SLOT(slotObjectActivate(CObject*)));
					connect(parameter, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotObjectDeleted(CObject*)));

					parameter->deserialize(param_elem);

					emit signalObjectCreated(parameter);

					parameter->sort();
					parameter->update();

					for (int i = 0; i < parameter->curveCount(); ++i)
					{
						CCurve *curve = parameter->curveAt(i);

						connect(curve, SIGNAL(signalActivate(CObject*)), this, SLOT(slotObjectActivate(CObject*)));
						connect(curve, SIGNAL(signalDeleted(CObject*)), this, SLOT(slotObjectDeleted(CObject*)));
						connect(m_pixmap, SIGNAL(signalRotate(qreal)), curve, SLOT(slotImageRotate(qreal)));

						curve->slotImageRotate(m_pixmap->rotateAngle());

						emit signalObjectCreated(curve);

						activateObject(curve);
					}

					param_elem = param_elem.nextSiblingElement("parameter");
				}

				m_depth->setFreeAngle(false);
				m_depth->lock();
			}

			// view
			QDomNodeList view_nodes = scene_elem.elementsByTagName("view");
			if (view_nodes.count())
			{
				QDomElement view_elem = view_nodes.at(0).toElement();

				nodes = view_elem.elementsByTagName("scale");
				if (nodes.count())
				{
					QDomElement elem_scale = nodes.at(0).toElement();

					qreal m11 = elem_scale.attribute("m11", "0.0").toFloat();
					qreal m22 = elem_scale.attribute("m22", "0.0").toFloat();

					views().at(0)->scale(m11, m22);
				}


				nodes = view_elem.elementsByTagName("hbar");
				if (nodes.count())
				{
					QDomElement elem_hbar = nodes.at(0).toElement();

					int value = elem_hbar.attribute("value", "0").toInt();

					views().at(0)->horizontalScrollBar()->setValue(value);
				}

				nodes = view_elem.elementsByTagName("vbar");
				if (nodes.count())
				{
					QDomElement elem_vbar = nodes.at(0).toElement();

					int value = elem_vbar.attribute("value", "0").toInt();

					views().at(0)->verticalScrollBar()->setValue(value);
				}
			}

			// active
			QDomNodeList active_nodes = scene_elem.elementsByTagName("active");
			if (active_nodes.count())
			{
				QDomElement active_elem = active_nodes.at(0).toElement();

				EObject active_object = static_cast<EObject>(active_elem.attribute("object", QString::number(ObjectNone)).toInt());
				int active_object_index = active_elem.attribute("index", "-1").toInt();
				int active_object_index_parent = active_elem.attribute("index_parent", "-1").toInt();

				if (active_object == ObjectPixmap && m_pixmap)
				{
					activateObject(m_pixmap);
				}
				else if (active_object == ObjectDepth && m_depth)
				{
					activateObject(m_depth);
				}
				else if (active_object == ObjectParameter && active_object_index >= 0)
				{
					activateObject(m_parameters.at(active_object_index));
				}
				else if (active_object == ObjectCurve && active_object_index >= 0 && active_object_index_parent >= 0)
				{
					activateObject(m_parameters.at(active_object_index_parent)->curveAt(active_object_index));
				}
			}
		}
	}
}
