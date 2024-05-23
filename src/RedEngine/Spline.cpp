#include "Spline.h"
#include <fstream>
#include <algorithm>

namespace red
{
	Spline::Spline()
	{
		m_degree = 3;
		m_nodes = {};
		m_knots = {};
		m_rollNodes = {};
		m_clamp = false;
	}

	Spline::~Spline()
	{
	}

	void Spline::onLoad()
	{

		if (!getData(getPath().c_str()))
		{
			//Defult spline file for new splines
			addNode(rend::vec3(0, 0, 0));
			addNode(rend::vec3(10, 0, 0));
			addNode(rend::vec3(10, 0, 10));
			addNode(rend::vec3(0, 0, 10));
			std::ofstream newfile(getPath().c_str());
			setData(getPath().c_str());
			std::cout << "File Created!" << std::endl;
		}
		else
		{
			std::cout << "Data Loaded!" << std::endl;
		}
	}

	bool Spline::getData(std::string _path)
	{
		float x, y, z;
		char type;
		std::ifstream file;

		file.open(_path);
		if (file.fail())
		{
			std::cout << "Failed to open spline file - " << _path << std::endl;
			return false;
		}

		//Removes old data
		m_nodes.clear(); 
		m_rollNodes.clear();
		m_boosters.clear();

		while (file >> type >> x >> y >> z)
		{
			if (type == 'n')
			{
				m_nodes.push_back(rend::vec3(x, y, z));
			}
			if (type == 'r')
			{
				m_rollNodes.push_back(rend::vec2(x, y));
			}
			if (type == 'b')
			{
				m_boosters.push_back(rend::vec3(x, y, z));
			}
			if (type == 'd')
			{
				m_degree = x;
				if (y == 1)
				{
					m_clamp = true;
				}
				else
				{
					m_clamp = false;
				}
			}
		}
		file.close();

		genKnotVec();
		orderRollNodes();

		return true;
	}

	bool Spline::setData(std::string _path)
	{
		std::ofstream file(_path);
		
		int clamp = 0;
		if (m_clamp)
		{
			clamp = 1;
		}
		file << "d " << m_degree << " " << clamp << " 0\n";

		for (int i = 0; i < m_nodes.size(); i++)
		{
			file << "n " << m_nodes[i].x << " " << m_nodes[i].y << " " << m_nodes[i].z << "\n";
		}

		for (int i = 0; i < m_rollNodes.size(); i++)
		{
			file << "r " << m_rollNodes[i].x << " " << m_rollNodes[i].y << " 0\n";
		}

		for (int i = 0; i < m_boosters.size(); i++)
		{
			file << "b " << m_boosters[i].x << " " << m_boosters[i].y << " " << m_boosters[i].z << "\n";
		}

		return true;
	}

	void Spline::loadFile()
	{
		if (getData(getPath().c_str()))
		{
			std::cout << "Spline data loaded!" << std::endl;
		}
		else
		{
			std::cout << "Spline data faield to load!" << std::endl;
		}
	}

	void Spline::saveFile()
	{
		if (setData(getPath().c_str()))
		{
			std::cout << "Spline data saved!" << std::endl;
		}
		else
		{
			std::cout << "Spline data failed to save!" << std::endl;
		}
	}

	int Spline::getDegree()
	{
		return m_degree;
	}

	void Spline::setDegree(int _degree)
	{
		if (_degree < 1)
		{
			//std::cout << "Invalid degree entered!" << std::endl;
			return;
		}
		m_degree = _degree;
		genKnotVec();
	}

	bool Spline::valid()
	{
		if (m_degree < m_nodes.size())
		{
			return true;
		}
		return false;
	}

	void Spline::addNode(rend::vec3 _pos)
	{
		m_nodes.push_back(_pos);
		genKnotVec();
	}

