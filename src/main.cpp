#include <iostream>
#include <string>
#include <vector>
#include <locale>
extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include <luabind/luabind.hpp>
#include "Engine.hpp"

struct WStringTool
{
    static std::string narrow(const std::wstring& ws)
    {
        std::vector<char> buffer(ws.size());
        std::locale loc("english");
        std::use_facet< std::ctype<wchar_t> >(loc).narrow(ws.data(), ws.data() + ws.size(), '?', &buffer[0]);

        return std::string(&buffer[0], buffer.size());
    /*std::string s;
    s.assign(ws.begin(), ws.end());
    return s;*/
    }

    static std::wstring widen(const std::string& s)
    {
        std::vector<wchar_t> buffer(s.size());
        std::locale loc("english");
        std::use_facet< std::ctype<wchar_t> >(loc).widen(s.data(), s.data() + s.size(), &buffer[0]);

        return std::wstring(&buffer[0], buffer.size());
        /*std::wstring ws;
        ws.assign(s.begin(), s.end());
        return ws;*/
    }
};

using namespace std;
using namespace luabind;

struct Hello
{
    Hello(const string &a) : b(a) { }

    void speek(const wstring &toSay) { wcout << toSay << "\n"; }
    void speek(const int &toSay) { cout << toSay << "\n"; }
    void blah() { cout << b << "\n"; }

    private:
        string b;
};

int main(int argc, char *argv[])
{
    lua_State *luaState = lua_open();
    luaL_openlibs(luaState);
    open(luaState);

    module(luaState)
    [
        class_<WStringTool>("wstring")
            .scope
            [
                def("L", &WStringTool::widen)
            ]
        ,
        class_<Hello>("Hello")
            .def(constructor<string>())
            .def("speek", (void(Hello::*)(const wstring&))&Hello::speek)
            .def("speek", (void(Hello::*)(const int&))&Hello::speek)
            .def("blah", &Hello::blah)
    ];

    try
    {
        Hello test("Test class from C++");
        globals(luaState)["test"] = &test;
        if (luaL_dofile(luaState, "test.lua") != 0)
            throw string(lua_tostring(luaState, -1));
        /*if (lua_pcall(luaState, 0, LUA_MULTRET, 0) != 0)
            throw string(lua_tostring(luaState, -1));*/
        cout << "All was OK!\n";
    }
    catch (const string &error)
    {
        cerr << "[LUA] Script error : " << error << "\n";
        lua_pop(luaState, 1);
    }
    catch(const exception &error)
    {
        cerr << error.what() << "\n";
    }

    Engine engine;
    engine.run();

    lua_close(luaState);

    return EXIT_SUCCESS;
}
