#include "Component.h"

#include <rend/rend.h>

namespace red
{
	struct TriangleRenderer : Component
	{
	public:
		TriangleRenderer();
		~TriangleRenderer();

		void onInit();

	private:
		rend::Mesh m_mesh;
		rend::Shader m_shader;

		float m_angle;

		void onDisplay();
	};
}