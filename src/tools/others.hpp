#ifndef OTHERS_HPP
#define OTHERS_HPP

/** \file others.hpp
* \brief Contains global utilities functions.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <limits>

namespace
{

/** \brief A portable version of 'system("PAUSE")'.
*
* \param Print some text?
*/
void printSystemPause(const bool &print = true)
{
    if (print)
        std::cout << "Press enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );
}

/** \brief Converts from number to std::string.
*
* \param nb Number (template).
* \return Number converted to std::string.
*/
template <typename T>
inline std::string nbToText(const T &nb)
{
    std::ostringstream oss;
    oss << nb;
    return oss.str();
}

}

#endif /* OTHERS_HPP */
