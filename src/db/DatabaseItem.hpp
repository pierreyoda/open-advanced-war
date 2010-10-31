#ifndef DATABASEITEM_HPP
#define DATABASEITEM_HPP

#include <list>
#include <string>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/nvp.hpp>

typedef std::list<std::string> l_string;

namespace db
{
    /**
    * \brief Represents a database item.
     */
    struct DatabaseItem
    {
        friend class boost::serialization::access;

        /** \brief Default constructor.
        *
        * \param name The item name (example : "mySoldier").
        */
        DatabaseItem(const std::string &name) : m_name(name)
        { }

        /** \brief m_name accessor
        *
        * \return The item name (m_name).
        */
        std::string name() const { return m_name; }

        private:
            DatabaseItem() : m_name() // for deserialization (construction)
            { }

            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &BOOST_SERIALIZATION_NVP(m_name);
            }

            std::string m_name; /**<  Item name. */
    };

    template <typename Derived>
    Derived *findItemIn(const std::string &item, std::list<Derived> &in)
    {
        if (item.empty())
            return 0;
        for (typename std::list<Derived>::iterator iter = in.begin();
            iter != in.end(); iter++)
        if (iter->name() == item)
            return &*iter;
        return 0;
    }
    template <typename Derived>
    const Derived *findItemInConst(const std::string &item,
        const std::list<Derived> &in)
    {
        if (item.empty())
            return 0;
        for (typename std::list<Derived>::const_iterator iter = in.begin();
            iter != in.end(); iter++)
        if (iter->name() == item)
            return &*iter;
        return 0;
    }
} /* End of namespace db */

#endif /* DATABASEITEM_HPP */