	void Spline::addNode(rend::vec3 _pos, int _index)
	{
		if (m_nodes.size() == 0 || _index < 0 || _index > m_nodes.size() - 1)
		{
			addNode(_pos);
			return;
		}
		m_nodes.insert(m_nodes.begin() + _index, _pos);
		genKnotVec();
	}

	void Spline::addRollNode(float t, float degree)
	{
		if (t > getMaxKnot() || t < getMinKnot())
		{
			//std::cout << t << " is not a valid point on the spline so can't create roll node!" << std::endl;
			return;
		}
		m_rollNodes.push_back(rend::vec2(t, degree));
		orderRollNodesOrder();
	}

	void Spline::removeNode()
	{
		if (m_nodes.size() == 0)
		{
			return;
		}
		m_nodes.pop_back();
		genKnotVec();
	}

	void Spline::removeNode(int _index)
	{
		if (m_nodes.size() == 0 || _index < 0 || _index >= m_nodes.size() - 1)
		{
			removeNode();
			return;
		}
		m_nodes.erase(m_nodes.begin() + _index);
		genKnotVec();
	}

	void Spline::removeRollNode()
	{
		if (m_rollNodes.size() == 0)
		{
			return;
		}
		m_rollNodes.pop_back();
		orderRollNodesOrder();
	}

	void Spline::removeRollNodePoint(float t)
	{
		if (m_rollNodes.size() != 0)
		{
			for (int i = 0; i < m_rollNodes.size(); i++)
			{
				if (m_rollNodes[i].x == t)
				{
					m_rollNodes.erase(m_rollNodes.begin() + i);
					orderRollNodesOrder();
				}
			}
		}
	}

	void Spline::removeRollNode(int _index)
	{
		if (m_rollNodes.size() == 0 || _index < 0 || _index > m_rollNodes.size() - 1)
		{
			removeRollNode();
			return;
		}
		m_rollNodes.erase(m_rollNodes.begin() + _index);
		orderRollNodesOrder();
	}

	void Spline::addBooster(float _start, float _end, float _speed)
	{
		//If wrap around make 2 booster to simulate 1
		if (_end < _start)
		{
			addBooster(getMinKnot(), _end, _speed);
			addBooster(_start, getMaxKnot(), _speed);
			return;
		}
		m_boosters.push_back(rend::vec3(_start, _end, _speed));
	}

	void Spline::removeBooster(int _index)
	{
		if (m_boosters.size() == 0 || _index < 0 || _index > m_boosters.size() - 1)
		{
			//std::cout << "Warning - removeBooster() failed as index was out of range!" << std::endl;
			return;
		}
		m_boosters.erase(m_boosters.begin() + _index);
	}

	rend::vec2 Spline::getBoosterPos(int _index)
	{
		if (m_boosters.size() == 0 || _index < 0 || _index > m_boosters.size() - 1)
		{
			//std::cout << "Warning - getBoosterPos() failed as invalid index was out of range!" << std::endl;
			return rend::vec2(0);
		}
		return rend::vec2(m_boosters[_index].x, m_boosters[_index].y);
	}

	float Spline::getBoosterSpeed(int _index)
	{
		if (m_boosters.size() == 0 || _index < 0 || _index > m_boosters.size() - 1)
		{
			//std::cout << "Warning - getBoosterSpeed() failed as invalid index was out of range!" << std::endl;
			return 0;
		}
		return m_boosters[_index].z;
	}

	void Spline::setBoosterPos(rend::vec2 _pos, int _index)
	{
		if (m_boosters.size() == 0 || _index < 0 || _index > m_boosters.size() - 1)
		{
			return;
		}
		if (_pos.x > getMaxKnot()) _pos.x = getMaxKnot();
		if (_pos.y > getMaxKnot()) _pos.y = getMaxKnot();
		if (_pos.x < getMinKnot()) _pos.x = getMinKnot();
		if (_pos.y < getMinKnot()) _pos.y = getMaxKnot();
		if (_pos.x > _pos.y)
		{
			float temp = _pos.x;
			_pos.x = _pos.y;
			_pos.y = temp;
		}
		m_boosters[_index].x = _pos.x;
		m_boosters[_index].y = _pos.y;
	}

