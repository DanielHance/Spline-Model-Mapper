#include "Resource.h"

namespace red
{
	struct GUI;

struct Texture : Resource
{
public:
	void onLoad() { m_texture = std::make_shared<rend::Texture>(getPath().c_str()); }
	rend::vec2 getSize() { return rend::vec2(m_texture->width(), m_texture->height()); }

private:
	friend struct GUI;
	std::shared_ptr<rend::Texture> m_texture;
};

}