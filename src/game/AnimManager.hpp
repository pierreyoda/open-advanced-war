#ifndef ANIMMANAGER_HPP
#define ANIMMANAGER_HPP

#include <string>

namespace db
{
    struct Animation;
}

/** \brief Facilitates searching for animations in database.
*
*/
struct AnimManager
{
    public:
        AnimManager();
        ~AnimManager();

        const db::Animation *search(const std::string &item,
            const std::string &anim);
};

#endif /* ANIMMANAGER_HPP */
