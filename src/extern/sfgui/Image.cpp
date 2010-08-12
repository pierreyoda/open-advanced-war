#include <SFGUI/Image.hpp>
#include <SFML/Graphics/Image.hpp>

namespace sfg {

Image::Image( const sf::Image &image ) :
	m_image( image ),
	m_subrect( 0, 0, image.GetWidth(), image.GetHeight() )
{
}

Image::Image( const sf::Image &image, const sf::IntRect &subrect ) :
	m_image( image ),
	m_subrect( subrect )
{
}

void Image::SetSubRect( const sf::IntRect &subrect ) {
	m_subrect = subrect;
}

const sf::IntRect &Image::GetSubRect() const {
	return m_subrect;
}

const sf::Image &Image::GetImage() const {
	return m_image;
}

Image::operator const sf::Image & () const {
	return m_image;
}

}
