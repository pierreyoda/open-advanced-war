#include <SFGUI/Container.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {
Container::Container( const sf::FloatRect &rect, const std::string &id ) :
	Widget( rect, id )
{
}

Container::Ptr Container::Create( const sf::FloatRect &rect, const std::string &id ) {
	return Ptr( new Container( rect, id ) );
}

bool Container::RenderImpl( sf::RenderTarget &/*target*/ ) const {
	return true;
}

bool Container::OnEvent( const sf::Event &/*event*/ ) {
	return false;
}

Container::ResizeResult Container::OnResize( const sf::FloatRect &/*oldrect*/ ) {
	return Proceed;
}

void Container::OnFocusChange( bool /*state*/ ) {
}

}
