#include <SFGUI/Align.hpp>
#include <SFGUI/Listbox.hpp>
#include "EditorGui.hpp"
#include "../constantes.hpp"
#include "../tools/ImageManager.hpp"
#include "../Game.hpp"

using namespace sf;

EditorGui::EditorGui() : GuiManager("data/paper.png",
    FloatRect(0, 0, SCREEN_W, GUI_END_H))
{
    m_listTerrain = sfg::SpriteListbox::Create(FloatRect(0, 0, 500, 90));
    sfg::AlignWidgetInRect(
		*m_listTerrain,
		FloatRect(0, GUI_START_H, SCREEN_W, GUI_PART_H),
		sfg::AlignLeft| sfg::AlignTop,
		Vector2f(75, 5)
	);

    getGui().AddWidget(m_listTerrain);

    m_listTerrain->Selected = sfg::Slot<sfg::DefaultSlot>(
        &EditorGui::terrainListItemClicked, this);
}

void EditorGui::addItemToTerrainList(const std::string &type,
    const std::string &image, const IntRect &subRect)
{
    if (image.empty())
        return;
    Image *ptr = gImageManager.getResource(image);
    if (ptr == 0)
        return;
    for (std::list<p_listItem>::const_iterator iter = m_listTerrainItems.begin();
        iter != m_listTerrainItems.end(); iter++)
        if (iter->first == type) // already present
            return;
    m_listTerrainItems.push_back(p_listItem(type, m_listTerrain->GetItemCount()));
    Sprite sprite(*ptr);
        sprite.SetSubRect(subRect);
    m_listTerrain->AddItem(sprite);
}

void EditorGui::terrainListItemClicked(sfg::Widget::Ptr widget)
{
    unsigned int index = m_listTerrain->GetSelectedIndex();
    for (std::list<p_listItem>::const_iterator iter = m_listTerrainItems.begin();
    iter != m_listTerrainItems.end(); iter++)
    if (iter->second == index) // already present
        gGame.changeCurrentTerrain(iter->first);
}
