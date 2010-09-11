#ifndef XSPRITE_HPP
#define XSPRITE_HPP

#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include "../tools/PausableClock.hpp"

namespace db
{
    struct Animation;
    struct XSpriteItem;
}

/** \brief An eXtended Sprite that can be animated. Inspired from Hiura's animation classes.
*
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
        * \param frametime Frametime (time elapsed sinced last loop turn).
        */
        void update();

        /** \brief Plays the required animation.
        *
        * \param anim Pointer to an animation.
        */
        void playAnim(const db::Animation *anim);
        /** \brief Plays the required animation.
        *
        * \param anim Pointer to an animation.
        * \param loop Play in buckle?
        */
        void playAnim(const db::Animation *anim, const bool &loop);

        /** \brief Change the current frame to the required frame (if existing).
        *
        * \param id Frame's id.
        */
        void setFrame(const unsigned int &id);

        /** \brief If playing an animation, pauses it.
        */
        void pauseAnim();
        /** \brief If playing an animation, stops it.
        */
        void stopAnim();
        /** \brief If animation is paused, starts it.
        */
        void startAnim();
        /** \brief Restarts the animation.
        */
        void restartAnim();

        /** \brief Sets the filter.
        *
        * \param filter New filter.
        * \param use Use the filter (true by default).
        */
        void setFilter(const sf::Color &filter, const bool &use);
        void setFilter(const sf::Color &filter) { setFilter(filter, true);} // for Lua
        /** \brief Toggles filter usage.
        */
        void useFilter(const bool &use);

        bool isAnimPaused() const;
        bool isAnimStopped() const;
        /** \brief m_useFilter accessor.
        *
        * \see m_useFilter
        * \return m_useFilter.
        */
        bool isFilterUsed() const { return m_useFilter; }
        /** \brief m_filter accessor.
        *
        * \see m_filter
        * \return m_filter.
        */
        sf::Color currentFilter() const { return m_filter; }
        /** \brief m_currentFrame accessor.
        *
        * \see m_currentFrame
        * \return m_currentFrame.
        */
        unsigned int currentFrame() const { return m_currentFrame; }

    private:
        void updateFilter(const bool &remove = false);

        bool m_loop; /**< Play in buckle? */
        unsigned int m_currentFrame; /**< Current frame ID. */
        const db::Animation *m_anim; /**< Pointer to animation to play. */
        PausableClock m_timer; /**< Pausable clock. */
        bool m_useFilter; /**< Apply filter? */
        sf::Color m_filter; /**< Eventual color to apply as filter. */
};

#endif /* XSPRITE_HPP */
