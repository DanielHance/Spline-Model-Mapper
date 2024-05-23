#include "Component.h"

#include <rend/rend.h>

namespace red
{
	struct Spline;
	struct Model;

	struct SplineRenderer : Component
	{
	public:
		SplineRenderer();
		~SplineRenderer();

		void onInit();
		void onTick();

		//Getter and Setter
		void setSpline(std::shared_ptr<Spline> _spline); //
		void setSplineColor(rend::vec3 _col); //
		void setNodeConnectColor(rend::vec3 _col); //
		void setNodeColor(rend::vec3 _col); //
		void setNodeHighlightColor(rend::vec3 _col); //
		void setRollColor(rend::vec3 _col);//
		void setRollHighlightColor(rend::vec3 _col);//
		void setBoostColor(rend::vec3 _col);//
		void setBoostHighlightColor(rend::vec3 _col);//
		void setShow(bool _flag); //
		void setSplineShow(bool _flag); //
		void setNodeShow(bool _flag); //
		void setNodeConnect(bool _flag); //
		void setRollShow(bool _flag);//
		void setBoosterShow(bool _flag);//
		void setNodeHighlight(int _index = -1);//
		void setRollHighlight(int _index = -1);//
		void setBoostHighlight(int _index = -1);//
		rend::vec3 getSplineColor(); //
		rend::vec3 getNodeConnectColor(); //
		rend::vec3 getNodeColor(); //
		rend::vec3 getNodeHighlightColor(); //
		rend::vec3 getRollColor(); //
		rend::vec3 getRollHighlightColor(); //
		rend::vec3 getBoostColor(); //
		rend::vec3 getBoostHighlightColor(); //
		bool getShow();//
		bool getSplineShow();//
		bool getNodeShow();//
		bool getNodeConnect();//
		bool getRollShow();//
		bool getBoostShow();//

		void setDetail(float _detail);
		std::shared_ptr<Spline> getSpline();

	private:
		std::shared_ptr<Spline> m_spline;
		std::shared_ptr<Model> m_nodeModel;
		std::shared_ptr<Model> m_rollModel;
		rend::vec4 m_splineCol;
		rend::vec4 m_nodeConnectCol;
		rend::vec4 m_nodeCol;
		rend::vec4 m_nodeHighlightCol;
		rend::vec4 m_rollCol;
		rend::vec4 m_rollHighlightCol;
		rend::vec4 m_boostCol;
		rend::vec4 m_boostHighlightCol;
		rend::Shader m_shader_line;
		rend::ModelShader m_shader_model;
		rend::Mesh m_mesh; //Needs for line shader
		float m_detail;
		float t;

		bool s_show;
		bool s_spline;
		bool s_node;
		bool s_connect;
		bool s_roll;
		bool s_boost;
		int h_node;
		int h_roll;
		int h_boost;

		void onDisplay();
	};
}