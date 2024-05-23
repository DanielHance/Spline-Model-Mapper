#include <rend/rend.h>
#include <iostream>

namespace red
{
	struct Core;
	struct Input;
	struct Texture;

	struct GUI
	{

	public:
		GUI(std::shared_ptr<Core> _core);
		~GUI();
		void Draw(rend::vec2 _pos, Texture& _image);
		void Draw(rend::rect _rect, Texture& _image);
		bool Button(rend::vec2 _pos, Texture& _image);
		bool Button(rend::rect _rect, Texture& _image);
		bool Button(rend::rect _rect, Texture& _imageUp, Texture& _imageDown);
		
	private:
		rend::Mesh m_mesh;
		rend::Shader m_shader;
		rend::ivec2 screenSize;

		int ButtonLogic(rend::rect _button);
		bool MouseCollide(rend::vec2 _mouse, rend::rect _area);

		std::weak_ptr<Core> m_core;
		std::weak_ptr<Input> m_input;

	};
}