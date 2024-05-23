#include "Component.h"

#include <rend/rend.h>

namespace red
{
	struct Model;

	struct ModelRenderer : Component
	{
	public:
		ModelRenderer();
		~ModelRenderer();

		void onInit();

		//Getters and Setters
		void setModel(std::shared_ptr<Model> _model);
		void setShow(bool);
		bool getShow();

	private:
		
		std::shared_ptr<Model>  m_model;
		rend::ModelShader m_shader;
		bool m_show;
		//std::shared_ptr<Shader> m_shader = NULL;

		void onDisplay();
	};
}