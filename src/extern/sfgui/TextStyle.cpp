#include <SFGUI/TextStyle.hpp>

namespace sfg {

TextStyle::TextStyle( const sf::Font& font, unsigned int charsize, const sf::Color& color ) :
	m_font( &font ),
	m_charsize( charsize ),
	m_color( color )
{
}

void TextStyle::SetFont( const sf::Font& font ) {
	m_font = &font;
}

const sf::Font& TextStyle::GetFont() const {
	return *m_font;
}

void TextStyle::SetCharacterSize( unsigned int size ) {
	m_charsize = size;
}

unsigned int TextStyle::GetCharacterSize() const {
	return m_charsize;
}

void TextStyle::SetColor( const sf::Color& color ) {
	m_color = color;
}

const sf::Color& TextStyle::GetColor() const {
	return m_color;
}


}
