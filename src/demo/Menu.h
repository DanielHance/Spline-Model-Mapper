#include <iostream>
#include <rend/rend.h>
#include <RedEngine/RedEngine.h>

using namespace red;

struct CarPhysics;
struct Track;

struct Menu : Component
{
public:
	Menu();
	void onTick();
	void onGUI();

	void setSpline(std::shared_ptr<Spline> _spline);
	void setSplineRenderer(std::shared_ptr<SplineRenderer> _renderer);
	void setCarRenderer(std::shared_ptr<ModelRenderer> _renderer);
	void setTrackRenderer(std::shared_ptr<ModelRenderer> _renderer);
	void setCarPhysics(std::shared_ptr<CarPhysics> _physics);
	void setTrack(std::shared_ptr<Track> _track);

private:
	std::shared_ptr<Spline> m_spline;
	std::shared_ptr<SplineRenderer> m_rendererS;  //Spline renderer
	std::shared_ptr<ModelRenderer> m_rendererC;   //Car Renderer
	std::shared_ptr<ModelRenderer> m_rendererT;   //Track Renderer
	std::shared_ptr<CarPhysics> m_physics;
	std::shared_ptr<Track> m_track;

	bool ui;
	bool loadWindow;
	bool start;

	void splineName();
	void changeSpline();
	void updateTrackModel(bool _fast);

	std::string m_splineName;

	std::string m_trackPath;
	std::string m_trackDefultPath;
	rend::vec3 m_trackAxis;
	rend::vec2 m_trackOffset;
	float m_trackScale;
	float m_trackStep;

	int m_nodeIndex;
	int m_rollIndex;
	bool m_rollWrap;
	int m_boosterIndex;
	int m_degree;

	bool m_nodeHighlight;
	bool m_rollHighlight;
	bool m_boostHighlight;

	
};