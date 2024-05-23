#include <rend/rend.h>

#include "Component.h"

namespace red
{
	struct Camera : Component
	{
	public:

	Camera();
	~Camera();

	rend::mat4 getView();
	};
}