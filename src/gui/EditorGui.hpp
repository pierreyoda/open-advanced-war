#ifndef EDITORGUI_HPP
#define EDITORGUI_HPP

#include <list>
#include <SFGUI/ListBox.hpp>
#include "GuiManager.hpp"

typedef std::pair<std::string, unsigned int> p_listItem;

class EditorGui : public GuiManager
{
    public:
        EditorGui();

        void addItemToTerrainList(const std::string &type,
            const std::string &image, const sf::IntRect &subRect);

    private:
        void terrainListItemClicked();

        sfg::SpriteListbox::Ptr m_listTerrain; /**< List of tiles and buildings. */
        std::list<p_listItem> m_listTerrainItems;
};

#endif /* EDITORGUI_HPP */
