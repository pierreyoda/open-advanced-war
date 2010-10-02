#ifndef XSPRITEITEM_HPP
#define XSPRITEITEM_HPP

#include <vector>
#include "DatabaseItem.hpp"
#include "../GameGlobals.hpp"
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>

namespace db
{
    /** \brief Contains informations to create an animation's frame.
    */
    struct Frame
    {
        friend class boost::serialization::access;

        Frame(const unsigned int &x, const unsigned int &y, const float &duration)
        {
            this->x = x, this->y = y, this->w = gg.case_w, this->h = gg.case_h;
            this->duration = duration;
        }
        Frame(const unsigned int &x, const unsigned int &y,
            const unsigned int &w, const unsigned int &h, const float &duration)
        {
            this->x = x, this->y = y, this->w = w, this->h = h;
            this->duration = duration;
        }

        unsigned int x, y, w, h;
        float duration; /** < Frame's duration. */

        private:
            Frame() : x(0), y(0), w(0), h(0), duration(0)
            { }

            template<class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &BOOST_SERIALIZATION_NVP(x);
                ar &BOOST_SERIALIZATION_NVP(y);
                ar &BOOST_SERIALIZATION_NVP(w);
                ar &BOOST_SERIALIZATION_NVP(h);
                ar &BOOST_SERIALIZATION_NVP(duration);
            }
    };

    /** \brief Contains informations to create an animation.
    */
    struct Animation : public DatabaseItem
    {
        friend class boost::serialization::access;

        Animation(const std::string &name, const std::string &img) :
            DatabaseItem(name), m_image(img)
        { }

        /** \brief Removes all frames. For scripting convenience. NB : name and image are conserved.
        *
        * \return Reference to self.
        */
        Animation &clear()
        {
            m_frames.clear();
            return *this;
        }

        /** \brief Adds a frame.
        *
        * \param frame A frame.
        * \return Reference to instance (self).
        */
        Animation &addFrame(const Frame &frame)
        {
            m_frames.push_back(frame);
            return *this;
        }
        /** \brief Adds a frame (overloaded) of default case size.
        *
        * \param x X position.
        * \param y Y position.
        * \return Reference to instance (self).
        */
        Animation &addFrame(const unsigned int &x, const unsigned int &y)
        {
            m_frames.push_back(Frame(x, y, 1.f));
            return *this;
        }
        /** \brief Adds a frame (overloaded) of default case size.
        *
        * \param x X position.
        * \param y Y position.
        * \param duration Frame's duration.
        * \return Reference to instance (self).
        */
        Animation &addFrame(const unsigned int &x, const unsigned int &y,
            const float &duration)
        {
            m_frames.push_back(Frame(x, y, duration));
            return *this;
        }

        /** \brief Change image filename. For scripting convenience.
        *
        * \param image Image's filename.
        * \return Reference to self.
        */
        Animation &setImage(const std::string &image)
        {
            if (!image.empty())
                m_image = image;
            return *this;
        }

        /** \brief m_image accessor
        *
        * \return Image filename.
        */
        const std::string &image() const { return m_image; }

        /** \brief Returns the frame number.
        *
        * \return Frame's vector size.
        */
        unsigned int frameNumber() const { return m_frames.size(); }

        /** \brief Provides secured access to frames.
            *
        * \param id Frame number.
        * \return Pointer to the required frame if existing, null pointer otherwise.
        */
        const Frame *getFrame(const unsigned int &id) const
        {
            if (id < m_frames.size())
                return &m_frames[id];
            return 0;
        }

        /** \brief Operator []. Provides secured access to frames.
        *
        * \see getFrame
        */
        const Frame *operator[](const unsigned int &id) const
        {
            return getFrame(id);
        }

        private:
            Animation() : DatabaseItem("")
            { }

            template<class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("DatabaseItem",
                    boost::serialization::base_object<DatabaseItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_frames);
                ar &BOOST_SERIALIZATION_NVP(m_image);
            }

            std::vector<Frame> m_frames; /**< Frames. */
            std::string m_image; /** < Image filename. */
    };

    typedef std::list<Animation> l_anim;
    typedef std::pair<std::string, l_anim> p_animSpecs; // Animation specializations

    /** \brief Contains informations to create an eXtended Sprite (XSprite).
    */
    struct XSpriteItem : public DatabaseItem
    {
        friend class boost::serialization::access;

        XSpriteItem(const std::string &name) : DatabaseItem(name)
        { }

        XSpriteItem &addAnim(const Animation &anim)
        {
            return addAnim(anim, "");
        }
        XSpriteItem &addAnim(const Animation &anim, const std::string &faction)
        {
            l_anim *ptr = findAnimSpecs(faction);
            if (ptr == 0)
            {
                m_anims.push_front(p_animSpecs(faction, l_anim()));
                ptr = &(m_anims.begin()->second);
            }
            const Animation *ptr2 = findItemInConst<Animation>(anim.name(), *ptr);
            if (ptr2 != 0) // anim name already present in faction specialization
            {
                std::cerr << "[XSpriteItem '" << name() << "'] addAnim : "
                    << "Warning, animation '" << anim.name()
                    << "' already exists in faction specialization '" << faction
                    << "'.\n";
                return *this;
            }
            ptr->push_back(anim);
            return *this;
        }
        const Animation *findAnim(const std::string &anim,
            const std::string &faction) const
        {
            const l_anim *ptr = findAnimSpecsConst(faction);
            if (ptr == 0) // specified faction specialization not found
                return 0;
            return findItemInConst<Animation>(anim, *ptr);
        }

        private:
            l_anim *findAnimSpecs(const std::string &faction)
            {
                for (std::list<p_animSpecs>::iterator iter =
                     m_anims.begin(); iter != m_anims.end(); iter++)
                    if (iter->first == faction)
                        return &iter->second;
                return 0;
            }
            const l_anim *findAnimSpecsConst(const std::string &faction) const
            {
                for (std::list<p_animSpecs>::const_iterator iter =
                     m_anims.begin(); iter != m_anims.end(); iter++)
                    if (iter->first == faction)
                        return &iter->second;
                return 0;
            }

            template<class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("DatabaseItem",
                    boost::serialization::base_object<DatabaseItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_anims);
            }

            std::list<p_animSpecs> m_anims;
    };
} /* End of namespace db */

#endif /* XSPRITEITEM_HPP */
