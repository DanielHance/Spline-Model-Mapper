#include "Resource.h"
#include <vector>

namespace red
{
	struct ModelRenderer;

	struct Model : Resource
	{
	public:
		void onLoad();
	
		//Getters and Setters
		std::shared_ptr<rend::Model> getModel() { return m_model; }
		std::shared_ptr<std::vector<rend::Face>> getFaces() { return m_faces; }
		glm::vec3 getMax() { return m_max; }
		glm::vec3 getMin() { return m_min; }

	private:
		std::shared_ptr<rend::Model> m_model;
		std::shared_ptr<std::vector<rend::Face>> m_faces;
		rend::vec3 m_max;
		rend::vec3 m_min;
	};

}