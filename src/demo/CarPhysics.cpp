#include "CarPhysics.h"

CarPhysics::CarPhysics()
{
	m_stepSize = 0.05f;
	m_pos = 0;
	m_velocity = 0;
	m_drag = 10;
	m_mass = 9;
	m_physics = true;
	m_rollWrap = true;
}

CarPhysics::~CarPhysics()
{
}

void CarPhysics::setSpline(std::shared_ptr<Spline> _spline)
{
	m_spline = _spline;
}

void CarPhysics::setPos(float _t)
{
	if (_t >= m_spline->getMinKnot() && _t <= m_spline->getMaxKnot())
	{
		m_pos = _t;
	}
}

void CarPhysics::setVelocity(float _t)
{
	m_velocity = _t;
}

void CarPhysics::setStepSize(float _step)
{
	m_stepSize = _step;
}

void CarPhysics::setMass(float _mass)
{
	if (_mass > 0)
	{
		m_mass = _mass;
	}
}

void CarPhysics::setDrag(float _drag)
{
	m_drag = _drag;
}

void CarPhysics::setPhysics(bool _flag)
{
	m_physics = _flag;
}

void CarPhysics::setRollWrap(bool _wrap)
{
	m_rollWrap = _wrap;
}

float CarPhysics::getPos()
{
	return m_pos;
}

float CarPhysics::getStepSize()
{
	return m_stepSize;
}

float CarPhysics::getMass()
{
	return m_mass;
}

float CarPhysics::getDrag()
{
	return m_drag;
}

bool CarPhysics::getPhysics()
{
	return m_physics;
}

bool CarPhysics::getRollWrap()
{
	return m_rollWrap;
}

void CarPhysics::onTick()
{
	rend::vec3 tan = m_spline->getVelocity(m_pos);

	if (m_physics)
	{
		float grav = 9.8;
		float drag = m_drag;
		float boost = m_spline->getBoost(m_pos);
		float limiter = 500; //Divide everything by this to prevent stuff moving too fast

		if (tan.y > 0)
		{
			drag = -drag;
		}

		float hyp = glm::length(tan);
		float opp = glm::length(tan - rend::vec3(tan.x, 0, tan.z));
		float slope = sin(opp / hyp);

		float acceleration = getAcceleration(slope, m_mass, grav, drag);

		//If the slope is going up the acceleration should be negative 
		if (tan.y > 0)
		{
			acceleration = -acceleration;
		}

		m_velocity += acceleration / limiter;

		//Check if boost needs to be add
		if (boost != 0)
		{
			if ((boost < 0 && boost < m_velocity) || (boost > 0 && boost > m_velocity))
			{
				m_velocity = boost;
			}
		}

		if (m_velocity < 0.001 && m_velocity > -0.001)
		{
			m_velocity = 0;
		}

		m_pos = m_spline->stepFast(m_pos, m_velocity * getCore().lock()->getDelta()); 
		
	}
	else
	{
		m_pos += m_stepSize * getCore().lock()->getDelta();
		
	}

	//Wrap aroudn when reaching end of spline
	if (m_pos >= m_spline->getMaxKnot())
	{
		m_pos = m_spline->getMinKnot() + 0.001;
	}
	else if (m_pos <= m_spline->getMinKnot())
	{
		m_pos = m_spline->getMaxKnot() - 0.001;
	}
	

	//Move Model
	getEntity()->getTransform()->setPos(m_spline->getPoint(m_pos));

	//Rotate the model
	float roll = m_spline->getRoll(m_pos, m_rollWrap);
	getEntity()->getTransform()->LookAt(tan);
	getEntity()->getTransform()->Rotate(rend::vec3(0, 0, roll));

	//DEBUG STUFF
	// rend::vec3 nor = m_spline->getNormal(m_pos);
	//rend::vec3 bi = m_spline->getBinormal(m_pos);
	//rend::vec3 pri = m_spline->getPrincipal(m_pos);
	//std::cout << m_pos << std::endl;
	//std::cout << "Speed = " << m_velocity << std::endl;
	//std::cout << "Roll = " << roll << std::endl;
	//std::cout << "tan = " << tan.x << ", " << tan.y << ", " << tan.z << std::endl;
	//std::cout << "nor = " << nor.x << ", " << nor.y << ", " << nor.z << std::endl;
	//std::cout << "bi = " << bi.x << ", " << bi.y << ", " << bi.z << std::endl;
	//std::cout << "pri = " << pri.x << ", " << pri.y << ", " << pri.z << std::endl;
	//std::cout << "pitch = " << getEntity()->getTransform()->getRotation().y << std::endl;
}


float CarPhysics::getAcceleration(float slope, float mass, float grav, float damping)
{
	float cosTheta = -slope / glm::sqrt(1 + slope * slope);

	//Handle vertical slopes (infinae slope)
	if (isinf(slope))
	{
		return -grav - (damping / mass) * m_velocity;
	}

	return -grav * cosTheta - (damping / mass) * m_velocity;
}