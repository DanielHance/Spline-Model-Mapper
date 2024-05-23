#include "Collider.h"

#include <rend/rend.h>
#include <vector>

namespace red
{
	struct Model;
	struct BoxCollider;

	struct MeshCollider : Collider
	{
	public:
		MeshCollider();
		~MeshCollider();
		bool isColliding(Collider& _other);

		//Getter and Setters
		void setModel(std::shared_ptr<Model> _model) { m_model = _model; }
		std::shared_ptr<Model> getModel() { return m_model; }

	private:
		friend struct boxCollider;
		std::shared_ptr<Model> m_model;

		bool meshCollide(MeshCollider& _other);
		bool boxCollider(BoxCollider& _other);
		rend::Face convertToWorld(rend::Face _face, rend::mat4 _mat);
		rend::vec3 toWorld(rend::vec3 _pos, rend::mat4 _mat);
		bool m_marker;
		
	};
}