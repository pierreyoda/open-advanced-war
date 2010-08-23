#ifndef DATABASESERIALIZATION_HPP
#define DATABASESERIALIZATION_HPP

#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "Database.hpp"

/** \brief Provides functions to (de)serialize the database.
*/
struct DatabaseSerialization
{
    void exportToXml(const std::string &filename)
    {
        std::ofstream file(filename.c_str());
        if (!file)
            throw "Error : cannot save database to XML at '" + filename
                + "'.";
        {
            boost::archive::xml_oarchive archive(file);
            //archive << BOOST_SERIALIZATION_NVP(database);
        }
    }
    void importFromXml(const std::string &filename)
    {

    }
};

#endif /* DATABASESERIALIZATION_HPP */
