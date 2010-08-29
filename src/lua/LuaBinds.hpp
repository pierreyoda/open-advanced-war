#ifndef LUADATABASE_HPP
#define LUADATABASE_HPP

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
}
#include <luabind/luabind.hpp>

/** \brief Stores static functions that bind C++ classes and functions to Lua.
*/
class LuaBinds
{
    public:
        /** \brief Binds database to lua.
        */
        static void exportDatabase(lua_State *lua);
#ifndef DB_EXPORTER
        /** \brief Binds global general tools to lua.
        */
        static void exportTools(lua_State *lua);
        /** \brief Binds game to lua.
        */
        static void exportGame(lua_State *lua);
#endif /* DB_EXPORTER */

    private:
        LuaBinds() { }
        ~LuaBinds() { }
};

#endif /* LUADATABASE_HPP */
