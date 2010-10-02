#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <list>
#include "XSprite.hpp"
#include <boost/serialization/serialization.hpp>
#include "../db/Unit.hpp"

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

/** \brief Enumeration of entity classes.
*/
enum Classes
{
    NONE, /**< Not found (for errors). */
    TILE, /**< Tile. */
    BUILDING, /**< Building. */
    UNIT /**< Unit. */
};

class GameEntity
{
    friend class boost::serialization::access;

    public:
        GameEntity(const std::string &type, const std::string &faction);
        virtual ~GameEntity();

        void updatePosition();
        void setPosition(const sf::Vector2i &pos);
        void setPosition(const int &x, const int &y);
        void setOrientation(const Orientation &orientation);
        void changeOwnerId(const unsigned int &ownerId) { m_ownerId = ownerId; }

        void playAnim(const std::string &anim) { playAnim(anim, true); }
        void playAnim(const std::string &anim, const bool &loop);

        Classes getClass() const { return m_class; }
        std::string type() const { return m_type; }
        std::string alias() const { return m_alias; }
        unsigned int ownerId() const { return m_ownerId; }

        sf::Vector2i position() const { return m_pos; }
        Orientation orientation() const { return m_orientation; }
        XSprite &xsprite() { return m_xsprite; }
        const XSprite &xspriteConst() const { return m_xsprite; }

        GameEntity &setIntCaracteristic(const std::string &name,
            const int &value);
        GameEntity &setBoolCaracteristic(const std::string &name,
            const bool &value);
        GameEntity &setStringCaracteristic(const std::string &name,
            const std::string &value);
        int getIntCaracteristic(const std::string &name,
            const int &default_ = 0);
        Tribool getBoolCaracteristic(const std::string &name);
        std::string getStringCaracteristic(const std::string &name);

        static Classes findClassFromType(const std::string &type);
        static sf::Vector2i pixelsToTiles(const sf::Vector2i &pos);
        static sf::Vector2i pixelsToTiles(const int &posX, const int &posY);
        static sf::Vector2i pixelsToTiles(const sf::Vector2f &pos);
        static sf::Vector2f tilesToPixels(const sf::Vector2i &pos);

    private:
        GameEntity() : m_class(NONE) { }

        template <typename Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            ar &BOOST_SERIALIZATION_NVP(m_class);
            ar &BOOST_SERIALIZATION_NVP(m_pos);
                updatePosition(); // useless on saving ; should split serialize into save/load
            ar &BOOST_SERIALIZATION_NVP(m_type);
            ar &BOOST_SERIALIZATION_NVP(m_alias);
            ar &BOOST_SERIALIZATION_NVP(m_ownerId);
            //ar &BOOST_SERIALIZATION_NVP(m_caracteristics);
            ar &BOOST_SERIALIZATION_NVP(m_orientation);
        }

        Classes m_class; /**< Entity's class (ex : "UNIT", "BUILDING"). */
        sf::Vector2i m_pos; /**< Entity's position (not in pixel but in "tiles"). */
        std::string m_type; /**< Entity type (ex : "tank  factory", "soldier"). */
        std::string m_alias; /**< Entity alias (optionnal; ex : "leaderA", "VIP"). */
        std::string m_faction; /**< Entity faction (null by default). */
        unsigned int m_ownerId; /**< Entity (eventual) owner identifier (army ID). */
        std::list<db::IntCaracteristic> m_intCaracteristics; /**< List of int caracteristics. */
        std::list<db::BoolCaracteristic> m_boolCaracteristics; /**< List of bool caracteristics. */
        std::list<db::StringCaracteristic> m_stringCaracteristics; /**< List of string caracteristics. */
        Orientation m_orientation; /**< Entity's orientation (by default right). */
        XSprite m_xsprite; /**< Entity's eXtended sprite. */
};

#endif /* GAMEENTITY_HPP */
