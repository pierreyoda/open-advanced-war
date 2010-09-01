#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "XSpriteItem.hpp"

namespace db
{
    typedef std::pair<std::string, bool> p_producible;

    /**
    * \brief Contains informations to create a building.
    */
    class Building : public XSpriteItem
    {
        friend class boost::serialization::access;

        public:
            /**
            * \brief Default constructor.
            * \param name The building name (example : "City" or "Tank factory").
            * \param gainPerTurn Building gain per turn to the owner (can be negative).
            * \param Can be captured.
            */
            Building(const std::string &name, const int &gainPerTurn,
                const bool &capturable) :
                XSpriteItem(name), m_gainPerTurn(gainPerTurn),
                m_capturable(capturable)
            { }

            Building &setResistance(const unsigned int &resistance)
            {
                m_resistance = resistance;
                return *this;
            }
            Building &addProduction(const std::string &item,
                const bool &isCategory)
            {
                p_producible pair(item, isCategory);
                if (std::find(m_canProduce.begin(), m_canProduce.end(), pair)
                    == m_canProduce.end()) // Does not already exist
                    m_canProduce.push_back(pair);
                return *this;
            }

            int resistance() const { return m_resistance; }
            bool capturable() const { return m_capturable; }

        private:
            Building() : XSpriteItem(""), m_gainPerTurn(0)
            { }

            template<class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("XSpriteItem",
                    boost::serialization::base_object<XSpriteItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_gainPerTurn);
                ar &BOOST_SERIALIZATION_NVP(m_capturable);
                ar &BOOST_SERIALIZATION_NVP(m_resistance);
                ar &BOOST_SERIALIZATION_NVP(m_canProduce);
            }

            int m_gainPerTurn; /**<  Gain per turn (example : 1000). Can be negative. */
            bool m_capturable; /**< Can be captured or not? */
            unsigned int m_resistance; /**< Resistance if can be captured. */
            std::list<p_producible> m_canProduce; /**< List of items that building can produce. Can be a unit or a category. */
    };
} /* End of namespace db */

#endif /* BUILDING_HPP */
