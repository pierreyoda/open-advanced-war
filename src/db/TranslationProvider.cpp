#include "TranslationProvider.hpp"

using namespace std;

namespace db
{
    TranslationProvider::TranslationProvider()
    {

    }

    string TranslationProvider::tr(const string &lang, const string &item,
        const bool &add)
    {
        l_string::iterator iter = checkItem(item, add);
        l_translation::iterator iter2 = checkLang(lang, add);
        if (iter == m_items.end() || iter2 == m_translations.end())
            return item;
        const l_itemTranslation &translations = iter2->translations;
        for (l_itemTranslation::const_iterator iter = translations.begin();
            iter != translations.end(); iter++)
        {
            // To implement
        }
        return "";
    }

    l_string::iterator TranslationProvider::checkItem(const string &item,
    const bool &add)
    {
        l_string::iterator iter = find(m_items.begin(),
            m_items.end(), item);
        if (iter == m_items.end() && add)
        {
            m_items.push_back(item);
            return find(m_items.begin(), m_items.end(), item); // not optimized
        }
        return iter;
    }

    l_translation::iterator TranslationProvider::checkLang(const string &lang,
        const bool &add)
    {
        l_translation::iterator iter = find(m_translations.begin(),
            m_translations.end(), lang);
        if (iter == m_translations.end() && add)
        {
            m_translations.push_back(TranslationHandler(lang));
            return find(m_translations.begin(),
                m_translations.end(), lang); /* not optimized
(but here not very important because there are not thousands of languages) */
        }
        return iter;
    }
} /* End of namespace db */
