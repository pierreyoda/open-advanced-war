#ifndef LUAVIRTUALMACHINE_HPP
#define LUAVIRTUALMACHINE_HPP

#include <list>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include "LuaDatabase.hpp"
#include "LuaTools.hpp"

/** \brief Handles lua interpreter.
*/
class LuaVM
{
    public:
        LuaVM(lua_State *lua)
        {
            if (lua == 0)
                lua = lua_open();
            luaVm = lua;
            luabind::open(luaVm);
            luaL_openlibs(luaVm);
            luabind::module(luaVm)
            [
                luabind::class_<LuaVM>("VM")
                    .def("include", &LuaVM::include)
            ];
            exportDatabase(luaVm);
            exportTools(luaVm);
        }
        ~LuaVM()
        {
            lua_close(luaVm);
        }

        lua_State *operator()() { return luaVm; }
        lua_State *getLua() { return luaVm; }

        /** \brief Makes load required file(s) by Lua embedded interpretor. Use this instead of 'require'.
         *
         * \param toInclude A file to load, or some files separated by ';' (relative or absolute path).
         */
        void include(const std::string &toInclude)
        {
            std::vector<std::string> files;
            boost::algorithm::split(files, toInclude, boost::is_any_of(";"));
            for (unsigned int i = 0; i < files.size(); i++)
            {
                const std::string &file = files[i];
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

    private:
        lua_State *luaVm;
        l_string filesAlreadyLoaded;
};

#endif /* LUAVIRTUALMACHINE_HPP */
