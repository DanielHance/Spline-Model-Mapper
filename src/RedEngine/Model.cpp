#include "Model.h"

namespace red
{
	void Model::onLoad()
	{
		m_model = std::make_shared<rend::Model>(getPath().c_str());
		
		m_faces = std::make_shared<std::vector<rend::Face>>(m_model->getFaces());
		m_max = m_model->getMax();
		m_min = m_model->getMin();
	}
}