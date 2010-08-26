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

class GameEntity : public XSprite
{
    public:
        GameEntity(const std::string &name);
        ~GameEntity();

        void setPosition(const sf::Vector2i &pos);
        void setPosition(const int &x, const int &y);

        void playAnim(const std::string &anim, const bool &loop = true);

    private:
        std::string m_name; /**< Entity name (ex : "tank  factory", "soldier"). */
        std::string m_alias; /**< Entity alias (optionnal; ex : "leaderA", "VIP"). */
        std::list<Caracteristic> m_caracteristics; /**< List of (variables) caracteristics. */
        Orientation m_orientation;
};

#endif /* GAMEENTITY_HPP */
