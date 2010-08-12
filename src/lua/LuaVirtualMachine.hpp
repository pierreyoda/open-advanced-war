#ifndef LUAVIRTUALMACHINE_HPP
#define LUAVIRTUALMACHINE_HPP

#include <list>
#include <string>
#include <vector>
extern "C"
{
    #include <lua.h>
    #include <lualib.h>
}
#include <luabind/luabind.hpp>
#include <boost/algorithm/string.hpp>

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
        }
        ~LuaVM()
        {
            lua_close(luaVm);
        }

        lua_State *operator()() { return luaVm; }
        lua_State *getLua() { return luaVm; }

    private:
        void include(const std::string toInclude)
        {
            std::vector<std::string> files;
            boost::algorithm::split(files, toInclude, boost::is_any_of(";"));
            for (unsigned int i = 0; i < files.size(); i++)
            {
                bool ok = true;
                for (std::list<std::string>::iterator iter =
                    filesAlreadyLoaded.begin(); iter != filesAlreadyLoaded.end();
                    iter++)
                {
                    if (*iter == files[i])
                    {
                        ok = false;
                        break;
                    }
                }
                if (!ok)
                {
                    std::cerr << "Warning : '" << files[i] << "' is already loaded. Skipping...\n";
                    continue;
                }
                if (luaL_dofile(luaVm, files[i].c_str()) != 0)
                    std::cerr <<  std::string(lua_tostring(luaVm, -1)) << "\n";
                else
                    filesAlreadyLoaded.push_back(files[i]);
            }
        }

        lua_State *luaVm;
        std::list<std::string> filesAlreadyLoaded;
};

#endif /* LUAVIRTUALMACHINE_HPP */
