#ifndef UNIT_HPP
#define UNIT_HPP

#include "XSpriteItem.hpp"

namespace
{
    /** \brief A tri-bool enumeration.
    */
    enum Tribool
    {
        INDETERMINATE,
        TRUE_ = 1,
        FALSE_ = 0
    };

    Tribool boolToTribool(const bool &boolean)
    {
        if (boolean)
            return TRUE_;
        return FALSE_;
    }
}

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
            Unit(const std::string &name) : XSpriteItem(name), m_propulsion("*none*")
            { }

            /** \brief Set the propulsion used.
            *
            * \param propulsion Propulsion's name ("*none*" forbidden).
            * \return Reference to self.
            */
            void setPropulsion(const std::string &propulsion)
            {
                if (!propulsion.empty())
                    m_propulsion = propulsion;
            }

            /** \brief Get the propulsion used.
            *
            * \return Propulsion used.
            */
            std::string propulsion() const { return m_propulsion; }

            /** \brief Add a caracteristic, integer. If already existing, will be replaced.
            *
            * \param name Caracteristic's name.
            * \param value Caracteristic's value.
            * \return Reference to self.
            */
            Unit &addIntCaracteristic(const std::string &name, const int &value)
            {
                addCaracteristic<int>(name, value, m_intCaracteristics);
                return *this;
            }
            /** \brief Add a caracteristic, boolean. If already existing, will be replaced.
            *
            * \param name Caracteristic's name.
            * \param value Caracteristic's value.
            * \return Reference to self.
            */
            Unit &addBoolCaracteristic(const std::string &name, const bool &value)
            {
                addCaracteristic<bool>(name, value, m_boolCaracteristics);
                return *this;
            }

            /** \brief Find a caracteristic., integer Returns default value (0) if not found.
            *
            * \param name Caracteristic's name.
            */
            int findIntCaracteristic(const std::string &name)
            {
                IntCaracteristic *ptr = findCaracteristic<int>(name,
                    m_intCaracteristics);
                if (ptr == 0) // not found
                    return 0;
                return ptr->value;
            }

            /** \brief Find a caracteristic., boolean. Returns "indeterminate" value if not found.
            *
            * \param name Caracteristic's name.
            */
            Tribool findBoolCaracteristic(const std::string &name)
            {
                BoolCaracteristic *ptr = findCaracteristic<bool>(name,
                    m_boolCaracteristics);
                if (ptr == 0) // not found
                    return INDETERMINATE;
                return boolToTribool(ptr->value);
            }

        private:
            Unit() : XSpriteItem("")
            { }

            template <typename Type>
            void addCaracteristic(const std::string &name, const Type &value,
                std::list< Caracteristic<Type> > &in)
            {
                Caracteristic<Type> *ptr = findCaracteristic(name, in);
                if (ptr != 0) // found
                    ptr->value = value;
                else
                    in.push_back(Caracteristic<Type>(name, value));
            }
            template <typename Type>
            Caracteristic<Type> *findCaracteristic(const std::string &name,
                std::list< Caracteristic<Type> > &in)
            {
                for (typename std::list< Caracteristic<Type> >::iterator iter = in.begin();
                    iter != in.end(); iter++)
                {
                    if (iter->name == name)
                        return &*iter;
                }
                return 0;
            }

            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("XSpriteItem",
                    boost::serialization::base_object<XSpriteItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_propulsion);
                ar &BOOST_SERIALIZATION_NVP(m_intCaracteristics);
                ar &BOOST_SERIALIZATION_NVP(m_boolCaracteristics);
            }

            std::string m_propulsion;
            std::list<IntCaracteristic> m_intCaracteristics;
            std::list<BoolCaracteristic> m_boolCaracteristics;
    };
} /* End of namespace db */

#endif /* UNIT_HPP */
