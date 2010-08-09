#include "Engine.hpp"

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

        App.Display();
    }
}
