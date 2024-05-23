#include "AudioSource.h"
#include "AudioClip.h"
#include "Entity.h"
#include "Transform.h"
#include <vector>

namespace red
{

	AudioSource::AudioSource()
	{
	}

	AudioSource::~AudioSource()
	{
		alDeleteSources(1, &m_source);
	}

	void AudioSource::onInit()
	{
		m_source = 8;
		alGenSources(1, &m_source);


		alSource3f(m_source, AL_POSITION, 0.0f, 0.0f, 0.0f);
		//alSourcef(sourceId, AL_PITCH, 10);
		//alSourcef(sourceId, AL_GAIN, vol);
	}

	void AudioSource::play(std::shared_ptr<AudioClip> _clip)
	{
		m_clip = _clip;
		alSourcei(m_source, AL_BUFFER, m_clip.lock()->m_buffer);
		alSourcePlay(m_source);
	}

	void AudioSource::onTick()
	{
		//Update the source pos
		rend::vec3 pos = getEntity()->getTransform()->getPos();
		alSource3f(m_source, AL_POSITION, pos.x, pos.y, pos.z);
	}
}