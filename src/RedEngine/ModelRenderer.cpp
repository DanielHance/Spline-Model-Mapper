#include "ModelRenderer.h"
#include "Entity.h"
#include "Core.h"
#include "Camera.h"
#include "Transform.h"
#include "rend/rend.h"
#include "Model.h"

namespace red
{

	ModelRenderer::ModelRenderer()
	{
		m_show = true;
	}


	ModelRenderer::~ModelRenderer()
	{
	}

	void ModelRenderer::setModel(std::shared_ptr<Model> model)
	{
		m_model = model;
	}

	void ModelRenderer::setShow(bool _show)
	{
		m_show = _show;
	}

	bool ModelRenderer::getShow()
	{
		return m_show;
	}

	void ModelRenderer::onInit()
	{	
	}

	void ModelRenderer::onDisplay()
	{
		if (m_show)
		{
			//Model -> world -> clip -> Projection
			rend::mat4 modelMat(getEntity()->getTransform()->getModelMat());
			rend::mat4 viewMat(getCore().lock()->getCamera()->getView());
			rend::mat4 projectionMat(rend::perspective(rend::radians(45.0f), 1.0f, 0.1f, 100.0f));

			m_shader.projection(projectionMat);
			m_shader.view(viewMat);
			m_shader.model(modelMat);
			m_shader.model(*m_model->getModel());
			//m_shader.backface_cull(false);
			m_shader.render();
		}
	}
}