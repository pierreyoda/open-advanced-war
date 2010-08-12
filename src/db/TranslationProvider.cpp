#include "TranslationProvider.hpp"

using namespace std;

namespace db
{
    TranslationProvider::TranslationProvider() : m_selectedLang(0)
    {

    }
    TranslationProvider::~TranslationProvider()
    {
        m_items.clear();
        m_selectedLang = 0;
        m_translations.clear();
        delete m_selectedLang;
    }

    void TranslationProvider::selectLang(const string &lang)
    {
        if (lang.empty()) // Invalid string format
            return;
        if (m_selectedLang != 0 && *m_selectedLang == lang) // Already selected
            return;
        for (l_translation::iterator iter = m_translations.begin();
            iter != m_translations.end(); iter++)
        {
            if (iter->name() == lang)
            {
                m_selectedLang = &*iter;
                return;
            }
        }
        m_translations.insert(m_translations.begin(), lang);
        m_selectedLang = &*m_translations.begin();
    }

    void TranslationProvider::translateItem(const string &item, const string &tr)
    {
        if (m_selectedLang == 0)
            return;
        l_itemTranslation &translations = m_selectedLang->translationsRef();
        for (l_itemTranslation::iterator iter = translations.begin();
            iter != translations.end(); iter++)
        {
            if (iter->first == item)
            {
                iter->second = tr;
                return;
            }
        }
        translations.push_back(p_string(item, tr));
    }

    string TranslationProvider::tr(const string &item, const bool &add)
    {
        if (item.empty())
            return item;
        l_string::iterator iter = checkItem(item, add);
        if (iter == m_items.end() || m_selectedLang == 0)
            return item;
        const l_itemTranslation &translations = m_selectedLang->translationsRef();
        for (l_itemTranslation::const_iterator iter = translations.begin();
            iter != translations.end(); iter++)
        {
            if (iter->first == item)
            {
                if (iter->second.empty())
                    return item;
                return iter->second;
            }
        }
        return item;
    }

    l_string::iterator TranslationProvider::checkItem(const string &item,
    const bool &add)
    {
        l_string::iterator iter = find(m_items.begin(),
            m_items.end(), item);
        if (iter == m_items.end() && add)
        {
            m_items.insert(m_items.begin(), item);
            return m_items.begin();
        }
        return iter;
    }
} /* End of namespace db */
