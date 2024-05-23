#include "MeshCollider.h"
#include "Entity.h"
#include "Transform.h"
#include "Core.h"
#include "Model.h"
#include "tritri.h"
#include "BoxCollider.h"
#include <iostream>
#include <math.h>
#ifdef __EMSCRIPTEN__
#else
#include <thread>
#endif


namespace red
{

	MeshCollider::MeshCollider()
	{
	}


	MeshCollider::~MeshCollider()
	{
	}

	bool MeshCollider::isColliding(Collider& _other)
	{
		bool rtn = false;

		if (dynamic_cast<BoxCollider*>(&_other) != nullptr)
		{
			rtn = boxCollider(*_other.getEntity()->getComponent<BoxCollider>());
		}
		else
		{
			rtn = meshCollide(*_other.getEntity()->getComponent<MeshCollider>());
		}
	
		return rtn;
	}


	bool MeshCollider::meshCollide(MeshCollider& _other)
	{
		std::shared_ptr<std::vector<rend::Face>> faces1 = getModel()->getFaces();
		std::shared_ptr<std::vector<rend::Face>> faces2 = _other.getModel()->getFaces();
		rend::mat4 mat1 = getEntity()->getTransform()->getModelMat();
		rend::mat4 mat2 = _other.getEntity()->getTransform()->getModelMat();

		for (int f1 = 0; f1 != faces1->size(); f1++)
		{
	
			for (int f2 = 0; f2 != faces2->size(); f2++)
			{
				rend::vec3 temp;

				temp = toWorld(faces1->at(f1).a.position, mat1);
				float a1[3] = { temp.x, temp.y, temp.z };
				temp = toWorld(faces1->at(f1).b.position, mat1);
				float a2[3] = { temp.x, temp.y, temp.z };
				temp = toWorld(faces1->at(f1).c.position, mat1);
				float a3[3] = { temp.x, temp.y, temp.z };

				temp = toWorld(faces2->at(f2).a.position, mat2);
				float b1[3] = { temp.x, temp.y, temp.z };
				temp = toWorld(faces2->at(f2).b.position, mat2);
				float b2[3] = { temp.x, temp.y, temp.z };
				temp = toWorld(faces2->at(f2).c.position, mat2);
				float b3[3] = { temp.x, temp.y, temp.z };

				if (NoDivTriTriIsect(a1, a2, a3, b1, b2, b3))
				{
					//std::cout << "Collide!" << std::endl;
					return true;
				}
			}
		}
		return false;
	}


	bool MeshCollider::boxCollider(BoxCollider& _other)
	{
		//Full mesh bounding box test
		rend::vec3 boxMax = _other.getEntity()->getTransform()->getPos() + _other.getSize() / 2.0f;
		rend::vec3 boxMin = _other.getEntity()->getTransform()->getPos() - _other.getSize() / 2.0f;
		std::shared_ptr<std::vector<rend::Face>> faces1 = getModel()->getFaces();
		rend::mat4 mat1 = getEntity()->getTransform()->getModelMat();

		//Mesh bounding box test
		if (aabbTest(boxMax, boxMin, toWorld(getModel()->getMax(), mat1), toWorld(getModel()->getMin(), mat1)));
		{
			//Face bounding box test
			for (int f1 = 0; f1 != faces1->size(); f1++)
			{
				if (aabbTest(boxMax, boxMin, toWorld(faces1->at(f1).max, mat1), toWorld(faces1->at(f1).min, mat1)));
				{
					return true;
				}
			}
		}
		return false;
	}

	rend::Face MeshCollider::convertToWorld(rend::Face _face, rend::mat4 _mat)
	{
		rend::Face _rtn;
		_rtn.a.position = _mat * rend::vec4(_face.a.position, 1);
		_rtn.b.position = _mat * rend::vec4(_face.b.position, 1);
		_rtn.c.position = _mat * rend::vec4(_face.c.position, 1);
		_rtn.max = _mat * rend::vec4(_face.max, 1);
		_rtn.min = _mat * rend::vec4(_face.min, 1);
		return _rtn;
		//Note this function only updates the positions in the vertexs and not the textrure coords or normals
	}

	rend::vec3 MeshCollider::toWorld(rend::vec3 _pos, rend::mat4 _mat)
	{
		return _mat * rend::vec4(_pos, 1);
	}
}