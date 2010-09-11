#include "Engine.hpp"
#include "db/Database.hpp"
#include "gui/HudManager.hpp"
#include "Map.hpp"
#include "lua/LuaVirtualMachine.hpp"
#include "tools/ImageManager.hpp"
#include "tools/FilesPathHandler.hpp"

const std::string ICON_VARIABLE = "ICON_PATH";

using namespace sf;

Engine::Engine() : App(VideoMode(SCREEN_W, SCREEN_H, 32), "Open Advanced War"),
    m_takeScreen(false)
{
    App.SetFramerateLimit(60);
    App.UseVerticalSync(true);

    // Window icon
    std::string icon = LuaVM::getInstance().extractVariable<std::string>(
        ICON_VARIABLE);
    if (!icon.empty()) // Extraction was OK
    {
        std::string toLoad = gFph(icon);
        if (toLoad.empty())
            toLoad = icon;
        Image *imagePtr = gImageManager.getResource(toLoad);
        if (imagePtr != 0)
            App.SetIcon(imagePtr->GetWidth(), imagePtr->GetHeight(),
                imagePtr->GetPixelsPtr());
    }
}

Engine::~Engine()
{

}

void Engine::run()
{
    Map *map = game.getMapPtr();
    for (unsigned int i = 0; i < 25; i++)
        map->setTile(i, 2, "Forest");
    map->setTile(3, 0, "Road");
    map->setTile(3, 1, "Road");
    map->placeBuilding(10, 10, "Barrack", false);

    const Input &Input = App.GetInput();
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
                if (Event.Key.Code == Key::F5)
                    m_takeScreen = true;
            }
            if (Event.Type == Event::MouseMoved)
            {
                int x = Input.GetMouseX(), y = Input.GetMouseY();
                game.onMouseOver(Vector2i(x, y));
            }
        }

        App.Clear();
            map->renderTo(App);
            HudManager::drawFps(App, App.GetFrameTime());
        App.Display();

        if (m_takeScreen)
        {
            Image screenshot;
            screenshot.CopyScreen(App);
            screenshot.SaveToFile("screen.png");
            m_takeScreen = false;
        }
    }
}
