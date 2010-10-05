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

#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <boost/filesystem.hpp>
#include "lua/LuaVirtualMachine.hpp"
#include "db/DatabaseSerialization.hpp"
#include "tools/others.hpp"

namespace fs = boost::filesystem;

using namespace std;
using namespace luabind;

string getLuaInputFile(const string &modulesDir)
{
    fs::path modulesPath(modulesDir);
    if (!fs::exists(modulesPath))
        throw string("Error : directory '" + modulesDir + "' not found.");
    vector<fs::path> modules;
    // getting all present directories in 'modulesDir'
    for (fs::directory_iterator iter(modulesPath); iter !=
        fs::directory_iterator(); iter++)
        if (fs::is_directory(*iter)
            && iter->filename() != ".svn") // for developpers - avoid including of SVN cache files
            modules.push_back(*iter);
    if (modules.empty())
        throw string("Error : no module(s) found in directory '" + modulesDir
            + "'.");
    // checking these directories and printing them
    unsigned int validDir = 0;
    cout << "Present modules :\n";
    for (vector<fs::path>::iterator iter = modules.begin(); iter != modules.end();
        iter++)
    {
        fs::path input = *iter / "database.lua";
        if (fs::exists(input)) // file is present
        {
            cout << validDir;
            if (validDir < 10) // justify
                cout << " ";
            cout <<  " : " << input << "\n";
            ++validDir;
        }
        else // no input file
        {
            cout << "X  : " << *iter << " - NO INPUT FILE 'database.lua' !\n";
            modules.erase(iter); // deleting the invalid folder
            --iter;
        }
    }
    // waiting for user choice
    cout << "\nPlease choose the module to 'compile'.\n";
    int idChoice = 0;
    do
    {
        idChoice = -1;
        string choice;
        cin >> choice;
        if (choice == "exit")
            throw string("User cancelled.\n");
        // managing some usual invalid typing
        if (choice.empty())
            continue;
        if (choice.size() == 1 && !isdigit(choice[0]))
            continue;
        if (choice.size() == 2 && !isdigit(choice[1]))
            continue;
        // converting the typing into a number
        std::stringstream ss;
        ss << choice;
        ss >> idChoice;
    } while (idChoice < 0 || idChoice >= (int)modules.size());
    return fs::path(modules[idChoice] / "database.lua").string();
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
    cout << "NB : at each step typing 'exit' will close the program." << endl
        << endl << endl;

    lua_State *luaState = 0;
    LuaVM &luaVM = LuaVM::getInstance();
    luaVM.init(luaState);
    globals(luaVM())["vm"] = &luaVM;

    db::Database &db = db::Database::getInstance();
        globals(luaVM())["database"] = &db;
        globals(luaVM())["trans"] = &db.translationsRef();

    try
    {
        string input = getLuaInputFile("modules/");
        if (input.empty())
            throw string("Error : no input file specified.\n");
        cout << "Processing file '" << input << "' ...\n";
        luaVM.include(input);
        database.setModuleName(luaVM.extractVariable<string>("MODULE_NAME"));
        string output = luaVM.extractVariable<string>("DATABASE_PATH");
        if (output.empty())
            throw string("Error : cannot found or invalid 'DATABASE_PATH'.");
        if (fs::exists(output) && !fs::is_regular_file(output))
            throw string("Error : path '" + output + "' already exists but is not a regular file.");
        cout << "Compiling database into '" << output << "'...\n";
        DatabaseSerialization::exportToXml(output);
    }
    catch (const string &error)
    {
        cerr << error << "\n";
        printSystemPause(false);
        printSystemPause();
        return 1;
    }
    catch (const exception &error)
    {
        cerr << error.what() << "\n";
        printSystemPause();
        return 1;
    }

    cout << "Done.\n";

    printSystemPause(false); // needed but don't know why...
    printSystemPause();
    return 0;
}
