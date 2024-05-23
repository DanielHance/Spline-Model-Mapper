#include "Component.h"

#include <rend/rend.h>

namespace red
{
	struct RigidBody : Component
	{
	public:
		RigidBody();
		~RigidBody();

		void onTick();

	private:


	};
}