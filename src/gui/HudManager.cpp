#include <SFML/Graphics.hpp>
#include "HudManager.hpp"
#include "../tools/others.hpp"

using namespace sf;

void HudManager::drawFps(RenderTarget &target, const float &fpsCount,
                         const Vector2i &screenSize)
{
    static bool init = false, resetpos = true;
    static Clock refreshClock;
    static Text text;
    if (!init || refreshClock.GetElapsedTime() >= 1.f)
    {
        if (!init)
        {
            text.SetString("60");
            text.SetCharacterSize(20);
            init = true;
        }
        float fps = 1.f / fpsCount;
        if (fps < 0)
        {
            fps = 0;
            resetpos = true;
        }
        else if (fps < 100)
            resetpos = true;
        else
            resetpos = true;
        text.SetString(nbToText(static_cast<int>(fps)));
        if (resetpos)
        {
            Vector2f size(text.GetRect().Width, text.GetRect().Height);
            text.SetPosition(screenSize.x-size.x, screenSize.y-size.y);
            resetpos = false;
        }
        refreshClock.Reset();
    }
    target.Draw(text);
}

