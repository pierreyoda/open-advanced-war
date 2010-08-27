#include "Engine.hpp"
#include "db/Database.hpp"
#include "gui/HudManager.hpp"

using namespace sf;

Engine::Engine() : App(VideoMode(SCREEN_W, SCREEN_H, 32), "Open Advanced War")
{
    App.SetFramerateLimit(60);
    App.UseVerticalSync(true);
}

Engine::~Engine()
{

}

void Engine::run()
{
    Map *map = game.getMapPtr();
    map = new Map();
    for (unsigned int i = 0; i < 25; i++)
        map->setTile(i, 2, "Forest");
    map->setTile(3, 1, "Road");
    //std::cout <<
    while (App.IsOpened())
    {
        Event Event;
        while (App.GetEvent(Event))
        {
            if (Event.Type == Event::Closed)
                App.Close();
            if (Event.Type == Event::KeyPressed)
            {
                if (Event.Key.Code == Key::Escape)
                    App.Close();
            }
        }

        App.Clear();
            map->renderTo(App);
            HudManager::drawFps(App, App.GetFrameTime());
        App.Display();
    }
}
