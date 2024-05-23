#include "Collider.h"

#include <rend/rend.h>

namespace red
{

	struct MeshCollider;

	struct BoxCollider : Collider
	{
	public:
		BoxCollider();
		~BoxCollider();

		bool isColliding(Collider& _other);

	private:
		bool boxCollide(BoxCollider& _other);
		bool meshCollide(MeshCollider& _other);

	};
}