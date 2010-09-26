#include <SFGUI/Align.hpp>
#include "EditorGui.hpp"
#include "../constantes.hpp"
#include "../tools/ImageManager.hpp"

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
}

void EditorGui::addItemToTerrainList(const std::string &type,
    const std::string &image, const IntRect &subRect)
{
    if (image.empty())
        return;
    Image *ptr = gImageManager.getResource(image);
    if (ptr == 0)
        return;
    m_listTerrainItems.push_back(p_listItem(type, m_listTerrain->GetItemCount()));
    Sprite sprite(*ptr);
        sprite.SetSubRect(subRect);
    m_listTerrain->AddItem(sprite);
}

void EditorGui::terrainListItemClicked()
{

}
