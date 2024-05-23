#include "Menu.h"
#include "demo/CarPhysics.h"
#include "demo/Track.h"

Menu::Menu()
{
	ui = false;
	loadWindow = false;
	start = true;
	m_nodeIndex = 0;
	m_rollIndex = 0;
	m_boosterIndex = 0;
	m_degree = 3;
	m_rollWrap = true;
	m_nodeHighlight = true;
	m_rollHighlight = true;
	m_boostHighlight = false;
}

void Menu::setSpline(std::shared_ptr<Spline> _spline)
{
	m_spline = _spline;
	splineName();
}

void Menu::setSplineRenderer(std::shared_ptr<SplineRenderer> _renderer)
{
	m_rendererS = _renderer;
}

void Menu::setCarRenderer(std::shared_ptr<ModelRenderer> _renderer)
{
	m_rendererC = _renderer;
}

void Menu::setTrackRenderer(std::shared_ptr<ModelRenderer> _renderer)
{
	m_rendererT = _renderer;
}

void Menu::setCarPhysics(std::shared_ptr<CarPhysics> _physics)
{
	m_physics = _physics;
}

void Menu::setTrack(std::shared_ptr<Track> _track)
{
	m_track = _track;
}

void Menu::onTick()
{
	if (getCore().lock()->getInput()->getKeyDown(224)) //alt
	{
		if (ui)
		{
			getCore().lock()->getInput()->setMouseLock(true);
			ui = false;
		}
		else
		{
			getCore().lock()->getInput()->setMouseLock(false);
			ui = true;
			
		}
	}
}

void Menu::splineName()
{
	std::string name = m_spline->getPath();
	//Remove the file path
	int cutPos = name.rfind("/"); 
	if (cutPos != std::string::npos)
	{
		name = name.substr(cutPos + 1);
		//Remove the file type
		cutPos = name.rfind(".");
		if (cutPos != std::string::npos)
		{
			name = name.substr(0, cutPos);
			m_splineName = name;
		}
	}
}

void Menu::updateTrackModel(bool _fast)
{
	m_track->setDefultPath(m_trackDefultPath);
	m_track->setPath(m_trackPath);
	m_track->setAxis(m_trackAxis);
	m_track->setScale(m_trackScale);
	m_track->setOffset(m_trackOffset);
	m_track->setStep(m_trackStep);
	m_track->setFast(_fast);
	m_track->create();
	getCore().lock()->getResources()->reload<Model>(m_track->getPath());
	m_rendererT->setModel(getCore().lock()->getResources()->load<Model>(m_track->getPathSimple()));
	m_rendererT->setShow(true);
	m_trackDefultPath = m_track->getDefultPathSimple();
	m_trackPath = m_track->getPathSimple();
	m_trackAxis = m_track->getAxis();
	m_trackScale = m_track->getScale();
	m_trackOffset = m_track->getOffset();
	m_trackStep = m_track->getStep();
}

void Menu::changeSpline()
{
	std::string path = "splines/" + m_splineName + ".txt";
	setSpline(getCore().lock()->getResources()->load<Spline>(path));
	m_rendererS->setSpline(getCore().lock()->getResources()->load<Spline>(path));
	m_physics->setSpline(getCore().lock()->getResources()->load<Spline>(path));
	m_track->setSpline(getCore().lock()->getResources()->load<Spline>(path));
}

