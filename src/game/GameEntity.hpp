#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <list>
#include "XSprite.hpp"

/** \brief Enumeration of orientations.
*/
enum Orientation
{
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
    public:
        GameEntity(const std::string &type);
        ~GameEntity();

        void updatePosition();
        void setPosition(const sf::Vector2i &pos);
        void setPosition(const int &x, const int &y);

        void playAnim(const std::string &anim, const bool &loop = true);

        std::string type() const { return m_type; }
        std::string alias() const { return m_alias; }

        sf::Vector2i position() const { return m_pos; }
        XSprite &xsprite() { return m_xsprite; }
        const XSprite &xspriteConst() const { return m_xsprite; }

    private:
        sf::Vector2i m_pos;
        std::string m_type; /**< Entity type (ex : "tank  factory", "soldier"). */
        std::string m_alias; /**< Entity alias (optionnal; ex : "leaderA", "VIP"). */
        std::list<Caracteristic> m_caracteristics; /**< List of (variables) caracteristics. */
        Orientation m_orientation; /**< Entity's orientation (by default right). */
        XSprite m_xsprite; /**< Entity's eXtended sprite. */
};

#endif /* GAMEENTITY_HPP */
