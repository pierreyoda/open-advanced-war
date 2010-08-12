#include <SFGUI/DefaultImage.hpp>

namespace sfg {

const sf::Image CreateDefaultImage( unsigned int width, unsigned int height, const sf::Color& color ) {
	sf::Image  img;
	img.Create( width, height, color );
	return img;
}

}
