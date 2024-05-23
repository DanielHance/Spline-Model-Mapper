#include <rend/rend.h>

#include "Component.h"


namespace red
{
	struct Entity;

	struct Transform : Component
	{
	public:
		Transform();
		~Transform();

		//Getters and Setters
		rend::mat4 getModelMat();
		rend::vec3 getPos() { return m_pos; }
		rend::vec3 getScale() { return m_scale; }
		rend::vec3 getRotation() { return m_rotation; }
		void setPos(rend::vec3 _pos) { m_pos = _pos; }
		void setRotation(rend::vec3 _rotation) { m_rotation = _rotation; }
		void setScale(rend::vec3 _scale) { m_scale = _scale; }

		//Transform Functions
		void Move(rend::vec3 _move);
		void Rotate(rend::vec3 _rot);
		void Scale(rend::vec3 _scale) { m_scale += _scale; }
		void LookAt(rend::vec3 _target);
		void LookAt(std::weak_ptr<Entity> _target);
		void LookAt(rend::vec3 _target, rend::vec3 _forward);
		void LookAt(std::weak_ptr<Entity> _target, rend::vec3 _forward);
		void LookAt(rend::vec3 _target, rend::vec3 _forward, rend::vec3 _up);
		void LookAt(std::weak_ptr<Entity> _target, rend::vec3 _forward, rend::vec3 _up);

	private:
		rend::vec3 m_pos;
		rend::vec3 m_scale;
		rend::vec3 m_rotation;
	};

}