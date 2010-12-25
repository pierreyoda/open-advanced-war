#ifndef UNIT_HPP
#define UNIT_HPP

#include "XSpriteItem.hpp"

namespace db
{
    /** \brief A (template) class that represents a Feature (name and [maximum in most case] value).
    */
    template <typename Type>
    struct Feature
    {
        friend class boost::serialization::access;

        Feature(const std::string &name, const Type &value) :
            name(name), value(value)
        { }

        std::string name;
        Type value;

        private:
            Feature() : name(), value(Type())
            { }
    };
    template <class Archive, typename Type>
    void serialize(Archive &ar, Feature<Type> &carac,
        const unsigned int &version)
    {
        ar &boost::serialization::make_nvp("name",
            carac.name);
        ar &boost::serialization::make_nvp("value",
            carac.value);
    }

    template <typename Type>
    void addFeature(const std::string &name, const Type &value,
        std::list< Feature<Type> > &in)
    {
        Feature<Type> *ptr = findFeature(name, in);
        if (ptr != 0) // found
            ptr->value = value;
        else
            in.push_back(Feature<Type>(name, value));
    }
    template <typename Type>
    Feature<Type> *findFeature(const std::string &name,
        std::list< Feature<Type> > &in)
    {
        for (typename std::list< Feature<Type> >::iterator
            iter = in.begin(); iter != in.end(); iter++)
            if (iter->name == name)
                return &*iter;
        return 0;
    }
    template <typename Type>
    const Feature<Type> *findFeatureConst(const std::string &name,
        const std::list< Feature<Type> > &in)
    {
        for (typename std::list< Feature<Type> >::const_iterator
            iter = in.begin(); iter != in.end(); iter++)
            if (iter->name == name)
                return &*iter;
        return 0;
    }

    typedef Feature<int> IntFeature;
    typedef Feature<std::string> StringFeature;

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

            /** \brief Add a Feature, integer. If already existing, will be replaced.
            *
            * \param name Feature's name.
            * \param value Feature's value.
            * \return Reference to self.
            */
            Unit &addIntFeature(const std::string &name, const int &value)
            {
                addFeature<int>(name, value, m_intFeatures);
                return *this;
            }

            /** \brief Find a feature, integer Returns default value (0) if not found.
            *
            * \param name Feature's name.
            */
            int findIntFeature(const std::string &name) const
            {
                const IntFeature *ptr = findFeatureConst<int>(name,
                    m_intFeatures);
                if (ptr == 0) // not found
                    return 0;
                return ptr->value;
            }

        private:
            Unit() : XSpriteItem("")
            { }

            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("XSpriteItem",
                    boost::serialization::base_object<XSpriteItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_propulsion);
                ar &BOOST_SERIALIZATION_NVP(m_intFeatures);
            }

            std::string m_propulsion;
            std::list<IntFeature> m_intFeatures;
    };
} /* End of namespace db */

#endif /* UNIT_HPP */
