#ifndef LUATOOLS_HPP
#define LUATOOLS_HPP

/** \file LuaTools.hpp
* \brief Binds tools to Lua.
*/

#include "../tools/PausableClock.hpp"

void exportTools(lua_State *lua)
{
    using namespace luabind;
    module(lua)
    [
        class_<PausableClock>("PausableClock")
            .def(constructor< >())
            .def(constructor<bool>())
            .def("pause", &PausableClock::pause)
            .def("start", &PausableClock::start)
            .def("getElapsedTime", &PausableClock::getElapsedTime)
            .def("reset", (void(PausableClock::*)())&PausableClock::reset)
            .def("reset", (void(PausableClock::*)(const bool&))&PausableClock::reset)
    ];
}

#endif /* LUATOOLS_HPP */
