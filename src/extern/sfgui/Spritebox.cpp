#include <SFGUI/Spritebox.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

Spritebox::Spritebox() :
	m_rect( 0, 0, 0, 0 ),
	m_step( 1, 1 )
{
}

void Spritebox::SetImage( const sf::Image &image, sf::IntRect imagerect ) {
	if( imagerect.Height == 0 || imagerect.Width == 0 ) {
		imagerect.Width = image.GetWidth();
		imagerect.Height = image.GetHeight();
	}

	m_step = sf::Vector2f( static_cast<float>( imagerect.Width ) / 3.0f, static_cast<float>( imagerect.Height ) / 3.0f );

	for( unsigned int y = 0; y < 3; ++y ) {
		for( unsigned int x = 0; x < 3; ++x ) {

			m_sprite[y * 3 + x].SetImage( image );
			m_sprite[y * 3 + x].SetSubRect(
				sf::IntRect(
					imagerect.Left + x * static_cast<int>( m_step.x ),
					imagerect.Top + y * static_cast<int>( m_step.y ),
					static_cast<int>( m_step.x ),
					static_cast<int>( m_step.y )
				)
			);

		}
	}

	// Set current rect to update corner sprite positions.
	SetRect( m_rect );
}

void Spritebox::SetRect( const sf::FloatRect &rect ) {
	m_rect = rect;

	// Setup corners only.
	m_sprite[0].SetPosition( rect.Left, rect.Top );
	m_sprite[2].SetPosition( rect.Left + rect.Width - m_step.x, rect.Top );
	m_sprite[6].SetPosition( rect.Left, rect.Top + rect.Height - m_step.y );
	m_sprite[8].SetPosition( rect.Left + rect.Width - m_step.x, rect.Top + rect.Height - m_step.y );
}

void Spritebox::Render( sf::RenderTarget &target ) const {
	sf::Vector2f  pos;

	// Render center and sides.
	for( pos.y = m_rect.Top; pos.y < m_rect.Top + m_rect.Height - m_step.y; pos.y += m_step.y ) {
		for( pos.x = m_rect.Left; pos.x < m_rect.Left + m_rect.Width - m_step.x; pos.x += m_step.x ) {
			// Center.
			if( pos.y > m_rect.Top && pos.x > m_rect.Left ) {
				m_sprite[4].SetPosition( pos );
				target.Draw( m_sprite[4] );
			}

			// Top and bottom.
			if( pos.y == m_rect.Top && pos.x > m_rect.Left ) {
				m_sprite[1].SetPosition( pos.x, m_rect.Top );
				m_sprite[7].SetPosition( pos.x, m_rect.Top + m_rect.Height - m_step.y );
				target.Draw( m_sprite[1] );
				target.Draw( m_sprite[7] );
			}

		}

		// Left and right.
		if( pos.y > m_rect.Top ) {
			m_sprite[3].SetPosition( m_rect.Left, pos.y );
			m_sprite[5].SetPosition( m_rect.Left + m_rect.Width - m_step.x, pos.y );
			target.Draw( m_sprite[3] );
			target.Draw( m_sprite[5] );
		}
	}

	target.Draw( m_sprite[0] );
	target.Draw( m_sprite[2] );
	target.Draw( m_sprite[6] );
	target.Draw( m_sprite[8] );
}

const sf::FloatRect &Spritebox::GetRect() const {
	return m_rect;
}

const sf::Image &Spritebox::GetImage() const {
	return *m_sprite->GetImage();
}

}