	void Spline::setBoosterSpeed(float _speed, int _index)
	{
		if (m_boosters.size() == 0 || _index < 0 || _index > m_boosters.size() - 1)
		{
			return;
		}
		m_boosters[_index].z = _speed;
	}

	int Spline::getTotalNodes()
	{
		return m_nodes.size();
	}

	int Spline::getTotalKnots()
	{
		return m_knots.size();
	}

	int Spline::getTotalRollNodes()
	{
		return m_rollNodes.size();
	}

	int Spline::getTotalBoosters()
	{
		return m_boosters.size();
	}

	rend::vec3 Spline::getNodePos(int _index)
	{
		if (m_nodes.size() == 0 || _index < 0 || _index > m_nodes.size() - 1)
		{
			//std::cout << "Warning - getNodePos() failed as index was out of range!" << std::endl;
			return rend::vec3(0);
		}
		return m_nodes[_index];
	}

	float Spline::getRollNodePos(int _index)
	{
		if (m_rollNodes.size() == 0 || _index < 0 || _index > m_rollNodes.size() - 1)
		{
			//std::cout << "Warning - getRollNodePos() failed as index was out of range!" << std::endl;
			return 0;
		}
		return m_rollNodes[_index].x;
	}

	float Spline::getRollNodeAngle(int _index)
	{
		if (m_rollNodes.size() == 0 || _index < 0 || _index > m_rollNodes.size() - 1)
		{
			//std::cout << "Warning - getRollNodeAngle() failed as index was out of range!" << std::endl;
			return 0;
		}
		return m_rollNodes[_index].y;
	}

	void Spline::setRollNodePos(float _pos, int _index)
	{
		if (m_rollNodes.size() == 0 || _index < 0 || _index > m_rollNodes.size() - 1)
		{
			//std::cout << "Warning - setRollNodePos() failed as index was out of range!" << std::endl;
			return;
		}
		m_rollNodes[_index].x = _pos;
		orderRollNodesOrder();
	}

	void Spline::setRollNodeAngle(float _angle, int _index)
	{
		if (m_rollNodes.size() == 0 || _index < 0 || _index > m_rollNodes.size() - 1)
		{
			//std::cout << "Warning - setRollNodeAngle() failed as index was out of range!" << std::endl;
			return;
		}
		m_rollNodes[_index].y = _angle;
	}

	void Spline::setNodePos(rend::vec3 _pos, int _index)
	{
		if (m_nodes.size() == 0 || _index < 0 || _index > m_nodes.size() - 1)
		{
			//std::cout << "Warning - setNodePos() failed as index was out of range!" << std::endl;
			return;
		}
		m_nodes[_index] = _pos;
	}

	void Spline::moveNode(rend::vec3 _move, int _index)
	{
		if (m_nodes.size() == 0 || _index < 0 || _index > m_nodes.size() - 1)
		{
			//std::cout << "Warning - moveNode() failed as index was out of range!" << std::endl;
			return;
		}
		m_nodes[_index] += _move;
	}

	void Spline::makeLoop()
	{
		if (m_clamp)
		{
			std::cout << "Unclamp spline to create a loop!" << std::endl;
			return;
		}

		for (int i = 0; i < m_degree; i++)
		{
			addNode(m_nodes[i]);
		}
	}

	float Spline::getLength()
	{
		return getLength(getMinKnot(), getMaxKnot(), 0.001);
	}

	float Spline::getLength(float _step)
	{
		return getLength(getMinKnot(), getMaxKnot(), _step);
	}

	float Spline::getLength(float _start, float _end)
	{
		return getLength(_start, _end, 0.001);
	}

