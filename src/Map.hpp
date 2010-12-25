#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <boost/shared_ptr.hpp>
#include "GameGlobals.hpp"
#include "game/GameEntity.hpp"

namespace sf
{
    class RenderTarget;
    typedef Vector2<int> Vector2i;
}

/// TODO (Pierre-Yves#3#): Add documentation to all functions getTile... and setTile... .
/** \brief Represents a map.
*/
class Map
{
    friend class boost::serialization::access;

    public:
        Map(const sf::Vector2i &size = sf::Vector2i(gg.screen_w/gg.case_w,
            gg.screen_h/gg.case_h));
        ~Map();

        void renderTo(sf::RenderTarget &target);

        sf::Vector2i size() const
        {
           return m_size;
        }

        void onMouseOver(const sf::Vector2i &tilePos, const bool &nomore = false);

        bool placeBuilding(const sf::Vector2i &pos, const std::string &type,
            const std::string &faction, const bool &force);
        bool placeBuilding(const unsigned int &x, const unsigned int &y,
            const std::string &faction,  const std::string &type,
            const bool &force);
        void removeBuilding(const sf::Vector2i &pos);
        bool isBuildingPresent(const sf::Vector2i &pos);
        GameEntity *getBuilding(const sf::Vector2i &pos);
        std::string getBuildingType(const sf::Vector2i &pos);
        sf::Vector2i findBuildingPos(const std::string &type,
            const std::string &faction) const;

        std::string getTileType(const sf::Vector2i &pos) const;
        std::string getTileType(const unsigned int &x, const unsigned int &y)
            const;
        Orientation getTileOrientation(const sf::Vector2i &pos) const;
        Orientation getTileOrientation(const unsigned int &x,
            const unsigned int &y) const;
        GameEntity *getTilePtr(const sf::Vector2i &pos);
        GameEntity *getTilePtr(const unsigned int &x, const unsigned int &y);
       const  GameEntity *getTileConstPtr(const sf::Vector2i &pos) const;
        const GameEntity *getTileConstPtr(const unsigned int &x,
            const unsigned int &y) const;
        void setTile(const sf::Vector2i &pos, const std::string &type);
        void setTile(const unsigned int &x, const unsigned int &y,
            const std::string &type);
        void setTileAnim(const sf::Vector2i &pos, const std::string &anim);
        void setTileAnim(const unsigned int &x, const unsigned int &y,
            const std::string &anim);
        void setTileOrientation(const sf::Vector2i &pos, const std::string &type,
            const Orientation &orientation);
        void setTileOrientation(const unsigned int &x, const unsigned int &y,
            const Orientation &orientation);

        bool isInsideMap(const sf::Vector2i &pos) const;
        bool isInsideMap(const unsigned int &x, const unsigned int &y) const;

    private:
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            ar &BOOST_SERIALIZATION_NVP(m_tiles);
            ar &BOOST_SERIALIZATION_NVP(m_buildings);
            m_size = sf::Vector2i(0, m_tiles.size());
            if (m_size.y >= 1)
                m_size.x = m_tiles[0].size();
        }

        std::vector< std::vector<GameEntity*> > m_tiles; /**<  Vector of tiles.*/
        std::list<GameEntity*> m_buildings; /**< List of buildings. */
        GameEntity *m_prevMouseOver;
        sf::Vector2i m_size;
};

#endif /* MAP_HPP */
