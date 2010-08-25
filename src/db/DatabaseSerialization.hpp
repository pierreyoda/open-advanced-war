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
    static void exportToXml(const std::string &filename)
    {
        std::ofstream file(filename.c_str());
        if (!file)
            throw "Error : cannot save database to XML at '" + filename
                + "'.";
        {
            boost::archive::xml_oarchive archive(file);
            //boost::archive::text_oarchive archive(file);
            //const db::Database &ref = *database;
            archive << boost::serialization::make_nvp("database", database);
        }
    }
    static void importFromXml(const std::string &filename)
    {
        std::ifstream file(filename.c_str());
        if (!file)
            throw "Error : cannot open XML database at '" + filename
                + "'.";
        {
            boost::archive::xml_iarchive archive(file);
            //boost::archive::text_iarchive archive(file);
            //archive >> BOOST_SERIALIZATION_NVP(database);
        }
    }
};

#endif /* DATABASESERIALIZATION_HPP */