	float Spline::getLength(float _start, float _end, float _step)
	{
		if (m_nodes.size() == 0)
		{
			return 0;
		}

		bool flip = false;
		float stepSize = _step;
		float step = 0;
		rend::vec3 a(getPoint(_start));
		rend::vec3 b(0);
		float length = 0;
		
		if (_end < _start)
		{
			float temp = _end;
			_end = _start;
			_start = temp;
			flip = true;
		}

		if (getMaxKnot() < _end || getMinKnot() > _start)
		{
			//std::cout << "Spline getLength() input out of range!" << std::endl;
			return 0;
		}



		while (_start + step < _end)
		{
			step += stepSize;
			b = a;
			a = getPoint(step + _start);
			length += sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
		}

		if (flip)
		{
			length = -length;
		}

		return length;
	}

	float Spline::step(float _start, float _step)
	{
		return step(_start, _step, 0.001, false);
	}

	float Spline::step(float _start, float _step, float _error)
	{
		return step(_start, _step, _error, false);
	}

	float Spline::stepFast(float _start, float _step)
	{
		return step(_start, _step, 0.01, true);
	}

	float Spline::step(float _start, float _target, float _error, bool _fast)
	{
		if (m_nodes.size() == 0)
		{
			return 0;
		}

		float t = getMaxKnot();
		float distance = 0;
		float gap = fabs(_error) / 10;    
		float stepSize = (t - _start) / 2;
		_error = fabs(_error);              
		int i = 0;
		bool negative = false;

		//Check inputs to prevent infinate loop
		if (_start > getMaxKnot() || _start < getMinKnot())
		{
			//std::cout << "error --- invalid start value enter in spline step function!" << std::endl;
			return 0;
		}
		if (_target == 0)
		{
			return _start;
		}
		if (_error == 0)
		{
			_error = fabs(_target) / 10;
		}
		if (gap < 0.0005)
		{
			gap = 0.00005;
		}
		if (_target < 0)
		{
			t = getMinKnot();
			stepSize = (_start - t) / 2;
		}

		//Binary Search
		while (true)
		{
			distance = getLength(_start, t, gap);
			
			//Check if distance is in range to exit loop
			if (std::abs(distance - _target) < _error)
			{
				return t;
			}

			//Update the t position
			if (distance > _target)
			{
				t -= stepSize;
				if (t < getMinKnot()) t = getMinKnot();
				
			}
			else
			{
				t += stepSize;
				if (t > getMaxKnot()) t = getMaxKnot();
			}

			stepSize *= 0.5;
			i++;

			//Safty exit to stop infinate loop 
			if (i > 30)
			{
				//std::cout << "SLOW" << std::endl;
				return t;
			}
		}
	}

	float Spline::getMinKnot()
	{
		if (m_knots.size() == 0)
		{
			return 0;
		}
		if (m_clamp)
		{
			return m_knots.front();
		}
		else
		{
			return m_knots[m_degree];
		}
	}

	float Spline::getMaxKnot()
	{
		if (m_knots.size() == 0)
		{
			return 0;
		}
		if (m_clamp)
		{
			return m_knots.back();
		}
		else
		{
			return m_knots[m_knots.size() - (m_degree + 1)];
		}
	}

	void Spline::setClamp(bool _clamp)
	{
		m_clamp = _clamp;
		genKnotVec();
	}

	bool Spline::getClamp()
	{
		return m_clamp;
	}

