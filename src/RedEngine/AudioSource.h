#include "Component.h"
#include "AL/al.h"



namespace red
{
	struct AudioClip;
	struct AudioSource : Component
	{
	public:
		AudioSource();
		~AudioSource();

		void play(std::shared_ptr<AudioClip> _clip);
		void onInit();
		void onTick();

	private:
		std::weak_ptr<AudioClip> m_clip;
		ALuint m_source;
	};
}