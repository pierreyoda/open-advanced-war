#ifndef XSPRITE_HPP
#define XSPRITE_HPP

#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include "AnimManager.hpp"

namespace db
{
    struct Animation;
    struct XSpriteItem;
}

/** \brief An eXtended Sprite that can be animated. Inspired from Hiura's animation classes.
* \see http://www.sfml-dev.org/wiki/en/sources/frame_anim_animated (English)
* \see http://www.sfml-dev.org/wiki/fr/sources/frame_anim_animated (French)
*/
class XSprite : public sf::Sprite
{
    public:
        XSprite();
        ~XSprite();

        /** \brief Updates the XSprite.
        *
        * \param frametime Frametime (time elapsed sinced last loop turn.
        */
        void update(const float &frametime);
        /** \brief Plays the required animation if found in the database.
        *
        * \param name Animation name.
        */
        void play(const std::string &name);
        /** \brief If playing an animation, stops it.
        */
        void stop();
        /** \brief If playing an animation, pauses it.
        */
        void pause();

        bool isPaused() const;
        bool isStoped() const;

    private:
        bool m_playing;
        const db::Animation *m_anim;
};

#endif /* XSPRITE_HPP */