	void Spline::genKnotVec()
	{
		//Needs n + degree + 1 knot values (with the frist and last degree number of knots beeting the same to clamp the spline if that is selected)
		// 
		// Clamped      degree = 3    n = 7         knots = [0, 0, 0, 0, 1, 2, 3, 4, 4, 4, 4]
		// UnClamped    degree = 3    n = 7         knots = [-3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7]
		// 
		//(I store the degree as one less than it is beacuse I count 0 as a value so degree 3 = 4th degree)
		//(Unclapped needs to be made smaller to make teh t values in the middle of the spline watch with the clamped version) 

		m_knots.clear(); //Remove old knots
		int n = m_nodes.size(); //(n + 1)

		if (n != 0)
		{
			//UnClaped Knots
			if (!m_clamp)
			{
				for (int i = 0; i < n + m_degree + 1; i++)
				{
					m_knots.push_back(i - m_degree);
				}
			}
			//Clamped Knots
			else
			{
				//Extra start clamp knots
				for (int i = 0; i < m_degree; i++)
				{
					m_knots.push_back(0);
				}

				//main knots
				for (int i = 0; i < n - (m_degree - 1); i++)
				{
					m_knots.push_back(i);
				}

				//Extra end clamp knots
				for (int i = 0; i < m_degree; i++)
				{
					m_knots.push_back(m_nodes.size() - m_degree);
				}
			}
		}
		//Check the roll nodes and boosters to make sure they are still all in the range
		if (m_knots.size() != 0)
		{
			for (int i = 0; i < m_rollNodes.size(); i++)
			{
				if (m_rollNodes[i].x > m_knots[m_knots.size() - m_degree - 1])
				{
					auto index = m_rollNodes.begin() + i;
					m_rollNodes.erase(index);
					i--;
				}
			}
			orderRollNodesOrder();

			for (int i = 0; i < m_boosters.size(); i++)
			{
				if (m_boosters[i].y > m_knots[m_knots.size() - m_degree] - 1)
				{
					if (m_boosters[i].x > m_knots[m_knots.size() - m_degree] - 1)
					{
						//Remove boost
						auto index = m_boosters.begin() + i;
						m_boosters.erase(index);
						i--;
					}
					else
					{
						//Resize boost
						m_boosters[i].y = m_knots[m_knots.size() - m_degree - 1];
					}
				}
			}
		}
	}

	rend::vec3 Spline::getPoint(float t)
	{
		int n = m_nodes.size(); //n+1
		rend::vec3 rtn(0);

		if (n < m_degree + 1)
		{
			return rtn; //To few control points
		}

		for (int i = 0; i < n; i++)
		{
			double weight = basis(i, m_degree, t); //Ni,k(t)
			//std::cout << "weight = " << weight << std::endl;

			rtn.x += weight * m_nodes[i].x;
			rtn.y += weight * m_nodes[i].y;
			rtn.z += weight * m_nodes[i].z;
		}

		return rtn;
	}

	rend::vec3 Spline::getVelocity(float t)
	{
		/*
		* This is basicaly the driviative of the basis function however it can't be made its own private function as it needs two 
		control point values and passing all that into a single function is annoying so I added it here instead!
		*/

		int n = m_nodes.size(); //n+1
		rend::vec3 rtn(0);

		if (n < m_degree + 1)
		{
			return rtn; //To few control points
		}

		for (int i = 0; i < n; i++)
		{
			double weight = basisDeriv(i, m_degree, 1, t); //First Derivative
			rtn.x += weight * m_nodes[i].x;
			rtn.y += weight * m_nodes[i].y;
			rtn.z += weight * m_nodes[i].z;
		}

		if (rtn.length != 0)
		{
			rtn = glm::normalize(rtn);
		}

		return rtn;
	}

	rend::vec3 Spline::getNormal(float t)
	{
		rend::vec3 up(0, 1, 0);
		rend::vec3 tan = getVelocity(t);
		rend::vec3 normal = glm::normalize(glm::cross(tan, up));

		return normal;
	}

	rend::vec3 Spline::getBinormal(float t)
	{
		rend::vec3 tan = getVelocity(t);
		rend::vec3 nor = getNormal(t);
		rend::vec3 binormal = glm::normalize(glm::cross(tan, nor));
		return binormal;
	}

	rend::vec3 Spline::getPrincipal(float t)
	{
		rend::vec3 tan = getVelocity(t);
		rend::vec3 bi = getBinormal(t);
		rend::vec3 principal = glm::normalize(glm::cross(tan, bi));
		return principal;
	}

