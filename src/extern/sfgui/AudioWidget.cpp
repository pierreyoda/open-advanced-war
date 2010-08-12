#include <SFGUI/AudioWidget.hpp>

namespace sfg {

void AudioWidget::SetSound( SoundType type, const sf::SoundBuffer &buffer ) {
	m_sounds[type] = sf::Sound( buffer );
}

void AudioWidget::DisableSound( SoundType type ) {
	m_sounds.erase( type );
}

void AudioWidget::PlaySound( SoundType type ) {
	SoundMap::iterator  sounditer( m_sounds.find( type ) );

	if( sounditer != m_sounds.end() ) {
		sounditer->second.Play();
	}
}

}
