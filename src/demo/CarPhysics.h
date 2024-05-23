#include <rend/rend.h>
#include <RedEngine/RedEngine.h>
#include <iostream>

using namespace red;

struct CarPhysics : Component
{
public:
	CarPhysics();
	~CarPhysics();

	void onTick();

	//Getters and Setters
	void setSpline(std::shared_ptr<Spline> _spline);
	void setPos(float t);
	void setVelocity(float t);
	void setStepSize(float t);
	void setMass(float _mass);
	void setDrag(float _drag);
	void setPhysics(bool _flag);
	void setRollWrap(bool _wrap);
	float getPos();
	float getStepSize();
	float getMass();
	float getDrag();
	bool getPhysics();
	bool getRollWrap();


private:
	float getAcceleration(float slope, float mass, float grav, float damping);

	std::shared_ptr<Spline> m_spline;
	float m_stepSize;
	float m_velocity;
	float m_mass;
	float m_drag;
	float m_pos;
	bool m_physics;
	bool m_rollWrap;
};

