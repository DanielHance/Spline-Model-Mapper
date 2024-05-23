#include "Resource.h"
#include <AL/al.h>
#include <vector>

namespace red
{
	struct AudioSource;
	struct AudioClip : Resource
	{
	public:
		void onLoad();

	private:
		friend struct AudioSource;
		ALuint m_buffer;
		void load_ogg(const std::string& _path, std::vector<unsigned char>& _buffer, ALenum& _format, ALsizei& _freq);
	};

}