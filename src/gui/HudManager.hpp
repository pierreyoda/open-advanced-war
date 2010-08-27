#ifndef HUDMANAGER_HPP
#define HUDMANAGER_HPP

#include "../constantes.hpp"
namespace sf
{
    class RenderTarget;
    typedef Vector2<int> Vector2i;
}

class HudManager
{
    public:
        static void drawFps(sf::RenderTarget &target, const float &fpsCount,
                const sf::Vector2i &screenSize = sf::Vector2i(SCREEN_W, SCREEN_H));
};

#endif /* HUDMANAGER_HPP */
