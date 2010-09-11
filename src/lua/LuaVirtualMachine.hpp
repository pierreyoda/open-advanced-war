#ifndef LUAVIRTUALMACHINE_HPP
#define LUAVIRTUALMACHINE_HPP

#include <list>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include "../tools/Singleton.hpp"
#include "LuaBinds.hpp"
#include "../db/DatabaseItem.hpp"

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
            std::vector<std::string> files;
            boost::algorithm::split(files, toInclude, boost::is_any_of(";"));
            for (unsigned int i = 0; i < files.size(); i++)
            {
                const std::string &file = prefix + files[i];
                l_string::const_iterator iter = std::find(
                    filesAlreadyLoaded.begin(), filesAlreadyLoaded.end(), file);
                if (iter != filesAlreadyLoaded.end())
                {
                    std::cerr << "Warning : '" << file << "' is already loaded. Skipping...\n";
                    continue;
                }
                filesAlreadyLoaded.push_back(file);
                try
                {
                    if (luaL_dofile(luaVm, file.c_str()) != 0)
                        throw std::string(lua_tostring(luaVm, -1));
                }
                catch (const std::string &error)
                {
                    std::cerr << error << "\n";
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