void Menu::onGUI()
{
	if (ui)
	{
		//Set up GUI stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::StyleColorsDark();
		if (start)
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(465, getCore().lock()->getWindow()->getSize().y));
			ImGui::SetNextWindowCollapsed(false);
			m_trackPath = m_track->getPathSimple();
			m_trackDefultPath = m_track->getDefultPathSimple();
			m_trackAxis = m_track->getAxis();
			m_trackScale = m_track->getScale();
			m_trackOffset = m_track->getOffset();
			m_trackStep = m_track->getStep();
			start = false;
		}

		//Start of main GUI window--------------------------------------------------------------------------------------------------------------------------------------------------
		ImGui::Begin("Controls");
		
		//File Input
		ImGui::Text("Spline File");
		char fileC[255];
		strcpy(fileC, m_splineName.c_str());
		ImGui::InputText("File", fileC, sizeof(fileC) - 1);   //file id0
		m_splineName = fileC;
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "DO NOT use special file characters (.,/\\!?)");
		if (ImGui::Button("Load/Create", ImVec2(ImGui::CalcItemWidth(), 0)))
		{
			loadWindow = true;
		}
		if (ImGui::Button("Save", ImVec2(ImGui::CalcItemWidth(), 0)))
		{
			m_spline->saveFile();
		}
		if (ImGui::Button("Undo", ImVec2(ImGui::CalcItemWidth(), 0)))
		{
			m_spline->loadFile();
		}

		//Track Mesh
		ImGui::Text("");
		ImGui::Text("Track Mesh");
		if (ImGui::Button("Update Model (Fast)", ImVec2(ImGui::CalcItemWidth(), 0)))
		{
			updateTrackModel(true);
		}
		if (ImGui::Button("Update Model (Detailed)", ImVec2(ImGui::CalcItemWidth(), 0)))
		{
			updateTrackModel(false);
		}
		if (m_track->getWrap())
		{
			if (ImGui::Button("UnWarp", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				m_track->setWrap(false);
			}
		}
		else
		{
			if (ImGui::Button("Warp", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				m_track->setWrap(true);
			}
		}
		if (m_track->getNormalFlag())
		{
			if (ImGui::Button("Remove Vertex Normals", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				m_track->setNormalFlag(false);
			}
		}
		else
		{
			if (ImGui::Button("Add Vertex Normals", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				m_track->setNormalFlag(true);
			}
		}
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Recommend not change data below for track model.");
		char trackC[255];
		strcpy(trackC, m_trackDefultPath.c_str());
		ImGui::InputText("Template Path ", trackC, sizeof(trackC) - 1);
		m_trackDefultPath = trackC;
		trackC[254];
		strcpy(trackC, m_trackPath.c_str());
		ImGui::InputText("Gen Path ", trackC, sizeof(trackC) - 1);
		m_trackPath = trackC;
		int axis[3] = { m_trackAxis.x, m_trackAxis.y, m_trackAxis.z };
		ImGui::InputInt3("Axis", axis);
		m_trackAxis = rend::vec3(axis[0], axis[1], axis[2]);
		ImGui::InputFloat("Scale", &m_trackScale);
		ImGui::InputFloat("Detail", &m_trackStep);
		float offset[2] = { m_trackOffset.x, m_trackOffset.y };
		ImGui::InputFloat2("Offset", offset);
		m_trackOffset = rend::vec2(offset[0], offset[1]);

		//Pos Node Edit
		ImGui::Text("");
		ImGui::Text("Spline Nodes");
		if (m_spline->getTotalNodes() != 0)
		{
			ImGui::SliderInt("Index", &m_nodeIndex, 0, m_spline->getTotalNodes() - 1); //index id0
			rend::vec3 nPosV = m_spline->getNodePos(m_nodeIndex);
			float nPosF[3] = { nPosV.x, nPosV.y, nPosV.z };
			ImGui::InputFloat3("x,y,z", nPosF);
			m_spline->setNodePos(rend::vec3(nPosF[0], nPosF[1], nPosF[2]), m_nodeIndex);
		}
		if (ImGui::Button("Add Node", ImVec2(ImGui::CalcItemWidth(), 0)))
		{
			if (m_spline->getTotalNodes() == 0)
			{
				m_spline->addNode(rend::vec3(0));
				m_nodeIndex = 0;
			}
			else
			{
				m_spline->addNode(m_spline->getNodePos(m_nodeIndex), m_nodeIndex);
			}
		}
		if (m_spline->getTotalNodes() != 0)
		{
			if (ImGui::Button("Remove Node", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				m_spline->removeNode(m_nodeIndex);
				if (m_nodeIndex == m_spline->getTotalNodes())
				{
					m_nodeIndex -= 1;
				}
				//Need to check if and roll nodes or boosters were deleted and update the markers
				if (m_rollIndex >= m_spline->getTotalRollNodes())
				{
					m_rollIndex = m_spline->getTotalRollNodes() - 1;
				}
				if (m_boosterIndex >= m_spline->getTotalBoosters())
				{
					m_boosterIndex = m_spline->getTotalBoosters() - 1;
				}
			}
		}

		//Roll Node
		ImGui::Text("");
		ImGui::Text("Roll Nodes");
		if (m_spline->valid())
		{
			if (m_spline->getTotalRollNodes() != 0)
			{
				ImGui::SliderInt("Index ", &m_rollIndex, 0, m_spline->getTotalRollNodes() - 1); //index id1
				float rollPos = m_spline->getRollNodePos(m_rollIndex);
				float rollAngle = m_spline->getRollNodeAngle(m_rollIndex);
				ImGui::SliderFloat("Pos", &rollPos, m_spline->getMinKnot(), m_spline->getMaxKnot()); //pos id0
				ImGui::SliderFloat("Angle", &rollAngle, -180, 180);
				m_spline->setRollNodePos(rollPos, m_rollIndex);
				m_spline->setRollNodeAngle(rollAngle, m_rollIndex);
			}
			if (ImGui::Button("Add Roll Node", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				if (m_spline->getTotalRollNodes() - 1 < 0)
				{
					m_spline->addRollNode(0, 0);
					m_rollIndex = 0;
				}
				else
				{
					m_spline->addRollNode(m_spline->getRollNodePos(m_spline->getTotalRollNodes() - 1), m_spline->getRollNodeAngle(m_spline->getTotalRollNodes() - 1));
				}
			}
			if (m_spline->getTotalRollNodes() != 0)
			{
				if (ImGui::Button("Remove Roll Node", ImVec2(ImGui::CalcItemWidth(), 0)))
				{
					m_spline->removeRollNode(m_rollIndex);
					if (m_rollIndex == m_spline->getTotalRollNodes())
					{
						m_rollIndex -= 1;
					}
				}

				if (ImGui::Button("Sort Roll Node", ImVec2(ImGui::CalcItemWidth(), 0)))
				{
					m_spline->orderRollNodes();
				}
				if (m_rollWrap)
				{
					if (ImGui::Button("Unwrap", ImVec2(ImGui::CalcItemWidth(), 0)))
					{
						m_rollWrap = false;
						m_track->setRollWrap(false);
						m_physics->setRollWrap(false);
					}
				}
				else
				{
					if (ImGui::Button("Wrap", ImVec2(ImGui::CalcItemWidth(), 0)))
					{
						m_rollWrap = true;
						m_track->setRollWrap(true);
						m_physics->setRollWrap(true);
					}
				}
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(255, 165, 0, 255), "Not engough nodes.");
		}

		//Booster 
		ImGui::Text("");
		ImGui::Text("Boosters");
		if (m_spline->valid())
		{
			if (m_spline->getTotalBoosters() != 0)
			{
				ImGui::SliderInt("Index  ", &m_boosterIndex, 0, m_spline->getTotalBoosters() - 1); //index id2
				rend::vec2 bPosV = m_spline->getBoosterPos(m_boosterIndex);
				float bPosF[2] = { bPosV.x, bPosV.y };
				float speed = m_spline->getBoosterSpeed(m_boosterIndex);
				ImGui::SliderFloat2("Start,End", bPosF, m_spline->getMinKnot(), m_spline->getMaxKnot());
				ImGui::InputFloat("Speed", &speed);
				m_spline->setBoosterPos(rend::vec2(bPosF[0], bPosF[1]), m_boosterIndex);
				m_spline->setBoosterSpeed(speed, m_boosterIndex);
			}
			if (ImGui::Button("Add Booster", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				m_spline->addBooster(0, 0, 0);
				if (m_boosterIndex < 0)
				{
					m_boosterIndex = 0;
				}
			}
			if (m_spline->getTotalBoosters() != 0)
			{
				if (ImGui::Button("Remove Booster", ImVec2(ImGui::CalcItemWidth(), 0)))
				{
					m_spline->removeBooster(m_boosterIndex);
					if (m_boosterIndex == m_spline->getTotalBoosters())
					{
						m_boosterIndex -= 1;
					}
				}
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(255, 165, 0, 255), "Not engough nodes.");
		}

		//Spline Control
		ImGui::Text("");
		ImGui::Text("Spline Control");
		bool change = ImGui::InputInt("Degree", &m_degree);
		if (change)
		{
			if (m_degree < 1)
			{
				m_degree = 1;
			}
			else
			{
				m_spline->setDegree(m_degree);
			}
		}
		if (m_spline->valid())
		{
			if (ImGui::Button("Make Loop", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				m_spline->makeLoop();
				m_rollWrap = true;
				m_track->setWrap(true);
				m_track->setRollWrap(true);
				m_physics->setRollWrap(true);
			}
			if (m_spline->getClamp())
			{
				if (ImGui::Button("Unclamp", ImVec2(ImGui::CalcItemWidth(), 0)))
				{
					m_spline->setClamp(false);
				}
			}
			else
			{
				if (ImGui::Button("Clamp", ImVec2(ImGui::CalcItemWidth(), 0)))
				{
					m_spline->setClamp(true);
				}
			}
			ImGui::TextColored(ImVec4(255, 0, 0, 255), "*Clamping/Unclamping will effect the positions of\nbanking and boosters on spline!");
		}

		//Car Physics Controls
		ImGui::Text("");
		ImGui::Text("Animated Model Controls");
		if (m_spline->valid())
			{
			if (m_physics->getPhysics())
			{
				if (ImGui::Button("Step Mode", ImVec2(ImGui::CalcItemWidth(), 0)))
				{
					m_physics->setPhysics(false);
				}
				float temp = m_physics->getMass();
				ImGui::InputFloat("Mass", &temp);
				m_physics->setMass(temp);
				temp = m_physics->getDrag();
				ImGui::InputFloat("Drag", &temp);
				m_physics->setDrag(temp);
			}
			else
			{
				if (ImGui::Button("Physics Mode", ImVec2(ImGui::CalcItemWidth(), 0)))
				{
					m_physics->setPhysics(true);
					m_physics->setVelocity(0);
				}
				float cPos = m_physics->getPos();
				ImGui::SliderFloat("Pos ", &cPos, m_spline->getMinKnot(), m_spline->getMaxKnot());  //pos id1
				m_physics->setPos(cPos);
				float step = m_physics->getStepSize();
				ImGui::InputFloat("Step size", &step);
				m_physics->setStepSize(step);
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(255, 165, 0, 255), "Not engough nodes.");
		}

		//Spline Renderer Controls
		ImGui::Text("");
		ImGui::Text("View Controls");
		bool flag = m_rendererT->getShow();
		ImGui::Checkbox("Show track", &flag);
		m_rendererT->setShow(flag);
		flag = m_rendererC->getShow();
		ImGui::Checkbox("Show animated model", &flag);
		m_rendererC->setShow(flag);
		flag = m_rendererS->getSplineShow();
		ImGui::Checkbox("Show spline", &flag);
		m_rendererS->setSplineShow(flag);
		flag = m_rendererS->getNodeShow();
		ImGui::Checkbox("Show nodes", &flag);
		m_rendererS->setNodeShow(flag);
		flag = m_rendererS->getNodeConnect();
		ImGui::Checkbox("Connect nodes", &flag);
		m_rendererS->setNodeConnect(flag);
		flag = m_rendererS->getRollShow();
		ImGui::Checkbox("Show banking nodes", &flag);
		m_rendererS->setRollShow(flag);
		flag = m_rendererS->getBoostShow();
		ImGui::Checkbox("Show boosters", &flag);
		m_rendererS->setBoosterShow(flag);
		ImGui::Checkbox("Highlight nodes", &m_nodeHighlight);
		if (m_nodeHighlight)
		{
			m_rendererS->setNodeHighlight(m_nodeIndex);
		}
		else
		{
			m_rendererS->setNodeHighlight(-1);
		}
		ImGui::Checkbox("Highlight banking nodes", &m_rollHighlight);
		if (m_rollHighlight)
		{
			m_rendererS->setRollHighlight(m_rollIndex);
		}
		else
		{
			m_rendererS->setRollHighlight(-1);
		}
		ImGui::Checkbox("Highlight boosters", &m_boostHighlight); 
		if (m_boostHighlight)
		{
			m_rendererS->setBoostHighlight(m_boosterIndex);
		}
		else
		{
			m_rendererS->setBoostHighlight(-1);
		}
		rend::vec3 col = m_rendererS->getSplineColor();
		ImGui::ColorEdit3("Spline color", &(col[0]));
		m_rendererS->setSplineColor(col);
		col = m_rendererS->getNodeConnectColor();
		ImGui::ColorEdit3("Node connect color", &(col[0]));
		m_rendererS->setNodeConnectColor(col);
		col = m_rendererS->getNodeColor();
		ImGui::ColorEdit3("Node color", &(col[0]));
		m_rendererS->setNodeColor(col);
		col = m_rendererS->getNodeHighlightColor();
		ImGui::ColorEdit3("Node highlight", &(col[0]));
		m_rendererS->setNodeHighlightColor(col);
		col = m_rendererS->getRollColor();
		ImGui::ColorEdit3("Bank node color", &(col[0]));
		m_rendererS->setRollColor(col);
		col = m_rendererS->getRollHighlightColor();
		ImGui::ColorEdit3("Bank node highlight", &(col[0]));
		m_rendererS->setRollHighlightColor(col);
		col = m_rendererS->getBoostColor();
		ImGui::ColorEdit3("Booster color", &(col[0]));
		m_rendererS->setBoostColor(col);
		col = m_rendererS->getBoostHighlightColor();
		ImGui::ColorEdit3("Booster highlight", &(col[0]));
		m_rendererS->setBoostHighlightColor(col);

		ImVec2 windowPos(ImGui::GetWindowPos());
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImGui::End();
		//End of main GUI window---------------------------------------------------------------------------------------------------------------------------------------------------------
	
		ImGui::SetNextWindowPos(windowPos);
		ImGui::SetNextWindowSize(windowSize);
		ImGui::SetNextWindowCollapsed(false);

		//Start of load/create GUI window------------------------------------------------------------------------------------------------------------------------------------------------
		if (loadWindow)
		{
			ImGui::Begin("Load/Create Spline");
			ImGui::SetWindowFocus();
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "IMPORTANT!");
			std::string textS = "You are about to load/create a spline with the name '" + m_splineName + "', if this spline file does not already exist a new spline will be created with this name, please check your spelling is correct!";
			const char* textC = textS.c_str();
			ImGui::TextWrapped(textC);
			ImGui::Text("");
			textS = "Wold you like to save the current spline's changes?";
			const char* textC2 = textS.c_str();
			ImGui::TextWrapped(textC2);
			if (ImGui::Button("Load/Create and Save", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				loadWindow = false;
				m_spline->saveFile();
				changeSpline();
			}
			if (ImGui::Button("Load/Create and Don't Save", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				loadWindow = false;
				changeSpline();
			}
			if (ImGui::Button("Go Back", ImVec2(ImGui::CalcItemWidth(), 0)))
			{
				loadWindow = false;
			}

			ImGui::End();
		}
		//End of load/create GUI window----------------------------------------------------------------------
		
		//Renderer the GUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
