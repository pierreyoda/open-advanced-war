#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <algorithm>
#include "DatabaseItem.hpp"

namespace db
{
    /**
    * \brief A category is a group of database items (mostly units) that is used to simplifiate database sripting and handling.
    */
    class Category : public DatabaseItem
    {
        friend class boost::serialization::access;

        public:
            /**
            * \brief Default constructor.
            * \param name Category name (example : "infantry").
            */
            Category(const std::string &name) : DatabaseItem(name)
            { }


            /** Adds an item to the category if not already present.
            *
            * \param item Item's name.
            */
            void addItem(const std::string &item)
            {
                if (!isItemIn(item))
                    m_items.push_back(item);
            }

            /** Searchs for an item in the categorty
            *
            * \param item Item's name.
            * \return True if in category, False otherwise.
            */
            bool isItemIn(const std::string &item) const
            {
                return (std::find(m_items.begin(), m_items.end(), item)
                    != m_items.end());
            }

        private:
            Category() : DatabaseItem("")
            { }

            l_string m_items;

            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("DatabaseItem",
                    boost::serialization::base_object<DatabaseItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_items);
            }
    };
} /* End of namespace db */

#endif /* CATEGORY_HPP */
