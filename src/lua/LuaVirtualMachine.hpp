#ifndef LUAVIRTUALMACHINE_HPP
#define LUAVIRTUALMACHINE_HPP

#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <algorithm>
#include "../tools/Singleton.hpp"
#include "LuaBinds.hpp"
#include "../db/DatabaseItem.hpp"

#define PRINT_LUA_ERROR(LUA) std::cerr << lua_tostring(LUA, -1) << "\n";

/* A macro that facilitates lua function calling.
* LUA : Pointer to lua_State used.
* RTYPE : Return type
* NAME : function name
* ERROR : variable to change in case of error
* ARGS : arguments (variadic)
*/
#define CALL_LUA_FUNCTION(LUA, RTYPE, NAME, ERROR, ARGS...) \
    try { \
    luabind::call_function<RTYPE>(LUA, \
    NAME, \
    ARGS); } \
    catch (const std::exception &exception) { PRINT_LUA_ERROR(LUA); ERROR=true; }

/* A macro that facilitates lua function calling (with taking in account what is returned).
* LUA : Pointer to lua_State used.
* RTYPE : Return type
* RVAR : Return variable
* NAME : function name
* ERROR : variable to change in case of error
* ARGS : arguments (variadic)
*/
#define CALL_LUA_RFUNCTION(LUA, RTYPE, RVAR, NAME, ERROR, ARGS...) \
    try { \
    RVAR = luabind::call_function<RTYPE>(LUA, \
    NAME, \
    ARGS); } \
    catch (const std::exception &exception) { PRINT_LUA_ERROR(LUA); ERROR=true; }

/** \brief Handles lua interpreter.
*/
class LuaVM : public Singleton<LuaVM>
{
    friend class Singleton<LuaVM>;

    public:
        lua_State *operator()() { return luaVm; }
        lua_State *getLua() { return luaVm; }

        void init(lua_State *lua)
        {
            if (luaVm != 0) // Already initialized
                return;
            if (lua == 0)
                lua = lua_open();
            luaVm = lua;
            luabind::open(luaVm);
            luaL_openlibs(luaVm);
            luabind::module(luaVm)
            [
                luabind::class_<LuaVM>("VM")
                    .def("include", (void(LuaVM::*)(const std::string&))
                        &LuaVM::include)
                    .def("include", (void(LuaVM::*)(const std::string&,
                        const std::string&))&LuaVM::include)
            ];
            LuaBinds::exportDatabase(luaVm);
#ifndef DB_EXPORTER
            LuaBinds::exportTools(luaVm);
            LuaBinds::exportGame(luaVm);
#endif /* DB_EXPORTER */
        }

        /** \brief Overloaded for conveniance and lua binding.
        * \see include()
        */
        void include(const std::string &toInclude)
        {
            include(toInclude, "");
        }
        /** \brief Makes load required file(s) by Lua embedded interpretor. Use this instead of 'require'.
         *
         * \param toInclude A file to load, or some files separated by ';' (relative or absolute path).
         * \param prefix Prefix (ex : "modules/Native/").
         */
        void include(const std::string &toInclude, const std::string &prefix)
        {
            if (toInclude.empty())
                return;
            std::vector<std::string> files;
            boost::algorithm::split(files, toInclude, boost::is_any_of(";"));
            boost::filesystem::path dir = boost::filesystem::initial_path() / prefix;
            for (unsigned int i = 0; i < files.size(); i++)
            {
                const boost::filesystem::path file = dir / files[i];
                const std::string fileString = file.string();
                l_string::const_iterator iter = std::find(
                    filesAlreadyLoaded.begin(), filesAlreadyLoaded.end(), file);
                if (iter != filesAlreadyLoaded.end())
                {
                    std::cerr << "Warning : '" << file << "' is already loaded. Skipping...\n";
                    continue;
                }
                filesAlreadyLoaded.push_back(fileString);
                try
                {
                    if (luaL_dofile(luaVm, fileString.c_str()) != 0)
                        throw std::string();
                }
                catch (const std::string &error)
                {
                    std::cerr << "[Lua] " << fileString << " : " << lua_tostring(luaVm, -1) << "\n";
                }
                catch (const std::exception &exception)
                {
                    std::cerr << exception.what() << "\n";
                }
            }
        }

        /** \brief Extracts a lua variable (as copy so to use for simple objects like strings or numbers).
        *
        * \param name Variable's name.
        * \return A copy of the variable or the default of Type in case of error.
        */
        template <typename Type>
        Type extractVariable(const std::string &name)
        {
            using namespace luabind;

            Type toReturn;
            try
            {
                toReturn = object_cast<Type>(globals(luaVm)[name]);
            }
            catch (const std::exception &exception)
            {
                std::cout << "[LUA] : variable '" << name << "' extracting error : "
                    << exception.what() << "\n";
                toReturn = Type();
            }
            return toReturn;
        }

        static int add_file_and_line(lua_State* L) // from : Luabind doc
        {
           lua_Debug d;
           lua_getstack(L, 1, &d);
           lua_getinfo(L, "Sln", &d);
           std::string err = lua_tostring(L, -1);
           lua_pop(L, 1);
           std::stringstream msg;
           msg << d.short_src << ":" << d.currentline;

           if (d.name != 0)
           {
              msg << "(" << d.namewhat << " " << d.name << ")";
           }
           msg << " " << err;
           lua_pushstring(L, msg.str().c_str());
           return 1;
        }

    private:
        LuaVM()
        {

        }
        ~LuaVM()
        {
            lua_close(luaVm);
        }

        lua_State *luaVm;
        l_string filesAlreadyLoaded;
};

#endif /* LUAVIRTUALMACHINE_HPP */
