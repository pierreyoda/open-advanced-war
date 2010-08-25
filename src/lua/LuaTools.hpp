#ifndef LUATOOLS_HPP
#define LUATOOLS_HPP

/** \file LuaTools.hpp
* \brief Binds tools to Lua.
*/

#include "../tools/PausableClock.hpp"
#include "../tools/FilesPathHandler.hpp"

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
        , class_<FilesPathHandler>("FilesPathHandler")
            .def("addFile", (void(FilesPathHandler::*)(const std::string&,
                const std::string&))&FilesPathHandler::addFile)
            .def("addFile", (void(FilesPathHandler::*)(const std::string&,
                const std::string&, const bool&))&FilesPathHandler::addFile)
            .def("getFilepath", &FilesPathHandler::getFilepath)
    ];
}

#endif /* LUATOOLS_HPP */
