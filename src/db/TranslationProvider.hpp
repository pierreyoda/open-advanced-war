#ifndef TRANSLATIONPROVIDER_HPP
#define TRANSLATIONPROVIDER_HPP

#include <algorithm>
#include <SFML/System/String.hpp> // Unicode support
#include "DatabaseItem.hpp"

namespace db
{
    typedef std::list< std::pair<std::string, std::string> > l_itemTranslation;
    /** \brief Contains translations from english to another language.
    */
    struct TranslationHandler : public DatabaseItem
    {
            /**
            * \brief Default constructor.
            * \param name Language name (like "french", "spanish" or "italian").
            */
        TranslationHandler(const std::string &name) : DatabaseItem(name)
        { }

        bool operator==(const std::string &name) const
        {
            return (name == this->name());
        }
        l_itemTranslation translations; // needs to be replaced by a better solution (vector<string>?)
    };

    template<class Archive>
    void serialize(Archive &ar, TranslationHandler &tr, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(tr);
    }

    typedef std::list<TranslationHandler> l_translation; /** < Typedef for translation list. **/

    /** \brief Provides translations of the required item (if existing) in the required language (if existing).
    */
    class TranslationProvider
    {
        public:
            /** \brief Default constructor.
            */
            TranslationProvider();

            /**
            * \brief Returns the required item's tranlation in the required language.
            * \param lang Required item (example : "Save", "Soldier", "cancel").
            * \param item Required language (example : "fr", "arab", "spanish").
            * \param add Add "item" and/or "lang" if not existing (true by default).
            * \return Item's translation if "lang" and "item" exist. In case of error : returns "item".
            */
            std::string tr(const std::string &lang, const std::string &item,
                const bool &add = true);
            /**
            * \see tr()
            **/
            std::string operator()(const std::string &lang,
                const std::string &item)
            {
                return tr(lang, item);
            }

        private:
            l_string::iterator checkItem(const std::string &item,
                const bool &add);
            l_translation::iterator checkLang(const std::string &lang,
                const bool &add);

            std::list<std::string> m_items; /** < Items to be translated (in english). **/
            l_translation m_translations; /** < Translations. **/
    };
    template<class Archive>
    void serialize(Archive &ar, TranslationProvider &tr, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(tr);
        ar &tr.m_items, &tr.m_translations;
    }
} /* End of namespace db */

#endif /* TRANSLATIONPROVIDER_HPP */
