#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "constantes.hpp"
#include "Game.hpp"

class Engine
{
    public:
        Engine();
        ~Engine();

        void run();

    private:
        sf::RenderWindow App;
        Game game;
};

#endif /* ENGINE_HPP */
