#ifndef TRANSLATIONPROVIDER_HPP
#define TRANSLATIONPROVIDER_HPP

#include <algorithm>
//#include <SFML/System/String.hpp> // Unicode support
#include "DatabaseItem.hpp"

namespace db
{
    typedef std::pair<const std::string, std::string> p_string; // Maybe a pointer for first string?
    typedef std::list<p_string> l_itemTranslation;
    /** \brief Contains translations from english to another language.
    */
    struct TranslationHandler : public DatabaseItem
    {
        friend class boost::serialization::access;

            /**
            * \brief Default constructor.
            * \param name Language name (like "french", "spanish" or "italian").
            */
        TranslationHandler(const std::string &name) : DatabaseItem(name)
        { }

        bool operator==(const std::string &name) const // For convenience
        {
            return (name == this->name());
        }
        l_itemTranslation &translationsRef() { return m_translations; }

        private:
            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("DatabaseItem",
                    boost::serialization::base_object<DatabaseItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_translations);
            }

            l_itemTranslation m_translations;
    };

    typedef std::list<TranslationHandler> l_translation; /** < Typedef for translation list. **/

    /** \brief Provides translations of the required item (if existing) in the required language (if existing).
    */
    class TranslationProvider
    {
        friend class boost::serialization::access;

        public:
            /** \brief Default constructor.
            */
            TranslationProvider();
            ~TranslationProvider();

            /**
            * \brief Returns the required item's tranlation in the selected language.
            * \see selectLang()
            * \param item Required item (example : "Save", "Soldier", "cancel").
            * \param add Add "item" and/or "lang" if not existing (true by default).
            * \return Item's translation if "lang" and "item" exist. In case of error : returns "item".
            */
            std::string tr(const std::string &item, const bool &add);
            std::string tr(const std::string &item) { return tr(item, true); } // Needed for lua side.
            /**
            * \see tr()
            **/
            std::string operator()(const std::string &item)
            {
                return tr(item);
            }
            /**
            * \brief Select the required language to avoid excessive std::find call.
            * \param lang Language to select (add if not existing).
            */
            void selectLang(const std::string &lang);
            /**
            * \brief Translates the selected item in the selected language.
            * \param item Item to translate.
            * \param tr Item's translation in the selected language.
            * \see selectLang()
            */
            void translateItem(const std::string &item, const std::string &tr);

        private:
            template<class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &BOOST_SERIALIZATION_NVP(m_items);
                ar &BOOST_SERIALIZATION_NVP(m_translations);
            }

            l_string::iterator checkItem(const std::string &item,
                const bool &add);

            std::list<std::string> m_items; /** < Items to be translated (in english). **/
            l_translation m_translations; /** < Translations. **/
            TranslationHandler *m_selectedLang;
    };
} /* End of namespace db */

#endif /* TRANSLATIONPROVIDER_HPP */
