#include <rend/rend.h>
#include <RedEngine/RedEngine.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
//#include <chrono>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace red;

struct DistanceMap //Both pos's are stored after m_scale mutiplication
{
	float distance;
	float t;
	float roll;
	rend::vec3 pos;
	rend::vec3 tan;
	rend::vec3 nor;
	rend::vec3 bi;
	rend::vec3 pri;
};


struct MaterialRef
{
	std::string name;
	int faceNum;           //How many face in the part before the usemtl line
};


struct Face //This is diffrent from rend::Face
{
	rend::vec3 vertex;
	rend::vec3 texture;
	rend::vec3 normal;
};


struct Part
{
	std::string name;                 //o name
	std::vector<rend::vec3> vertex;   //v
	std::vector<rend::vec2> texture; //vt
	std::vector<rend::vec3> normal;  //vn
	std::vector<Face> face;          //f
	std::vector<MaterialRef> mat;    //usemat
};


struct Track : Component
{
public:
	Track();

	void onTick();
	void onInit();

	void create();

	//Getter and Setter
	float getScale();
	void setScale(float _scale);
	bool getNormalFlag();
	void setNormalFlag(bool _flag);
	bool getWrap();
	void setWrap(bool _wrap);
	bool getRollWrap();
	void setRollWrap(bool _wrap);
	rend::vec2 getOffset();
	void setOffset(rend::vec2 _offset);
	rend::vec3 getAxis();
	void setAxis(rend::vec3 _axis);
	void setSpline(std::shared_ptr<Spline> _spline);
	void setPath(std::string _path);
	void setFast(bool _flag);
	std::string getPath();
	std::string getPathSimple();
	std::string getDefultPathSimple();
	void setDefultPath(std::string _path);
	void setStep(float _step);
	float getStep();

private:
	std::shared_ptr<Spline> m_spline;               //Current spline
	std::shared_ptr<Model> m_model;                 //Current Model  (might remove later)
	std::vector<Part> m_parts;                      //Object parts
	std::vector<Part> m_partsDefult;                //Defult object paths
	std::vector<std::vector<Part>> m_segParts;
	std::vector<std::string> m_materials;           //Materials (.mtls)
	std::string m_path;                             //Model mapped to spline path
	std::string m_defultPath;                       //Defult model path
	std::string m_pathSimple;
	std::string m_defultPathSimple;
	rend::vec3 m_axis;                              //Axis of defult mode (x or z)
	rend::vec3 m_maxPos;                            //Max values of x,y,z
	rend::vec3 m_minPos;                            //Min values of x,y,z
	rend::vec2 m_offset;
	float m_scale;                                  //Scale of model on spline
	bool m_wrap;
	bool m_rollWrap;
	bool m_loaded;                                  //Has defult been loaded?
	float m_step;
	bool m_normal;
	bool m_fast;
	std::vector<std::function<void()>> m_segs;
	std::vector<std::thread> m_threads;
	std::mutex m_segMutex;

	void loadDefult();
	void createFile();
	void editData();
	void mapSegToSpline(int _segNum, int _segTotalNum, float segLength, float _posOffset);
	void segManager();
	std::vector<DistanceMap> distanceCache(int _segNum, int _segTotalNum, float _segLength, float _offset);
	rend::vec3 rotateVertRoundPoint(rend::vec3 _vert, rend::vec3 _center, rend::vec3 _axis, float _degree);
	std::vector<std::string> splitWhitespace(std::string _string);
	std::vector<std::string> splitFaceToken(std::string _string);

};