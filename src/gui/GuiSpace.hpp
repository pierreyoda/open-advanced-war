#ifndef GUISPACE_HPP
#define GUISPACE_HPP

#include <list>
#include <SFGUI/Listbox.hpp>
#include "GuiManager.hpp"

typedef std::pair<std::string, unsigned int> p_ItemId;

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

/** \brief Base class of all in-game GUIs. Inherits from GuiManager.
*/
class GuiSpace : public GuiManager
{
    public:
        GuiSpace(const std::string &name);
        virtual ~GuiSpace();

        void addButton(const std::string &id, const std::string &text,
            const sf::Vector2f &size, const sf::Vector2f &padding);
        void addVerticalSpriteList(const std::string &name,
            const sf::Vector2f &size, const sf::Vector2f &padding,
            const std::string &luaFunction);


        const std::string name() { return m_name; }
        const std::string inFunctionName() { return m_inFunctionName; }

    private:
        void onButtonClicked(sfg::Widget::Ptr widget); // SLOT
        void onListItemSelected(sfg::Widget::Ptr widget); // SLOT

        const std::string m_name;
        std::string m_inFunctionName;
        std::list<sfg::Button::Ptr> m_buttons;
        std::list<VerticalSpriteItemList> m_verticalLists;
};

#endif /* GUISPACE_HPP */
