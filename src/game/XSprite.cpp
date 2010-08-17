#include <SFML/Graphics.hpp>
#include "XSprite.hpp"
#include "../db/XSpriteItem.hpp"

XSprite::XSprite() : m_playing(false), m_anim(0)
{

}

XSprite::~XSprite()
{
    m_anim = 0;
}

void XSprite::update(const float &frametime)
{
    if (m_anim == 0)
        return;
}

void XSprite::play(const std::string &name)
{

}

bool XSprite::isPaused() const
{
    return (!m_playing);
}
