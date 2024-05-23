#include "SplineRenderer.h"
#include "Entity.h"
#include "Core.h"
#include "Camera.h"
#include "Transform.h"
#include "rend/rend.h"
#include "Spline.h"
#include "Model.h"
#include "Resources.h"

namespace red
{

	SplineRenderer::SplineRenderer()
	{
		//Defult values
		t = 0;
		m_shader_line = rend::RED_SHADER;
		m_splineCol = rend::vec4(0, 1, 0, 1);
		m_nodeConnectCol = rend::vec4(1, 0, 0, 1);
		m_nodeCol = rend::vec4(1, 1, 1, 1);
		m_nodeHighlightCol = rend::vec4(0, 0, 0, 1);
		m_rollCol = rend::vec4(1, 1, 1, 1);
		m_rollHighlightCol = rend::vec4(0, 0, 0, 1);
		m_boostCol = rend::vec4(1, 1, 1, 1);
		m_boostHighlightCol = rend::vec4(0, 0, 0, 1);
		s_show = true;
		s_spline = true;
		s_node = true;
		s_connect = true;
		s_roll = true;
		s_boost = true;
		h_node = -1;
		h_roll = -1;
		h_boost = -1;
	}

	SplineRenderer::~SplineRenderer()
	{
	}

	void SplineRenderer::setSpline(std::shared_ptr<Spline> spline)
	{
		m_spline = spline;
	}

	void SplineRenderer::setSplineColor(rend::vec3 _col)
	{
		m_splineCol = rend::vec4(_col, 1);
	}

	void SplineRenderer::setNodeConnectColor(rend::vec3 _col)
	{
		m_nodeConnectCol = rend::vec4(_col, 1);
	}

	void SplineRenderer::setNodeColor(rend::vec3 _col)
	{
		m_nodeCol = rend::vec4(_col, 1);
	}

	void SplineRenderer::setNodeHighlightColor(rend::vec3 _col)
	{
		m_nodeHighlightCol = rend::vec4(_col, 1);
	}

	void SplineRenderer::setRollColor(rend::vec3 _col)
	{
		m_rollCol = rend::vec4(_col, 1);
	}

	void SplineRenderer::setRollHighlightColor(rend::vec3 _col)
	{
		m_rollHighlightCol = rend::vec4(_col, 1);
	}

	void SplineRenderer::setBoostColor(rend::vec3 _col)
	{
		m_boostCol = rend::vec4(_col, 1);
	}

	void SplineRenderer::setBoostHighlightColor(rend::vec3 _col)
	{
		m_boostHighlightCol = rend::vec4(_col, 1);
	}

	void SplineRenderer::setShow(bool _flag)
	{
		s_show = _flag;
	}

	void SplineRenderer::setNodeShow(bool _flag)
	{
		s_node = _flag;
	}

	void SplineRenderer::setSplineShow(bool _flag)
	{
		s_spline = _flag;
	}

	void SplineRenderer::setNodeConnect(bool _flag)
	{
		s_connect = _flag;
	}

	void SplineRenderer::setRollShow(bool _flag)
	{
		s_roll = _flag;
	}

	void SplineRenderer::setBoosterShow(bool _flag)
	{
		s_boost = _flag;
	}

	void SplineRenderer::setNodeHighlight(int _index)
	{
		h_node = _index;
	}

	void SplineRenderer::setRollHighlight(int _index)
	{
		h_roll = _index;
	}

	void SplineRenderer::setBoostHighlight(int _index)
	{
		h_boost = _index;
	}

	rend::vec3 SplineRenderer::getSplineColor()
	{
		return rend::vec3(m_splineCol);
	}

	rend::vec3 SplineRenderer::getNodeConnectColor()
	{
		return rend::vec3(m_nodeConnectCol);
	}

	rend::vec3 SplineRenderer::getNodeColor()
	{
		return rend::vec3(m_nodeCol);
	}

	rend::vec3 SplineRenderer::getNodeHighlightColor()
	{
		return rend::vec3(m_nodeHighlightCol);
	}

	rend::vec3 SplineRenderer::getRollColor()
	{
		return rend::vec3(m_rollCol);
	}

	rend::vec3 SplineRenderer::getRollHighlightColor()
	{
		return rend::vec3(m_rollHighlightCol);
	}

	rend::vec3 SplineRenderer::getBoostColor()
	{
		return rend::vec3(m_boostCol);
	}

	rend::vec3 SplineRenderer::getBoostHighlightColor()
	{
		return rend::vec3(m_boostHighlightCol);
	}

	bool SplineRenderer::getShow()
	{
		return s_show;
	}

	bool SplineRenderer::getSplineShow()
	{
		return s_spline;
	}

	bool SplineRenderer::getNodeShow()
	{
		return s_node;
	}

	bool SplineRenderer::getNodeConnect()
	{
		return s_connect;
	}

	bool SplineRenderer::getRollShow()
	{
		return s_roll;
	}

	bool SplineRenderer::getBoostShow()
	{
		return s_boost;
	}

