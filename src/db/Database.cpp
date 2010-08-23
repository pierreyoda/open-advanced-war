#include <iostream>
#include "Database.hpp"

using namespace std;

db::Database *database = &db::Database::getInstance();

namespace db
{

    Database::Database() : m_moduleName("unknown")
    {

    }

    Database::~Database()
    {

    }

    Database &Database::addTile(const Tile &tile)
    {
        for (std::list<Tile>::const_iterator iter = m_tiles.begin();
            iter != m_tiles.end(); iter++)
            if (iter->name() == tile.name())
            {
                std::cerr << "Warning : tile '" << tile.name()
                    << "' already exists.\n";
                return *this;
            }
        m_tiles.push_back(tile);
        return *this;
    }
} /* End of namespace db */
