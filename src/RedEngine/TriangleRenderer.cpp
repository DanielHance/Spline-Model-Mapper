#include "TriangleRenderer.h"
#include "Entity.h"
#include "Transform.h"

namespace red
{

	TriangleRenderer::TriangleRenderer()
	{
	}


	TriangleRenderer::~TriangleRenderer()
	{
	}


	void TriangleRenderer::onInit()
	{
		//Set Pos
		getEntity()->getTransform()->setPos(rend::vec3(0, 0, -5));

		m_mesh.load(rend::TRIANGLE_MESH);
		m_shader.load(rend::COLOR_SHADER);

		m_shader.attribute("a_Position", *m_mesh.positions());

		m_shader.uniform("u_Projection", rend::perspective(rend::radians(45.0F), 1.0f, 0.1f, 100.0f));

		m_shader.uniform("u_Model", getEntity()->getTransform()->getModelMat());

		m_shader.uniform("u_Color", rend::vec4(1, 0, 0, 1));

		m_angle = 0.f;
	}

	void TriangleRenderer::onDisplay()
	{
		m_shader.uniform("u_Model", getEntity()->getTransform()->getModelMat());
		m_shader.render();
	}
}