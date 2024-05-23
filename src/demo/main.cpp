#include <iostream>
#include <rend/rend.h>
#include <RedEngine/RedEngine.h>

#include "Menu.h"
#include "FollowCam.h"
#include "CarPhysics.h"
#include "Track.h"

using namespace red;

#undef main
int main()
{
	std::shared_ptr<Core> core = Core::initialize();
	std::string defultSpline = "splines/testspline.txt";
	
	//Set up camera/menu
	std::shared_ptr<Entity> cam = core->addEntity();
	cam->addComponent<Camera>();
	cam->addComponent<FollowCam>();
	core->setCamera(cam);

	//Splien Renderer
	std::shared_ptr<Entity> spline1 = core->addEntity();
	std::shared_ptr<SplineRenderer> sr1 = spline1->addComponent<SplineRenderer>();
	sr1->setSpline(core->getResources()->load<Spline>(defultSpline));
	sr1->setDetail(0.01);

	//Coaster Car
	std::shared_ptr<Entity> car1 = core->addEntity();
	std::shared_ptr<ModelRenderer> mr1 = car1->addComponent<ModelRenderer>();
	mr1->setModel(core->getResources()->load<Model>("models/Car/Car_Low.obj"));
	car1->getTransform()->setScale(rend::vec3(0.15, 0.15, 0.15));
	std::shared_ptr<CarPhysics> cp1 = car1->addComponent<CarPhysics>();
	cp1->setSpline(core->getResources()->load<Spline>(defultSpline));
	cp1->setPos(3.5f);
	cp1->setStepSize(0.01f);
	car1->getTransform()->setRotation(rend::vec3(0, 180, 0));

	//Track 
	std::shared_ptr<Entity> model1 = core->addEntity();
	std::shared_ptr<Track> track1 = model1->addComponent<Track>();
	track1->setSpline(core->getResources()->load<Spline>(defultSpline));
	track1->setScale(0.002);
	std::shared_ptr<ModelRenderer> mr2 = model1->addComponent<ModelRenderer>();
	mr2->setModel(core->getResources()->load<Model>("models/HalfLife/scientists/walter/walter.obj"));
	mr2->setShow(false);

	//Adds GUI stuff to camera entity
	std::shared_ptr<Menu> GUI = cam->addComponent<Menu>();
	GUI->setSpline(core->getResources()->load<Spline>(defultSpline));
	GUI->setSplineRenderer(sr1);
	GUI->setTrackRenderer(mr2);
	GUI->setCarRenderer(mr1);
	GUI->setCarPhysics(cp1);
	GUI->setTrack(track1);


	try
	{
		core->start();

	}
	catch(std::exception& e)
	{
		std::cout << "Exc: " << e.what() << std::endl;
	}

	return 0;
}
