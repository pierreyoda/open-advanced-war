#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "constantes.hpp"

class Engine
{
    public:
        Engine();
        ~Engine();

        void run();

    private:
        sf::RenderWindow App;
};

#endif /* ENGINE_HPP */
