#include <iostream>
#include <rend/rend.h>
#include <RedEngine/RedEngine.h>

using namespace red;

struct FollowCam : Component
{
public:
	FollowCam();
	~FollowCam();

	void setTarget(std::shared_ptr<Entity> _target);
	void onTick();

private:
	std::shared_ptr<Entity> Player;
};