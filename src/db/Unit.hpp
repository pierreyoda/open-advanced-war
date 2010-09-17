#ifndef UNIT_HPP
#define UNIT_HPP

#include "XSpriteItem.hpp"

namespace db
{
    /** \brief A (template) class that represents a caracteristic (name and [maximum in most case] value).
    */
    template <typename Type>
    struct Caracteristic
    {
        friend class boost::serialization::access;

        Caracteristic(const std::string &name, const Type &value) :
            name(name), value(value)
        { }

        std::string name;
        Type value;

        private:
            Caracteristic() : name(), value(Type())
            { }
    };
    template <class Archive, typename Type>
    void serialize(Archive &ar, Caracteristic<Type> &carac,
        const unsigned int &version)
    {
        ar &boost::serialization::make_nvp("name",
            carac.name);
        ar &boost::serialization::make_nvp("value",
            carac.value);
    }

    typedef Caracteristic<int> IntCaracteristic;
    typedef Caracteristic<bool> BoolCaracteristic;

    /**
    * \brief Contains informations to create a tile.
    */
    class Unit : public XSpriteItem
    {
        friend class boost::serialization::access;

        public:
            /** \brief Default constructor.
            *
            * \param name The tile name (example : "light tank", "infantry").
            */
            Unit(const std::string &name) : XSpriteItem(name)
            { }

            /** \brief Add a caracteristic, integrer. If already existing, will be replaced.
            *
            * \param name Caracteristic's name.
            * \param value Caracteristic's value.
            */
            void addIntCaracteristic(const std::string &name, const int &value)
            {
                addCaracteristic<int>(name, value, m_intCaracteristics);
            }
            /** \brief Add a caracteristic, boolean. If already existing, will be replaced.
            *
            * \param name Caracteristic's name.
            * \param value Caracteristic's value.
            */
            void addBoolCaracteristic(const std::string &name, const bool &value)
            {
                addCaracteristic<bool>(name, value, m_boolCaracteristics);
            }

        private:
            Unit() : XSpriteItem("")
            { }

            template <typename Type>
            void addCaracteristic(const std::string &name, const Type &value,
                std::list< Caracteristic<Type> > &in)
            {
                for (typename std::list< Caracteristic<Type> >::iterator iter = in.begin();
                    iter != in.end(); iter++)
                {
                    if (iter->name == name)
                    {
                        iter->value = value;
                        return;
                    }
                }
                in.push_back(Caracteristic<Type>(name, value));
            }

            std::list<IntCaracteristic> m_intCaracteristics;
            std::list<BoolCaracteristic> m_boolCaracteristics;

            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("XSpriteItem",
                    boost::serialization::base_object<XSpriteItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_intCaracteristics);
                ar &BOOST_SERIALIZATION_NVP(m_boolCaracteristics);
            }
    };
} /* End of namespace db */

#endif /* UNIT_HPP */
