#include "Component.h"

namespace red
{

Component::Component()
{
	m_alive = true;
}


Component::~Component()
{
}


void Component::onInit()
{
	//std::cout << "onInit() called!" << std::endl;
}


void Component::onBegin()
{
	//std::cout << "onBegin() called!" << std::endl;
}


void Component::tick()
{
	onTick();
}


void Component::onTick()
{
	//std::cout << "onTick() called!" << std::endl;
}


void Component::display()
{
	onDisplay();
}


void Component::onDisplay()
{
	//std::cout << "onDisplay() called!" << std::endl;
}

void Component::gui()
{
	onGUI();
}

void Component::onGUI()
{

}

}