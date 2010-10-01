#ifndef DATABASESERIALIZATION_HPP
#define DATABASESERIALIZATION_HPP

#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/filesystem/path.hpp>
#include "Database.hpp"

/** \brief Provides functions to (de)serialize the database.
*/
struct DatabaseSerialization
{
    static bool exportToXml(const std::string &filename)
    {
        std::ofstream file(filename.c_str());
        if (!file)
            throw "Error : cannot save database to XML at '" + filename
                + "'.";
        try
        {
            boost::archive::xml_oarchive archive(file);
            archive << boost::serialization::make_nvp("database", database);
        }
        catch (const std::exception &exception)
        {
            std::cerr <<  "[DATABASE EXPORT] Error : " << exception.what()
                << "\n";
            return false;
        }
        return true;
    }
    static bool importFromXml(const std::string &filename)
    {
        std::ifstream file(filename.c_str());
        if (!file)
            throw "Error : cannot open XML database at '" + filename
                + "'.";
        try
        {
            boost::archive::xml_iarchive archive(file);
            archive >> BOOST_SERIALIZATION_NVP(database);
        }
        catch (const std::exception &exception)
        {
            std::cerr << "[DATABASE IMPORT] Error : " << exception.what()
                << "\n";
            return false;
        }
        return true;
    }
};

#endif /* DATABASESERIALIZATION_HPP */
