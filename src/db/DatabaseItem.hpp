#ifndef DATABASEITEM_HPP
#define DATABASEITEM_HPP

#include <list>
#include <string>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

typedef std::list<std::string> l_string;

namespace db
{
    /**
    * \brief Represents a database item.
     */
    struct DatabaseItem
    {
        /**
        * \brief Default constructor.
        * \param name The item name (example : "mySoldier").
        */
        DatabaseItem(const std::string &name) : m_name(name)
        { }

        /**
        * \brief m_name accessor
        * \return The item name (m_name).
        */
        const std::string &name() const { return m_name; }

        private:
            const std::string m_name; /**<  Item name. */
    };

    template<class Archive>
    void serialize(Archive &ar, DatabaseItem &dbItem, const unsigned int &version)
    {
        ar &dbItem.m_name;
    }

} /* End of namespace db */

BOOST_CLASS_VERSION(db::DatabaseItem, 1)

#endif /* DATABASEITEM_HPP */