	rend::vec3 Spline::getAcceleration(float t)
	{
		int n = m_nodes.size();
		rend::vec3 rtn(0);

		if (n < m_degree + 1)
		{
			return rtn; //To few control points
		}

		rend::vec3 tangent = getVelocity(t);
		rend::vec3 der2(0);

		for (int i = 0; i < n; i++)
		{
			double weight = basisDeriv(i, m_degree, 2, t); //Second Derivative
			der2.x += weight * m_nodes[i].x;
			der2.y += weight * m_nodes[i].y;
			der2.z += weight * m_nodes[i].z;
		}

		rtn = glm::cross(tangent, der2);
	
		if (rtn.length != 0)
		{
			rtn = glm::normalize(rtn);
		}

		return rtn;
	}

	rend::vec3 Spline::getTorsion(float t)
	{
		rend::vec3 rtn(0);

		if (m_nodes.size() < m_degree + 1)
		{
			return rtn; //To few control points
		}

		rend::vec3 normal = getAcceleration(t);
		rend::vec3 tangent = getVelocity(t);

		rtn = glm::cross(tangent, normal);

		if (rtn.length != 0)
		{
			rtn = glm::normalize(rtn);
		}

		return rtn;
	}

	float Spline::getRoll(float t)
	{
		return getRoll(t, false);
	}

	float Spline::getRoll(float t, bool _loop)
	{
		if (m_rollNodes.size() == 0)
		{
			return 0; //no roll nodes so no roll
		}
		if (m_rollNodes.size() == 1)
		{
			return m_rollNodes[0].y; //Only one roll node
		}
		bool flag = false;

		if (t < m_rollNodes[m_rollNodesOrder[0]].x)
		{
			if(!_loop) return m_rollNodes[m_rollNodesOrder[0]].y; //Everything before first is set to first
			flag = true;
		}

		if (t > m_rollNodes[m_rollNodesOrder[m_rollNodes.size() - 1]].x)
		{
			if(!_loop) return m_rollNodes[m_rollNodesOrder[m_rollNodes.size() - 1]].y; //Everythign after last is set to last
			flag = true;
		}

		if (flag)
		{
			//Need to make the add the back to the front and then make that the new back while making the old back the front, t then needs to be change to fit into this new t range
			//(t does not need to be in range of the spline knots as only in range of the front/back position of the modified roll nodes)
			float backX = m_rollNodes[m_rollNodesOrder.front()].x + getMaxKnot();
			float frontX = m_rollNodes[m_rollNodesOrder.back()].x;
			float backY = m_rollNodes[m_rollNodesOrder.front()].y;
			float frontY = m_rollNodes[m_rollNodesOrder.back()].y;
			bool flip = false;
			if (t < frontX)
			{
				t = t + frontX + (getMaxKnot() - backX);
				flip = true;
			}
			if (flip)
			{
				return (frontY + ((backY - frontY) / (backX - frontX)) * (t - frontX)) + (backY - frontY);
			}
			else
			{
				return frontY + ((backY - frontY) / (backX - frontX)) * (t - frontX);
			}
		}

		//Mapping output roll values with t values
		for (int i = 0; i < m_rollNodes.size() - 1; i++)
		{
			if (m_rollNodes[m_rollNodesOrder[i]].x <= t && m_rollNodes[m_rollNodesOrder[i + 1]].x > t)
			{
				float frontX = m_rollNodes[m_rollNodesOrder[i]].x;
				float backX = m_rollNodes[m_rollNodesOrder[i + 1]].x;
				float frontY = m_rollNodes[m_rollNodesOrder[i]].y;
				float backY = m_rollNodes[m_rollNodesOrder[i + 1]].y;
				return frontY + ((backY - frontY) / (backX - frontX)) * (t - frontX);
			}
		}

		//std::cout << "Something went wrong in getRoll(" << t << ")!" << std::endl;
		return 0;
	}

