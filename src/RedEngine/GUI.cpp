#include "GUI.h"
#include "Core.h"
#include "Texture.h"
#include "Input.h"
#include "Window.h"

namespace red
{

	GUI::GUI(std::shared_ptr<Core> _core)
	{
		m_shader.load(rend::GUI_SHADER);
		m_mesh.load(rend::GUI_QUAD_MESH);
		m_core = _core;
		m_input = _core->getInput();
	}

	GUI::~GUI()
	{
	}

	void GUI::Draw(rend::vec2 _pos, Texture& _image)
	{
		rend::ivec2 screenSize = m_core.lock()->m_window->getSize();

		m_shader.attribute("a_Position", *m_mesh.positions());
		m_shader.attribute("a_TexCoord", *m_mesh.texcoords());
		m_shader.uniform("u_Projection", rend::ortho(0.0f, (float)screenSize.x, (float)screenSize.y, 0.0f, -1.0f, 1.0f));

		rend::mat4 model(1.0f);

		model = rend::translate(model, rend::vec3(_pos, 0.0f));
		model = rend::scale(model, rend::vec3(_image.getSize(), 1.0f));

		m_shader.uniform("u_Model", model);
		m_shader.uniform("u_Texture", *_image.m_texture);

		m_shader.render();
	}

	void GUI::Draw(rend::rect _rect, Texture& _image)
	{
		rend::ivec2 screenSize = m_core.lock()->m_window->getSize();

		m_shader.attribute("a_Position", *m_mesh.positions());
		m_shader.attribute("a_TexCoord", *m_mesh.texcoords());
		m_shader.uniform("u_Projection", rend::ortho(0.0f, (float)screenSize.x, (float)screenSize.y, 0.0f, -1.0f, 1.0f));

		rend::mat4 model(1.0f);

		model = rend::translate(model, rend::vec3(_rect.x, _rect.y, 0.0f));
		model = rend::scale(model, rend::vec3(_rect.w, _rect.h, 1.0f));

		m_shader.uniform("u_Model", model);
		m_shader.uniform("u_Texture", *_image.m_texture);

		m_shader.render();
	}

	bool GUI::Button(rend::vec2 _pos, Texture& _image)
	{
		int state = ButtonLogic(rend::rect{ _pos.x, _pos.y, _image.getSize().x, _image.getSize().y });
		Draw(_pos, _image);
		if (state == 2) return true;
		return false;

	}

	bool GUI::Button(rend::rect _pos, Texture& _image)
	{
		int state = ButtonLogic(_pos);
		Draw(_pos, _image);
		if (state == 2) return true;
		return false;
	}

	bool GUI::Button(rend::rect _pos, Texture& _imageUp, Texture& _imageDown)
	{
		int state = ButtonLogic(_pos);
		if (state == 0)
		{
			Draw(_pos, _imageUp);
		}
		else
		{
			Draw(_pos, _imageDown);
			if (state == 2) return true;
		}
		return false;
	}

	int GUI::ButtonLogic(rend::rect _button)
	{
		rend::vec2 mousePos = m_input.lock()->getMousePos();
		rend::vec2 mouseDownPos = m_input.lock()->getMouseDownPos();
		int mouseCode = 1; 
		
		//Mouse originally down over button
		if (MouseCollide(mouseDownPos, _button))
		{
			//Mouse still over button
			if (MouseCollide(mousePos, _button))
			{
				//Mouse relased over button
				if (m_input.lock()->getKeyUpM(mouseCode))
				{
					return 2;
				}
				//Mouse is being held down
				if (m_input.lock()->getKeyM(mouseCode))
				{
					return 1;
				}
			}
		}
		return 0;
	}

	bool GUI::MouseCollide(rend::vec2 _mouse, rend::rect _area)
	{
		if (_mouse.x < _area.x || _mouse.x > _area.x + _area.w) return false; //x
		if (_mouse.y < _area.y || _mouse.y > _area.y + _area.h) return false; //y
		return true;
	}
}