#ifndef EDITORGUI_HPP
#define EDITORGUI_HPP

#include <list>
#include <SFGUI/ListBox.hpp>
#include "GuiManager.hpp"

typedef std::pair<std::string, unsigned int> p_listItem;

/*class SpriteWithLabelItemView
{
    public:
        typedef boost::shared_ptr<SpriteWithLabelItemView> Ptr;

        SpriteWithLabelItemView(const sf::Sprite &sprite, const sf::String &labelText);

        const sf::FloatRect GetRect() const;
        void SetRect(const sf::FloatRect &rect);

        void Render(sf::RenderTarget &target);

    private:
        sf::Sprite m_sprite;
        sfg::Label::Ptr m_label;
};*/

class EditorGui : public GuiManager
{
    public:
        EditorGui();

        void addItemToTerrainList(const std::string &type,
            const std::string &image, const sf::IntRect &subRect);

    private:
        void terrainListItemClicked(sfg::Widget::Ptr widget);

        sfg::SpriteListbox::Ptr m_listTerrain; /**< List of tiles and buildings. */
        std::list<p_listItem> m_listTerrainItems;
};

#endif /* EDITORGUI_HPP */
