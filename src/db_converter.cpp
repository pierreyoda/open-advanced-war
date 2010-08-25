/*
Open Advanced War is an open turn by turn strategic game directly inspired of advance(/famicon) wars (c).
This is a tool for Open Advanced War.
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

#define DB_EXPORTER

#include <iostream>
#include <boost/filesystem.hpp>
#include "lua/LuaVirtualMachine.hpp"
#include "db/DatabaseSerialization.hpp"
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

using namespace std;
using namespace luabind;

string getFilePath(const std::string &indication, const bool &mustExist = true,
    const std::string &forbidden = "")
{
    cout << "Please specify the " << indication << " :";
    if (mustExist)
        cout << " (must be an existing file)";
    cout << "\n";
    bool ok = false;
    string file;
    do
    {
        cin >> file;
        if (file == "exit")
            exit(0);
        if (file == forbidden)
        {
            cout << "Error : '" << file << "' is already specified.\n";
            continue;
        }
        if (mustExist)
        {
            ok = fs::exists(file);
            if (!ok)
                cout << "Error : '" << file << "' does not exist.\n";
        }
        else
            ok = true;
    } while (!ok);
    return file;
}

int main(int argc, char *argv[])
{
    const string version = "1.0";
    cout << "Open Advanced War - Database Converter tool v. " + version << endl;
    cout << "Copyright (C) 2010  Pierre-Yves Diallo (Pierreyoda) - GPL3" << endl
        << "This program comes with ABSOLUTELY NO WARRANTY." << endl
        << "This is free software, and you are welcome to redistribute it "
        << " under certain conditions." << endl
        << "For details open provided text file 'gpl3.txt'\n" << endl;
    cout << "NB : at each step command 'exit' will close the program." << endl;

    lua_State *luaVM = 0;
    LuaVM luaState(luaVM);
    globals(luaState())["vm"] = &luaState;

    db::Database &db = db::Database::getInstance();
    globals(luaState())["database"] = &db;
    globals(luaState())["trans"] = &db.translationsRef();

    string input = /*getFilePath("Lua input file")*/"modules/Native/database.lua",
        output = getFilePath("XML output file", false, input);
    luaState.include(input);

    database->setModuleName("blabla");
    DatabaseSerialization::exportToXml(output);

    return 0;
}
