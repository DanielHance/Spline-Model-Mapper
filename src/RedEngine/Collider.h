#pragma once
#include "Component.h"

#include <rend/rend.h>

namespace red
{
	struct Collider: Component
	{
	public:
		Collider();
		~Collider();

		virtual bool isColliding(Collider& _other) = 0;

		//Getter and Setters
		void setSize(rend::vec3 _size) { m_size = _size; }
		void setOffset(rend::vec3 _offset) { m_offset = _offset; }
		rend::vec3 getSize() { return m_size; }
		rend::vec3 getOffset() { return m_offset; }

		void onInit();

	protected:
		rend::vec3 m_size;
		rend::vec3 m_offset;

		bool aabbTest(rend::vec3 _aMax, rend::vec3 _aMin, rend::vec3 _bMax, rend::vec3 _bMin);

	};
}