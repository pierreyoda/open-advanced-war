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
        m_timer.reset();
        if (m_currentFrame+1 < m_anim->frameNumber())
            ++m_currentFrame;
        else
        {
            if (m_loop)
                m_currentFrame = 0;
            else
                stopAnim();
        }
        setFrame(m_currentFrame);
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
    m_currentFrame = id;
    sf::Image *ptr = gImageManager.getResource(gFph(m_anim->image()));
    if (ptr == 0)
        return;
    SetImage(*ptr);
    SetSubRect(sf::IntRect(framePtr->x, framePtr->y, framePtr->w, framePtr->h));
}

void XSprite::pauseAnim()
{
    m_timer.pause();
}

void XSprite::stopAnim()
{
    m_timer.reset(true);
}

void XSprite::startAnim()
{
    m_timer.start();
}

void XSprite::restartAnim()
{
    stopAnim();
    startAnim();
    setFrame(0);
}

bool XSprite::isAnimPaused() const
{
    return (m_timer.isPaused());
}

bool XSprite::isAnimStopped() const
{
    return (m_timer.isPaused() && m_timer.getElapsedTime() <= 0.01);
}
