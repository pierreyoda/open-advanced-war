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
* \endcode
*/

#include <iostream>
#include <string>
#include <vector>
#include "Engine.hpp"
#include "db/Database.hpp"
#include "lua/LuaVirtualMachine.hpp"
#include "tools/FilesPathHandler.hpp"
#include "tools/others.hpp"

FilesPathHandler gFph = FilesPathHandler();

using namespace std;
using namespace luabind;

int main(int argc, char *argv[])
{
    lua_State *luaState = 0;
    LuaVM &luaVm = LuaVM::getInstance();
    luaVm.init(luaState);
    globals(luaVm())["vm"] = &luaVm;
    globals(luaVm())["database"] = &database;
    globals(luaVm())["trans"] = &database.translationsRef();
    globals(luaVm())["gFph"] = &gFph;

    luaVm.include("test.lua");

    Engine engine;
    engine.run();

    printSystemPause();

    return 0;
}
