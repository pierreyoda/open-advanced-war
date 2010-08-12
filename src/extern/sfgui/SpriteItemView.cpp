#include <SFGUI/SpriteItemView.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

SpriteItemView::SpriteItemView( const sf::Sprite &sprite ) :
	m_sprite( sprite )
{
	m_sprite.SetPosition( 0, 0 );
}

const sf::FloatRect SpriteItemView::GetRect() const {
	return sf::FloatRect(
		m_sprite.GetPosition().x,
		m_sprite.GetPosition().y,
		static_cast<float>( m_sprite.GetSubRect().Width ) * m_sprite.GetScale().x,
		static_cast<float>( m_sprite.GetSubRect().Height ) * m_sprite.GetScale().y
	);
}

void SpriteItemView::SetRect( const sf::FloatRect& rect ) {
	m_sprite.SetPosition( rect.Left, rect.Top );
}

void SpriteItemView::Render( sf::RenderTarget &target ) {
	target.Draw( m_sprite );
}

}
