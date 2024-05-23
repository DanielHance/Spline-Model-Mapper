#include "Transform.h"
#include "Entity.h"

namespace red
{
	Transform::Transform()
	{
		m_pos = rend::vec3(0.0f);
		m_rotation = rend::vec3(0.0f);
		m_scale = rend::vec3(1.0f, 1.0f, 1.0f);
	}

	Transform::~Transform()
	{
	}

	rend::mat4 Transform::getModelMat()
	{
		rend::mat4 modelMat(1.0f);
		modelMat = rend::translate(modelMat, m_pos);
		//Needs to be in order   y -> x -> z to get correct rotation with lookat() function
		modelMat = rend::rotate(modelMat, rend::radians(m_rotation.y), rend::vec3(0, 1, 0)); 
		modelMat = rend::rotate(modelMat, rend::radians(m_rotation.x), rend::vec3(1, 0, 0));
		modelMat = rend::rotate(modelMat, rend::radians(m_rotation.z), rend::vec3(0, 0, 1));
		modelMat = rend::scale(modelMat, m_scale);

		return modelMat;
	}

	void Transform::Rotate(rend::vec3 _rot)
	{
		m_rotation += _rot;
	}

	void Transform::Move(rend::vec3 _move)
	{
		m_pos += rend::vec3(getModelMat() * rend::vec4(_move, 0.0f));
	}

	void Transform::LookAt(rend::vec3 _target)
	{
		LookAt(_target, rend::vec3(1, 0, 0), rend::vec3(0, 1, 0));
	}

	void Transform::LookAt(std::weak_ptr<Entity> _target)
	{
		LookAt(_target.lock()->getTransform()->getPos(), rend::vec3(1, 0, 0), rend::vec3(0, 1, 0));
	}

	void Transform::LookAt(rend::vec3 _target, rend::vec3 _forward)
	{
		LookAt(_target, _forward, rend::vec3(0, 1, 0));
	}

	void Transform::LookAt(std::weak_ptr<Entity> _target, rend::vec3 _forward)
	{
		LookAt(_target.lock()->getTransform()->getPos(), _forward, rend::vec3(0, 1, 0));
	}

	void Transform::LookAt(std::weak_ptr<Entity> _target, rend::vec3 _forward, rend::vec3 _up)
	{
		LookAt(_target.lock()->getTransform()->getPos(), _forward, _up);
	}

	void Transform::LookAt(rend::vec3 _target, rend::vec3 _forward, rend::vec3 _up)
	{
		rend::vec3 forward = glm::normalize(_forward);
		rend::vec3 targetDirection = glm::normalize(_target - rend::vec3(0));
		rend::vec3 myUp(0, 1, 0);

		float yaw = glm::degrees(std::atan2(targetDirection.x, targetDirection.z));
		if (yaw < 0)
		{
			yaw += 360;
		}
		float pitch = glm::degrees(glm::asin(-targetDirection.y));
		
		if (_forward.x == -1) yaw += 180;
		if (_forward.z == 1) yaw += 90;
		if (_forward.z == -1) yaw += 270;
		setRotation(rend::vec3(pitch, yaw, 0));
	}
}

