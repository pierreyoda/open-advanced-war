#ifndef XSPRITEITEM_HPP
#define XSPRITEITEM_HPP

#include "DatabaseItem.hpp"
#include "../GameGlobals.hpp"

namespace db
{
    /** \brief Contains informations to create an animation's frame.
    */
    struct Frame
    {
        Frame(const unsigned int &x, const unsigned int &y, const float &duration)
        {
            this->x = x, this->y = y, this->w = gg->case_w, this->h = gg->case_h;
            this->duration = duration;
        }
        Frame(const unsigned int &x, const unsigned int &y,
            const unsigned int &w, const unsigned int &h, const float &duration)
        {
            this->x = x, this->y = y, this->w = w, this->h = h;
            this->duration = duration;
        }

        unsigned int x, y, w, h;
        float duration;
    };
    template<class Archive>
    void serialize(Archive &ar, Frame &frame, const unsigned int &version)
    {
        ar &frame.x, &frame.y, &frame.w, &frame.h, &frame.duration;
    }

    /** \brief Contains informations to create an animation.
    */
    struct Animation : public DatabaseItem
    {
        Animation(const std::string &name, const std::string &img) :
            DatabaseItem(name), m_image(img)
        { }

        Animation &addFrame(const Frame &frame)
        {
            m_frames.push_back(frame);
            return *this;
        }

        private:
            std::list<Frame> m_frames;
            const std::string m_image;
    };
    template<class Archive>
    void serialize(Archive &ar, Animation &anim, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(anim);
        ar &anim.m_frames;
    }

    /** \brief Contains informations to create an eXtended Sprite (XSprite).
    */
    struct XSpriteItem : public DatabaseItem
    {
        XSpriteItem(const std::string &name) : DatabaseItem(name)
        { }

        XSpriteItem &addAnim(const Animation &anim)
        {
            std::list<Animation>::const_iterator iter;
            for (iter = m_anims.begin(); iter != m_anims.end(); iter++)
            {
                if (iter->name() == anim.name())
                    break;
            }
            if (iter == m_anims.end())
                m_anims.push_back(anim);
            return *this;
        }

        private:
            std::list<Animation> m_anims;
    };
    template<class Archive>
    void serialize(Archive &ar, XSpriteItem &xsprite, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(xsprite);
        ar &xsprite.m_anims;
    }
} /* End of namespace db */

BOOST_CLASS_VERSION(db::Frame, 1);
BOOST_CLASS_VERSION(db::Animation, 1);
BOOST_CLASS_VERSION(db::XSpriteItem, 1);

#endif /* XSPRITEITEM_HPP */
