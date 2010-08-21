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
} /* End of namespace db */
