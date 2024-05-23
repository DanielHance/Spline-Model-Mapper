#include "Resource.h"
#include <vector>

namespace red
{
	struct Spline : Resource
	{
	public:
		Spline();
		~Spline();

		//File data functions
		void onLoad(); 
		void loadFile(); //call getData
		void saveFile(); //call setData

		//Node functions
		void addNode(rend::vec3 _pos);  
		void addNode(rend::vec3 _pos, int _index); 
		void removeNode(); 
		void removeNode(int _index); 
		void setNodePos(rend::vec3 _pos, int _index); 
		void moveNode(rend::vec3 _move, int _index); 
		int getTotalNodes(); 
		rend::vec3 getNodePos(int _index); 

		//Roll node functions
		void orderRollNodes(); 
		void addRollNode(float t, float degree); 
		void removeRollNode(); 
		void removeRollNodePoint(float t); 
		void removeRollNode(int _index); 
		void setRollNodePos(float _pos, int _index); 
		void setRollNodeAngle(float _angle, int _index); 
		int getTotalRollNodes(); 
		float getRollNodePos(int _index);  
		float getRollNodeAngle(int _index); 
		float getRoll(float t);
		float getRoll(float t, bool _loop);
		
		//Booster functions
		void addBooster(float _start, float _end, float _speed); 
		void removeBooster(int _index); 
		void setBoosterPos(rend::vec2 _pos, int _index);
		void setBoosterSpeed(float _speed, int _index);
		int getTotalBoosters();
		rend::vec2 getBoosterPos(int _index);
		float getBoosterSpeed(int _index);
		float getBoost(float t); 



		//Spline data functions
		void setClamp(bool _clamp); 
		bool getClamp();
		void makeLoop(); 
		int getTotalKnots(); 
		float getMinKnot(); 
		float getMaxKnot(); 
		float getLength(); 
		float getLength(float _step); 
		float getLength(float _start, float _end);
		float getLength(float _start, float _end, float _step); 
		float step(float t, float _step); 
		float step(float t, float _step, float _error); 
		float stepFast(float t, float _step); 
		int getDegree();
		void setDegree(int _degree);
		bool valid(); //Returns if there is valid spline (there are enough node to make a curve) 
		rend::vec3 getPoint(float t);  
		rend::vec3 getVelocity(float t);  //Velocity Vector 
		rend::vec3 getNormal(float t); 
		rend::vec3 getBinormal(float t); 
		rend::vec3 getPrincipal(float t);
		rend::vec3 getAcceleration(float t); //Acceleration vector 
		rend::vec3 getTorsion(float t); //Torsion Vector 

	private:
		bool getData(std::string _path); //Load data from file
		bool setData(std::string _path); //save data to file
		void genKnotVec();
		void orderRollNodesOrder();
		float step(float t, float _step, float _error, bool _fast);
		double basis(int i, int k, float t);    //Basis functions need to be double as the number outputed are very small
		double basisDeriv(int i, int k, int d, float t);

		std::vector<rend::vec3> m_nodes;
		std::vector<double> m_knots;
		std::vector<rend::vec2> m_rollNodes;
		std::vector<int> m_rollNodesOrder; //Stores the oder of roll nodes so m_rollNode can stay unorder
		std::vector<rend::vec3> m_boosters;  //start, end, speed
		int m_degree;
		bool m_clamp;
		std::string m_filename;
		std::string m_filepath;
		
	};
}