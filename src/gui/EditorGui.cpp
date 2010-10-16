#include <SFGUI/Align.hpp>
#include <SFGUI/Listbox.hpp>
#include "EditorGui.hpp"
#include "../constantes.hpp"
#include "../tools/ImageManager.hpp"
#include "../lua/LuaVirtualMachine.hpp"
#include "../Game.hpp"

using namespace sf;

inline FloatRect intRectToFloatRect(const IntRect &rect)
{
    return FloatRect(rect.Left, rect.Top, rect.Width, rect.Height);
}

VerticalSpriteItemList::VerticalSpriteItemList(const std::string &name,
    const IntRect &rect, const std::string &luaBuildFunctionName) :
    m_list(sfg::SpriteListbox::Create(intRectToFloatRect(rect), name))
{
    bool error = false;
    CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void, luaBuildFunctionName.c_str(),
        error, this)
    if (error)
    {
        std::cerr << "[VerticalSpriteItemList] - Error : cannot build list '"
            << name << "'\n";
    }
}

void VerticalSpriteItemList::addItem(const std::string &id,
    const std::string &image, const IntRect &subRect)
{
    if (image.empty())
        return;
    if (id.empty())
    {
        std::cerr << "[VerticalSpriteItemList] adding item - "
            << "Error : cannot accept an empty name.\n";
        return;
    }
    Image *ptr = gImageManager.getResource(image);
    if (ptr == 0)
        return;
    for (std::list<p_ItemId>::const_iterator iter = m_ids.begin();
        iter != m_ids.end(); iter++)
        if (iter->first == id) // already present
            return;
    m_ids.push_back(p_ItemId(id, m_list->GetItemCount()));
    Sprite sprite(*ptr);
        sprite.SetSubRect(subRect);
    m_list->AddItem(sprite);
}

std::string VerticalSpriteItemList::getIdFromIndex(const unsigned int &index)
    const
{
    for (std::list<p_ItemId>::const_iterator iter = m_ids.begin();
        iter != m_ids.end(); iter++)
            if (iter->second == index) // already present
        return iter->first;
    return "";
}

EditorGui::EditorGui() : GuiManager("data/paper.png",
    FloatRect(0, 0, SCREEN_W, GUI_END_H))
{

}

/// TODO (Pierre-Yves#1#): [GUI] Add possiblity of placing buttons, lists... outside the gui space
void EditorGui::addVerticalSpriteList(const std::string &name,
    const Vector2f &size, const Vector2f &padding,
    const std::string &luaFunction)
{
    if (name.empty())
    {
        std::cerr << "[EditorGui] adding list - "
            << "Error : cannot accept an empty ID.\n";
        return;
    }
    for (std::list<VerticalSpriteItemList>::const_iterator iter =
         m_verticalLists.begin(); iter != m_verticalLists.end(); iter++)
        if (iter->name() == name) // same name already present
        {
            std::cerr << "[GUI-Editor] : Error, vertical list name '" <<
                name << "' is already used.\n";
            return;
        }
    m_verticalLists.push_front(VerticalSpriteItemList(name,
        IntRect(0, 0, size.x, size.y), luaFunction));
    sfg::SpriteListbox::Ptr list = m_verticalLists.begin()->getList();
    list->Selected = sfg::Slot<sfg::SpriteListbox::SelectSlot>(
        &EditorGui::listItemSelected, this);
    sfg::AlignWidgetInRect(
        *list,
        FloatRect(0, GUI_START_H, SCREEN_W, GUI_END_H),
        sfg::AlignLeft | sfg::AlignTop,
        padding);
    getGui().AddWidget(list);
}

void EditorGui::listItemSelected(sfg::Widget::Ptr widget)
{
    static bool error = false;
    if (error)
        return;
    VerticalSpriteItemList *ptr = 0;
    for (std::list<VerticalSpriteItemList>::iterator iter =
         m_verticalLists.begin(); iter != m_verticalLists.end(); iter++)
        if (iter->name() == widget->GetId())
            ptr = &*iter;
    if (ptr == 0)
        return;
    std::string id = ptr->getIdFromIndex(ptr->getList()->GetSelectedIndex());
    CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
        "onEditorGuiListItemSelected", error, ptr->name(), id)
}

void EditorGui::addButton(const std::string &id, const std::string &text,
    const Vector2f &size, const Vector2f &padding)
{
    if (id.empty())
    {
        std::cerr << "[EditorGui] adding button - "
            << "Error : cannot accept an empty ID.\n";
        return;
    }
    for (std::list<sfg::Button::Ptr>::const_iterator iter = m_buttons.begin();
        iter != m_buttons.end(); iter++)
        if ((*iter)->GetId() == id) // same ID already present
        {
            std::cerr << "[GUI-Editor] : Error, button ID'" <<
                id << "' is already used.\n";
            return;
        }
    sfg::Button::Ptr button = sfg::Button::Create(
        FloatRect(0, 0, size.x, size.y), text, id);
    m_buttons.push_back(button);
    button->Clicked = sfg::Slot<sfg::Button::ClickSlot>(
        &EditorGui::buttonClicked, this);
    sfg::AlignWidgetInRect(
        *button,
        FloatRect(0, GUI_START_H, SCREEN_W, GUI_END_H),
        sfg::AlignLeft | sfg::AlignTop,
        padding);
    getGui().AddWidget(button);
}

void EditorGui::buttonClicked(sfg::Widget::Ptr widget)
{
    static bool error = false;
    if (error)
        return;
    CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
        "onEditorGuiButtonClicked", error, widget->GetId())
}
