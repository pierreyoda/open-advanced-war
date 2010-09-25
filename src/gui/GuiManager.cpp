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
    m_gui.LoadSkinFromFile("data/default.skin");

	Image *ptr = gImageManager.getResource(background);
	if (ptr != 0)
        m_background = new Sprite(*ptr);
}

GuiManager::~GuiManager()
{

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

InGameGui::InGameGui() : GuiManager("data/paper.png",
    FloatRect(0, 0, SCREEN_W, GUI_END_H))
{
    m_showFpsBox = sfg::Checkbox::Create(FloatRect(0, GUI_START_H, 50, 30), L"Show FPS");
    m_fpsCounterLabel = sfg::Label::Create(Vector2f(0, GUI_START_H+GUI_PART_H/1.5f), L"0");
    m_endTurnButton = sfg::Button::Create(FloatRect(0,0,70,50), L"End turn");

    getGui().AddWidget(m_showFpsBox);
    getGui().AddWidget(m_fpsCounterLabel);
    getGui().AddWidget(m_endTurnButton);

    sfg::AlignWidgetInRect(
		*m_showFpsBox,
		FloatRect(0, GUI_START_H, SCREEN_W, GUI_PART_H),
		sfg::AlignLeft | sfg::AlignTop,
		Vector2f(5, 5)
	);
	sfg::AlignWidgetInRect(
		*m_fpsCounterLabel,
		FloatRect(0, GUI_START_H, SCREEN_W, GUI_PART_H),
		sfg::AlignLeft | sfg::AlignBottom,
		Vector2f(5, m_fpsCounterLabel->GetRect().Height+25)
	);
	sfg::AlignWidgetInRect(
		*m_endTurnButton,
		FloatRect(0, GUI_START_H, SCREEN_W, GUI_PART_H),
		sfg::AlignRight | sfg::AlignMiddle,
		Vector2f(50, 0)
	);

    getGui().ApplySkinSettings(m_fpsCounterLabel, "FPSLabel");

    m_showFpsBox->SetChecked(true);
    m_showFpsBox->SetSpacing(5.f);
    m_showFpsBox->Checked = sfg::Slot<sfg::Checkbox::CheckSlot>(
        &InGameGui::onShowFpsBoxToggled, this);
    m_endTurnButton->Clicked = sfg::Slot<sfg::Button::ClickSlot>(
        &InGameGui::onEndTurnButtonEnded, this);
}

void InGameGui::render(RenderTarget &target, const float &frametime)
{
    GuiManager::render(target, frametime);
    static Clock updateClock; // to avoid excessive FPS counter refresh
    if (frametime <= 0 || updateClock.GetElapsedTime() < 1.f)
        return;
    updateClock.Reset();
    m_fpsCounterLabel->SetString(nbToText<int>((int)1.f/frametime));
}

void InGameGui::onShowFpsBoxToggled(sfg::Widget::Ptr widget)
{
    m_fpsCounterLabel->Show(m_showFpsBox->IsChecked());
}

void InGameGui::onEndTurnButtonEnded(sfg::Widget::Ptr widget)
{
    std::cout << "End turn!\n";
}
