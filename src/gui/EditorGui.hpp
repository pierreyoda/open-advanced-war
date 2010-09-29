#ifndef EDITORGUI_HPP
#define EDITORGUI_HPP

#include <list>
#include <SFGUI/ListBox.hpp>
#include "GuiManager.hpp"

typedef std::pair<std::string, unsigned int> p_ItemId;

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

struct VerticalSpriteItemList
{
    VerticalSpriteItemList(const std::string &name, const sf::IntRect &rect,
        const std::string &luaBuildFunctionName);

    std::string name() const { return m_list->GetId(); }

    void addItem(const std::string &id, const std::string &image,
        const sf::IntRect &subRect);
    std::string getIdFromIndex(const unsigned int &index) const;
    sfg::SpriteListbox::Ptr getList() { return m_list; }

    private:
        sfg::SpriteListbox::Ptr m_list;
        std::list<p_ItemId> m_ids;
};

class EditorGui : public GuiManager
{
    public:
        EditorGui();

        void addVerticalSpriteList(const std::string &name,
            const sf::Vector2f &size, const sf::Vector2f &padding,
            const std::string &luaFunction);

    private:
        void listItemSelected(sfg::Widget::Ptr widget);

        std::list<VerticalSpriteItemList> m_verticalLists;
};

#endif /* EDITORGUI_HPP */