	void SplineRenderer::setDetail(float _deatil)
	{
		m_detail = _deatil;
	}

	std::shared_ptr<Spline> SplineRenderer::getSpline()
	{
		return m_spline;
	}

	void SplineRenderer::onInit()
	{
		m_mesh.load(rend::TRIANGLE_MESH);
		m_nodeModel = getCore().lock()->getResources()->load<Model>("models/Shapes/octahedron.obj");  
		m_rollModel = getCore().lock()->getResources()->load<Model>("models/Shapes/human.obj");
	}

	void SplineRenderer::onDisplay()
	{
		if (s_show)
		{
			//Model -> world -> clip -> Projection    (needs to reset the modelmat first)
			getEntity()->getTransform()->setPos(rend::vec3(0));
			getEntity()->getTransform()->setRotation(rend::vec3(0));
			getEntity()->getTransform()->setScale(rend::vec3(1));
			rend::mat4 modelMat(getEntity()->getTransform()->getModelMat());
			rend::mat4 viewMat(getCore().lock()->getCamera()->getView());
			rend::mat4 projectionMat(rend::perspective(rend::radians(45.0f), 1.0f, 0.1f, 100.0f));

			//Sets up line shader uniforms
			m_shader_line.uniform("u_Projection", projectionMat);
			m_shader_line.uniform("u_View", viewMat);
			m_shader_line.uniform("u_Model", modelMat);
			m_shader_line.attribute("a_Position", *m_mesh.positions());

			//Draws the spline
			if (s_spline && m_spline->valid())
			{
				float t = m_spline->getMinKnot();
				rend::vec3 a = m_spline->getPoint(t);

				while (t < m_spline->getMaxKnot() + m_detail)
				{
					rend::vec3 b = m_spline->getPoint(t);
					m_shader_line.uniform("u_Color", m_splineCol);

					//Changes colour of spline if boost is there
					if (s_boost)
					{
						for (int i = 0; i < m_spline->getTotalBoosters(); i++)
						{
							if (t > m_spline->getBoosterPos(i).x && t < m_spline->getBoosterPos(i).y)
							{
								m_shader_line.uniform("u_Color", m_boostCol);
								//Change color to boost highlight if needed
								if (i == h_boost)
								{
									m_shader_line.uniform("u_Color", m_boostHighlightCol);
									break;
								}
							}
						}
					}
				
					if (!(m_spline->getClamp() && t > m_spline->getMaxKnot()))
					{
						m_shader_line.render_line(a, b);
					}
					t += m_detail;
					a = b;
				}
			}

			//Draw line connecting nodes
			if (s_connect && m_spline->getTotalNodes() > 1)
			{
				m_shader_line.uniform("u_Color", m_nodeConnectCol);
				for (int i = 1; i < m_spline->getTotalNodes(); i++)
				{
					rend::vec3 a = m_spline->getNodePos(i - 1);
					rend::vec3 b = m_spline->getNodePos(i);
					m_shader_line.render_line(a, b);
				}
			}

			//Draw the nodes
			if (s_node && m_spline->getTotalNodes() != 0)
			{
				getEntity()->getTransform()->setScale(rend::vec3(0.2));
				m_shader_model.projection(projectionMat);
				m_shader_model.view(viewMat);
				m_shader_model.model(*m_nodeModel->getModel());

				for (int i = 0; i < m_spline->getTotalNodes(); i++)
				{
					getEntity()->getTransform()->setPos(m_spline->getNodePos(i));
					modelMat = getEntity()->getTransform()->getModelMat();
					m_shader_model.model(modelMat);
					m_shader_model.color(m_nodeCol);
					if (i == h_node)
					{
						m_shader_model.color(m_nodeHighlightCol);
					}
					m_shader_model.render();
				}
			}

			//Draw the roll nodes
			if (s_roll && m_spline->valid() && m_spline->getTotalRollNodes() != 0)
			{
				getEntity()->getTransform()->setScale(rend::vec3(0.05));
				m_shader_model.projection(projectionMat);
				m_shader_model.view(viewMat);
				m_shader_model.model(*m_rollModel->getModel());

				for (int i = 0; i < m_spline->getTotalRollNodes(); i++)
				{
					float pos = m_spline->getRollNodePos(i);
					getEntity()->getTransform()->setPos(m_spline->getPoint(pos));
					getEntity()->getTransform()->LookAt(m_spline->getVelocity(pos), rend::vec3(1, 0, 0));
					float angle = m_spline->getRollNodeAngle(i);
					getEntity()->getTransform()->Rotate(rend::vec3(0, 0, angle));
					modelMat = getEntity()->getTransform()->getModelMat();
					m_shader_model.model(modelMat);
					m_shader_model.color(m_rollCol);
					if (i == h_roll)
					{
						m_shader_model.color(m_rollHighlightCol);
					}
					m_shader_model.render();
				}
			}
			
		}
	}

	void SplineRenderer::onTick()
	{
	}

}