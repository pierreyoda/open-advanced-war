/*
Open Advanced War is an open turn by turn strategic game directly inspired of advance(/famicon) wars (c).
Copyright (C) 2010  Pierre-Yves Diallo (Pierreyoda)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Website: https://sourceforge.net/projects/openadvancedwar/<br />
E-mail: pierreyoda33@gmail.com
*/
/**
* \mainpage Open Advanced War
* Open Advanced War is an open turn by turn strategic game directly inspired of advance(/famicon) wars (c).
*
* Copyright (C) 2010  Pierre-Yves Diallo (Pierreyoda).
*
* Website: https://sourceforge.net/projects/openadvancedwar/<br />
* E-mail: pierreyoda33@gmail.com
*
* License : GPL (v3) - See main.cpp or gpl3.txt<br />
*/

#include <iostream>
#include <string>
#include <vector>
#include "Engine.hpp"
#include "db/Database.hpp"
#include "lua/LuaVirtualMachine.hpp"

using namespace std;
using namespace luabind;

struct Hello
{
    Hello(const string &a) : b(a) { }

    void speek(const string &toSay) { cout << toSay << "\n"; }
    void speek(const int &toSay) { cout << toSay << "\n"; }
    void blah() { cout << b << "\n"; }

    private:
        string b;
};

int main(int argc, char *argv[])
{
    lua_State *luaVm = 0;
    LuaVM luaState(luaVm);
    globals(luaState())["vm"] = &luaState;

    module(luaState())
    [
        class_<Hello>("Hello")
            .def(constructor<string>())
            .def("speek", (void(Hello::*)(const string&))&Hello::speek)
            .def("speek", (void(Hello::*)(const int&))&Hello::speek)
            .def("blah", &Hello::blah)
        , namespace_("db")
        [
            class_<db::TranslationProvider>("TranslationProvider")
                .def("tr", &db::TranslationProvider::tr)
                //.def("tr", (std::string &db::TranslationProvider::tr)
                .def("selectLang", &db::TranslationProvider::selectLang)
                .def("translateItem", &db::TranslationProvider::translateItem)
        ]
    ];

    db::Database db("Vanilla");

    try
    {
        Hello test("Test class from C++");
        globals(luaState())["test"] = &test;
            globals(luaState())["trans"] = &db.translationsRef();
        if (luaL_dofile(luaState(), "test.lua") != 0)
            throw string(lua_tostring(luaState(), -1));
        /*if (lua_pcall(luaState, 0, LUA_MULTRET, 0) != 0)
            throw string(lua_tostring(luaState, -1));*/
        cout << "All was OK!\n";
    }
    catch (const string &error)
    {
        cerr << "[LUA] Script error : " << error << "\n";
    }
    catch (const exception &error)
    {
        cerr << error.what() << "\n";
    }

    /*cout << "--- Starting UTF-8 test (compiled) ---\n";
    cout << sf::String(L"é_(éèç").ToAnsiString() << "\n";
    cout << "--- Ending UTF-8 test (compiled) ---\n";*/

    Engine engine;
    engine.run();

    return EXIT_SUCCESS;
}
