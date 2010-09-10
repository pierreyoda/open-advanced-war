#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <list>
#include "XSprite.hpp"
#include <boost/serialization/serialization.hpp>

/** \brief Enumeration of orientations.
*/
enum Orientation
{
    UNDEFINED, /**< Not defined (for errors). */
    RIGHT, /**< Looks towards the left. */
    LEFT, /**< Looks towards the right. */
    UPWARD, /**< Looks upward. */
    DOWNWARD /**< Looks downward. */
};

/** \brief Represents a caracteristic and its value.
*/
struct Caracteristic
{
    const std::string name;
    int value;
};

class GameEntity
{
    friend class boost::serialization::access;

    public:
        GameEntity(const std::string &type);
        ~GameEntity();

        void updatePosition();
        void setPosition(const sf::Vector2i &pos);
        void setPosition(const int &x, const int &y);
        void setOrientation(const Orientation &orientation);

        void playAnim(const std::string &anim) { playAnim(anim, true); }
        void playAnim(const std::string &anim, const bool &loop);

        std::string type() const { return m_type; }
        std::string alias() const { return m_alias; }

        sf::Vector2i position() const { return m_pos; }
        Orientation orientation() const { return m_orientation; }
        XSprite &xsprite() { return m_xsprite; }
        const XSprite &xspriteConst() const { return m_xsprite; }

    private:
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            ar &BOOST_SERIALIZATION_NVP(m_pos);
                updatePosition(); // useless on saving ; should split serialize into save/load
            ar &BOOST_SERIALIZATION_NVP(m_type);
            ar &BOOST_SERIALIZATION_NVP(m_alias);
            ar &BOOST_SERIALIZATION_NVP(m_caracteristics);
            ar &BOOST_SERIALIZATION_NVP(m_orientation);
        }

        sf::Vector2i m_pos; /**< Entity's position (not in pixel but in "tiles"). */
        std::string m_type; /**< Entity type (ex : "tank  factory", "soldier"). */
        std::string m_alias; /**< Entity alias (optionnal; ex : "leaderA", "VIP"). */
        std::list<Caracteristic> m_caracteristics; /**< List of (variables) caracteristics. */
        Orientation m_orientation; /**< Entity's orientation (by default right). */
        XSprite m_xsprite; /**< Entity's eXtended sprite. */
};

#endif /* GAMEENTITY_HPP */
