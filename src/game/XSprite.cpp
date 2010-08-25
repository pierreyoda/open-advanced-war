#include <SFML/Graphics.hpp>
#include "XSprite.hpp"
#include "../db/XSpriteItem.hpp"
#include "../tools/ImageManager.hpp"
#include "../tools/FilesPathHandler.hpp"

XSprite::XSprite() : m_loop(false), m_currentFrame(0), m_anim(0)
{

}

XSprite::~XSprite()
{
    m_anim = 0;
}

void XSprite::update()
{
    if (m_anim == 0 || m_timer.isPaused())
        return;
    const float time = m_timer.getElapsedTime();
    const db::Frame *framePtr = (*m_anim)[m_currentFrame];
    if (framePtr == 0)
        return;
    if (time >= framePtr->duration)
    {
        if (m_currentFrame+1 >= m_anim->frameNumber())
            restartAnim();
        else
            setFrame(++m_currentFrame);
    }
}

void XSprite::playAnim(const db::Animation *anim)
{
    playAnim(anim, true);
}

void XSprite::playAnim(const db::Animation *anim, const bool &loop)
{
    if (anim == 0)
        return;
    stopAnim();
    m_anim = anim;
    m_loop = loop;
    setFrame(0);
    m_timer.start();
}

void XSprite::setFrame(const unsigned int &id)
{
    if (m_anim == 0)
        return;
    const db::Frame *framePtr = (*m_anim)[id];
    if (framePtr == 0)
        return;
    SetImage(*gImageManager.getResource(gFph(m_anim->image())));
    SetSubRect(sf::IntRect(framePtr->x, framePtr->y, framePtr->w, framePtr->h));
}

void XSprite::pauseAnim()
{
    m_timer.pause();
}

void XSprite::stopAnim()
{
    m_timer.reset(true);
    m_currentFrame = 0;
}

void XSprite::startAnim()
{
    m_timer.start();
}

void XSprite::restartAnim()
{
    stopAnim();
    startAnim();
}

bool XSprite::isAnimPaused() const
{
    return (m_timer.isPaused());
}
