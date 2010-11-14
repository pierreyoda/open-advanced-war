#include <iostream>
#include <sstream>
#include <SFGUI/Align.hpp>
#include <SFML/Graphics.hpp>
#include "GuiManager.hpp"
#include "../constantes.hpp"
#include "../tools/others.hpp"
#include "../tools/ImageManager.hpp"

using namespace sf;

GuiManager::GuiManager(const std::string &background, const FloatRect &rect) :
    m_gui(rect)
{
    m_gui.EnableRenderToImage(false); // otherwise causes some bugs in screenshots
    m_gui.LoadSkinFromFile("data/default.skin");

	Image *ptr = gImageManager.getResource(background);
	if (ptr != 0)
        m_background = new Sprite(*ptr);
}

GuiManager::~GuiManager()
{
    delete m_background;
}

void GuiManager::render(RenderTarget &target, const float &frametime)
{
    // Drawing background in tiles (depends from image and rect sizes)
    if (m_background != 0)
    {
        for (unsigned int i = 0; i < m_gui.GetRect().Height;
            i += m_background->GetImage()->GetHeight())
            for (unsigned int j = 0; j < m_gui.GetRect().Width;
                j += m_background->GetImage()->GetWidth())
            {
                m_background->SetPosition(j, i);
                target.Draw(*m_background);
            }
    }
    m_gui.Render(target);
}

bool GuiManager::handleEvent(const Event &event)
{
    return m_gui.HandleEvent(event);
}
