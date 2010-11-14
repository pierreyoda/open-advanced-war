#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <SFGUI/GUI.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Checkbox.hpp>

namespace sf
{
    class RenderTarget;
    class Event;
    class Sprite;
}
namespace sfg
{
    class Skin;
}

/** \brief A class that handles GUI (with sfgui). Can be inherited to make a main menu, an in-game menu...
*/
class GuiManager
{
    public:
        GuiManager(const std::string &background, const sf::FloatRect &rect);
        virtual ~GuiManager();

        virtual void render(sf::RenderTarget &target, const float &frametime);
        virtual bool handleEvent(const sf::Event &event);

    protected:
        sfg::GUI &getGui() { return m_gui; }

    private:
        sfg::GUI m_gui;
        sf::Sprite *m_background;
};

#endif /* GUIMANAGER_HPP */
