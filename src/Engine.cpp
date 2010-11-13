#include "Engine.hpp"
#include "db/Database.hpp"
#include "Map.hpp"
#include "lua/LuaVirtualMachine.hpp"
#include "tools/ImageManager.hpp"
#include "tools/FilesPathHandler.hpp"

const std::string ICON_VARIABLE = "ICON_PATH";

using namespace sf;

Engine::Engine() : App(VideoMode(SCREEN_W, SCREEN_H+GUI_PART_H, 32), "Open Advanced War"),
    m_takeScreen(false)
{
    App.SetFramerateLimit(60);
    App.UseVerticalSync(true);
    App.ShowMouseCursor(false);
    gGame.setApp(&App);
    gGame.initTestMap();

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

    // Exposing game
    luabind::globals(LuaVM::getInstance().getLua())["game"] = &gGame;
    // Main menu
    bool error = false;
    CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void, "mainMenu", error, 0)
    if (error)
    {
        std::cerr << "Main menu error. Game will now exit.\n";
        exit(1);
    }
}

Engine::~Engine()
{

}

void Engine::run()
{
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
            gGame.listenEvent(Event);
        }
        gGame.listenInput(Input);

        App.Clear();
            gGame.renderGame(App.GetFrameTime());
        App.Display();

        if (m_takeScreen)
        {
            Image screenshot;
            screenshot.CopyScreen(App);
            if (!screenshot.SaveToFile("screen.jpg"))
                std::cerr << "Error : cannot save screenshot.\n";
            m_takeScreen = false;
        }
    }
}
