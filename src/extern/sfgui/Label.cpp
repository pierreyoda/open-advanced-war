#include <SFGUI/Label.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

Label::Label(
	const sf::Vector2f &position,
	const sf::String &text,
	const std::string &id
) :
	Widget( sf::FloatRect( position.x, position.y, 0, 0 ), id ),
	m_text( text )
{
	SetTextStyle( TextStyle() );
	UpdateRectFromString();
}

Label::Ptr Label::Create( const sf::Vector2f &position, const sf::String &text, const std::string &id ) {
	return Ptr( new Label( position, text, id ) );
}

bool Label::RenderImpl( sf::RenderTarget &target ) const {
	target.Draw( m_text );
	return false;
}

bool Label::OnEvent( const sf::Event &/*event*/ ) {
	return false;
}

Label::ResizeResult Label::OnResize( const sf::FloatRect &/*oldrect*/ ) {
	m_text.SetPosition( GetRect().Left + m_padding.x, GetRect().Top + m_padding.y );
	return Cancel;
}

const sf::String &Label::GetString() const {
	return m_text.GetString();
}

void Label::UpdateRectFromString() {
	SetRect( sf::FloatRect( GetRect().Left, GetRect().Top, m_text.GetRect().Width, static_cast<float>( m_text.GetCharacterSize() ) ) );
}

void Label::SetString( const sf::String &string ) {
	m_text.SetString( string );
	UpdateRectFromString();
	NotifyRedraw();
}

void Label::OnFocusChange( bool /*state*/ ) {
}

void Label::SetPadding( const sf::Vector2f &padding ) {
	m_padding = padding;
	SetRect( GetRect() );
	NotifyRedraw();
}

const sf::Vector2f Label::GetPadding() const {
	return m_padding;
}

sf::Vector2f Label::GetCharacterPos( std::size_t index ) const {
	return m_text.GetCharacterPos( index );
}

void Label::SetTextStyle( const TextStyle& style ) {
	m_textstyle = style;
	m_text.SetFont( style.GetFont() );
	m_text.SetCharacterSize( style.GetCharacterSize() );
	m_text.SetColor( style.GetColor() );
	UpdateRectFromString();
}

const TextStyle& Label::GetTextStyle() const {
	return m_textstyle;
}

}