	void Spline::orderRollNodes()
	{
		m_rollNodesOrder.clear();
		if (m_rollNodes.size() == 0)
		{
			return;
		}
		std::vector<float> temp;
		std::vector<rend::vec2> rtn;
		temp.resize(m_rollNodes.size());

		for (int i = 0; i < m_rollNodes.size(); i++)
		{
			temp[i] = m_rollNodes[i].x;
		}

		std::sort(temp.begin(), temp.end());

		for (int i = 0; i < temp.size(); i++)
		{
			for (int j = 0; j < m_rollNodes.size(); j++)
			{
				if (temp[i] == m_rollNodes[j].x)
				{
					rtn.push_back(m_rollNodes[j]);
				}
			}
		}

		m_rollNodes = rtn;
		orderRollNodesOrder();
	}

	void Spline::orderRollNodesOrder()
	{
		m_rollNodesOrder.clear();
		if (m_rollNodes.size() == 0)
		{
			return;
		}
		std::vector<float> temp;
		temp.resize(m_rollNodes.size());

		for (int i = 0; i < m_rollNodes.size(); i++)
		{
			temp[i] = m_rollNodes[i].x;
		}

		std::sort(temp.begin(), temp.end());

		for (int i = 0; i < temp.size(); i++)
		{
			for (int j = 0; j < m_rollNodes.size(); j++)
			{
				if (temp[i] == m_rollNodes[j].x)
				{
					m_rollNodesOrder.push_back(j);
				}
			}
		}
	}

	float Spline::getBoost(float t)
	{
		float speed = -99999;

		for (int i = 0; i < m_boosters.size(); i++)
		{
			if (t > m_boosters[i].x && t < m_boosters[i].y)
			{
				if (m_boosters[i].z > speed)
				{
					speed = m_boosters[i].z;
				}
			}
		}

		if (speed != -99999)
		{
			return speed;
		}
		return 0;
	}

	double Spline::basis(int i, int k, float t)
	{

		//Ni,0(t)
		if (k == 0)
		{
			if (m_knots[i] <= t && t < m_knots[i + 1])
			{
				return 1.0f;
			}
			else
			{
				return 0.0f;
			}
		}

		//Ni,k(t)
		double denom1 = m_knots[i + k] - m_knots[i];
		double denom2 = m_knots[i + k + 1] - m_knots[i + 1];
		double factor1 = 0.0f;
		double factor2 = 0.0f;

		if (denom1 != 0)
		{
			factor1 = ((t - m_knots[i]) / denom1) * basis(i, k - 1, t);
		}
		if (denom2 != 0)
		{
			factor2 = ((m_knots[i + k + 1] - t) / denom2) * basis(i + 1, k - 1, t);
		}

		double rtn = factor1 + factor2;

		return rtn;
	}

	double Spline::basisDeriv(int i, int k, int d, float t)
	{
		if (k == 0)
		{
			return 0;
		}

		if (d < 1)
		{
			std::cout << "Error: d < 1!" << std::endl;
			return 0;
		}

		double denom1 = m_knots[i + k] - m_knots[i];
		double denom2 = m_knots[i + k + 1] - m_knots[i + 1];
		double factor1 = 0;
		double factor2 = 0;

		if (d == 1)
		{
			//BasisDerivative (1st derivative)
			if (denom1 != 0)
			{
				factor1 = basis(i, k - 1, t) / denom1;
			}
			if (denom2 != 0)
			{
				factor2 = basis(i + 1, k - 1, t) / denom2;
			}
		}
		else
		{
			//BasisDerivative (d'th derivative)
			if (denom1 != 0)
			{
				factor1 = basisDeriv(i, k - 1, d - 1, t) / denom1;
			}
			if (denom2 != 0)
			{
				factor2 = basisDeriv(i + 1, k - 1, d - 1, t) / denom2;
			}
		}

		double rtn = k * (factor1 - factor2);

		return rtn;
	}
}