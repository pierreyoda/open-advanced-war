#include <SFML/Graphics.hpp>
#include <SFGUI/Align.hpp>
/*#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>*/
#include "Game.hpp"
#include "db/DatabaseSerialization.hpp"
#include "lua/LuaVirtualMachine.hpp"
#include "tools/FilesPathHandler.hpp"
#include "tools/others.hpp"
#include "Map.hpp"
#include "game/ArmyGeneral.hpp"
#include "game/GameEntity.hpp"
#include "constantes.hpp"

Game &gGame = Game::getInstance();

using namespace std;

Game::Game() : App(0), m_mapPtr(0), m_unitDeleted(false)
{

}

Game::~Game()
{
    delete m_mapPtr;
    for (map<string, GuiSpace*>::iterator iter = m_GUIs.begin();
        iter != m_GUIs.end(); iter++)
        delete iter->second;
}

void criticalModuleError()
{
    cerr << "Critical module error : game will now exit.\n";
    printSystemPause();
    exit(1);
}

void Game::initTestMap()
{
    string path = "modules/AW1/"; /// TODO (Pierre-Yves#1#): Make user can choose module (a list with sfgui? in an option menu?)
    if (path.empty() || !boost::filesystem::exists(path))
    {
        cerr << "Error :  '" << path << "' is an invalid module path.\n";
        criticalModuleError();
    }
    const boost::filesystem::path modulePath(path), mainPath = modulePath / "main.lua";
    if (!boost::filesystem::exists(mainPath))
    {
        cerr << "Error : file 'main.lua' is not present in module path '"
            << modulePath << "\n";
        criticalModuleError();
    }
    LuaVM::getInstance().include(mainPath.string());
    string databasePath(LuaVM::getInstance().extractVariable<string>(
        "DATABASE_PATH"));
    if (databasePath.empty() || !boost::filesystem::exists(databasePath)) // invalid file
    {
        cerr << "Error :  '" << databasePath << "' is an invalid database file path.\n";
        criticalModuleError();
    }
    if (!DatabaseSerialization::importFromXml(databasePath))
    {
        cerr << "Error while loading database file '" << databasePath << "'.\n";
        criticalModuleError();
    }
    if (database.getModuleName() !=
        LuaVM::getInstance().extractVariable<string>("MODULE_NAME"))
    {
        cerr << "Error, module and database does not have the same names.\n";
        criticalModuleError();
    }
}

void Game::newMap()
{
    if (m_mapPtr != 0)
        delete m_mapPtr;
    m_mapPtr = new Map();
}

bool Game::saveMap(const string &filename)
{
    if (m_mapPtr == 0)
        return false;
    try
    {
        ofstream file(filename.c_str());
        if (!file)
            throw string("cannot save map to " + filename + ".");
        boost::archive::xml_oarchive archive(file);
        archive << boost::serialization::make_nvp("map", *m_mapPtr);
        archive << BOOST_SERIALIZATION_NVP(m_armies);
    }
    catch (const string &error)
    {
        cout << "[Map saving] Error : " << error << "\n";
        return false;
    }
    catch (const exception &exception)
    {
        cout << "[Map saving] Error : " << exception.what()
         << "\n";
        return false;
    }
    return true;
}

bool Game::loadMap(const string &filename)
{
    if (m_mapPtr == 0)
        return false;
    try
    {
        ifstream file(filename.c_str());
        if (!file)
            throw string("cannot load map from " + filename + ".");
        boost::archive::xml_iarchive archive(file);
        archive >> boost::serialization::make_nvp("map", *m_mapPtr);
        archive >> BOOST_SERIALIZATION_NVP(m_armies);
    }
    catch (const string &error)
    {
        cout << "[Map loading] Error : " << error << "\n";
        return false;
    }
    catch (const exception &exception)
    {
        cout << "[Map loading] Error : " << exception.what()
         << "\n";
        return false;
    }
    static bool luaError = false;
    if (!luaError)
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void, "onMapLoaded",
            luaError, m_mapPtr)
    return true;
}

void Game::onMouseOver(const sf::Vector2i &mousePos)
{
    const sf::Vector2i mousePosTiles(GameEntity::pixelsToTiles(mousePos));
    Unit *ptr = 0;
    static Unit *prevUnit = 0;
    static sf::Vector2i prevUnitPos;
    for (unsigned int i = 0; i < m_armies.size(); i++)
        if (m_armies[i] != 0)
        {
            Unit *ptr2 = m_armies[i]->getUnitPtr(mousePosTiles);
            if (ptr2 == 0)
                continue;
            ptr = ptr2;
            break;
        }
    static bool luaError = false, luaError2 = false;
    if (m_unitDeleted && prevUnitPos == m_unitDeletedPos)
        prevUnit = 0;
    if (!luaError && prevUnit != 0)
    {
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
            "onMouseNoMoreOverGameEntity", luaError, (GameEntity*)prevUnit) // upcasting
        prevUnit = 0;
    }
    if (ptr != 0)
    {
        if (!luaError2)
            CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
                "onMouseOverGameEntity", luaError2, (GameEntity*)ptr) // upcasting
        prevUnit = ptr;
        prevUnitPos = prevUnit->position();
    }
    if (m_mapPtr != 0)
        m_mapPtr->onMouseOver(mousePosTiles, (ptr != 0));
}

void Game::listenInput(const sf::Input &Input)
{
    const sf::Vector2i mousePosTiles(GameEntity::pixelsToTiles(
        Input.GetMouseX(), Input.GetMouseY()));
    static sf::Vector2i prevPos(-1, -1);
    if (prevPos != mousePosTiles && Input.IsMouseButtonDown(sf::Mouse::Left)
        && m_mapPtr != 0)
    {
        prevPos = mousePosTiles;
        /*if (!m_unit.empty())
            spawnUnit(0, m_unit, mousePosTiles);
        else if (!m_building.empty())
            m_mapPtr->placeBuilding(mousePosTiles, m_building, m_faction, true);
        else
            m_mapPtr->setTile(mousePosTiles, m_tile);*/
    }
    static bool luaError = false;
    if (!luaError)
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void, "listenInput",
            luaError, &Input)
}

void Game::listenEvent(const sf::Event &Event)
{
    if (Event.Type == sf::Event::MouseMoved)
    {
        const int x = Event.MouseMove.X, y = Event.MouseMove.Y;
        onMouseOver(sf::Vector2i(x, y));
    }
    if (!m_currentGui.empty() && m_GUIs[m_currentGui] != 0)
        m_GUIs[m_currentGui]->handleEvent(Event);
    static bool luaError = false;
    if (!luaError)
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void, "onEvent",
            luaError, &Event)
}

void Game::addGui(const string &name, const bool &setAsCurrent)
{
    if (name.empty())
    {
        cerr << "[Game - addGui] Error : cannot add a GUI with an empty name.\n";
        return;
    }
    if (m_GUIs[name] != 0) // does exist
    {
        cerr << "[Game - addGui] Error : GUI name '" << name
            << "' already exists.\n";
        return;
    }
    GuiSpace *newGui = new GuiSpace(name);
    bool luaError = false;
    string functionName = "build" + newGui->inFunctionName();
    CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void, functionName.c_str(),
        luaError, newGui)
    if (luaError)
    {
        cerr << "[Game - addGui] Error, cannot build GUI '" << name << "'.\n";
        delete newGui;
        return;
    }
    m_GUIs[name] = newGui;
    if (setAsCurrent)
        m_currentGui = name;
}

void Game::setCurrentGui(const string &name)
{
    if (name.empty() || m_GUIs[name] == 0)
    {
        cerr << "[Game - setCurrentGui] Error :  invalid name.\n";
        return;
    }
    m_currentGui = name;
}

void Game::addArmy(const unsigned int &id, const string &name)
{
    if (name.empty())
    {
        cerr << "[Game - addArmy] Error : cannot add an army with an empty name"
            << "\n";
        return;
    }
    if (getArmy(id) != 0) // already exists
    {
        cerr << "[Game - addArmy] Error : army with ID '" << id
            << "' already exists.\n";
        return;
    }
    m_armies.push_back(new ArmyGeneral(id, name));
}

void Game::spawnUnit(const unsigned int &armyId, const string &type,
    const sf::Vector2i &pos)
{
    if (m_mapPtr != 0 && !m_mapPtr->isInsideMap(pos))
        return;
    ArmyGeneral *ptr = getArmy(armyId);
    if (ptr == 0)
    {
        cerr << "Error : no army of identifier '" << armyId << "'.\n";
        return;
    }
    if (isUnitPresent(pos))
        return;
    ptr->addUnit(pos, type, ptr->faction());
}

bool Game::isUnitPresent(const sf::Vector2i &pos)
{
    for (unsigned int i = 0; i < m_armies.size(); i++)
        if (m_armies[i] != 0 && m_armies[i]->getUnitId(pos) != 0) // unit here
            return true;
    return false;
}

ArmyGeneral *Game::getArmy(const unsigned int &armyId)
{
    for (unsigned int i = 0; i < m_armies.size(); i++)
        if (m_armies[i] != 0 && m_armies[i]->id() == armyId)
            return m_armies[i];
    return 0;
}

unsigned int Game::nbOfArmies() const
{
    return m_armies.size();
}

int Game::getGlobalAffector(const string &name)
{
    db::IntCaracteristic *ptr = db::findCaracteristic<int>(name,
        m_globalAffectors);
    if (ptr == 0) // not found
        return 0;
    return ptr->value;
}
void Game::setGlobalAffector(const string &name, const int &value)
{
    db::addCaracteristic<int>(name, value, m_globalAffectors);
}

void Game::renderGame(const float &frametime)
{
    if (App == 0)
        return;
    if (!m_currentGui.empty() && m_GUIs[m_currentGui] != 0)
        m_GUIs[m_currentGui]->render(*App, frametime);
    /*static sf::Shape mask = sf::Shape::Rectangle(sf::FloatRect(0, 0, SCREEN_W, SCREEN_H), sf::Color::Black);
    App->Draw(mask); // to mask sfgui's cursor in game (map) part*/
    if (m_mapPtr != 0)
        m_mapPtr->renderTo(*App);
    for (unsigned int i = 0; i < m_armies.size(); i++)
    {
        if (m_armies[i] != 0)
            drawArmy(*App, *m_armies[i]);
    }
    for (list<p_renderingInfos>::iterator iter = m_renderingList.begin();
        iter != m_renderingList.end(); iter++)
    {
        iter->first.update();
        App->Draw(iter->first);
    }
}

void Game::startDrawingXSprite(XSprite *xsprite, const string &id)
{
    if (xsprite == 0)
        return;
    for (list<p_renderingInfos>::const_iterator iter = m_renderingList.begin();
        iter != m_renderingList.end(); iter++)
    {
        if (iter->second == id)
        {
            cout << "[Game] Warning : xsprite '" << id
                << "' is already in rendering list. Ignoring...\n";
            return;
        }
    }
    m_renderingList.push_back(p_renderingInfos(*xsprite, id));
}

void Game::stopDrawingXSprite(const string &id)
{
    for (list<p_renderingInfos>::iterator iter = m_renderingList.begin();
        iter != m_renderingList.end(); iter++)
    {
        if (iter->second == id)
        {
           m_renderingList.erase(iter);
           return;
        }
    }
}

struct MakeChoiceGui
{
    MakeChoiceGui(const vector<string> &vector, sf::FloatRect &rect) :
        m_gui(sf::FloatRect(rect.Left, rect.Top, rect.Width+10, rect.Height+10)), // otherwise some elements are cut
        m_list(sfg::StringListbox::Create(rect)), m_selected(false),
        m_lastSelected(-1)
    {
        m_gui.LoadSkinFromFile("data/default.skin");
        for (unsigned int i = 0; i < vector.size(); i++)
            if (!vector[i].empty())
                m_list->AddItem(vector[i]);
        m_gui.AddWidget(m_list);
        /*sfg::AlignWidgetInRect(
            *m_list,
            rect,
            sfg::AlignLeft | sfg::AlignTop,
            sf::Vector2f());*/
        m_list->Selected = sfg::Slot<sfg::StringListbox::SelectSlot>(
            &MakeChoiceGui::listItemSelected, this);
    }

    sfg::GUI &getGui() { return m_gui; }

    bool getSelected(int &selected, int &prevSelected)
    {
        if (m_selected)
        {
            prevSelected = selected, selected = m_list->GetSelectedIndex();
            m_selected = false;
            return true;
        }
        return false;
    }

    private:
        void listItemSelected(sfg::Widget::Ptr widget)
        {
            m_selected = true;
        }
        sfg::GUI m_gui;
        sfg::StringListbox::Ptr m_list;
        bool m_selected;
        int m_lastSelected;
};

int Game::getChoiceFromTable(const luabind::object &table,
    const unsigned int &size, sf::FloatRect &rect)
{
    static const unsigned int LIMIT = 25; // table size limit
    if (App == 0 || !table.is_valid() || luabind::type(table) != LUA_TTABLE)
        return -1; // no choice made
    // Converting table
    vector<string> convertedTable;
    try
    {
        for (unsigned int i = 1; i <= size && i <= LIMIT; i++)
        {
            convertedTable.push_back(luabind::object_cast<string>(table[i]));
        }
    }
    catch (const exception &exception)
    {
        cout << "[Game - getChoiceFromVector] Error while converting table : "
            << exception.what() << "\n";
        return -1;
    }
    if (convertedTable.empty())
        return -1;
    // Selection Loop
    bool takeScreen = false, pressedReturn = false;
    int selected = -1, prevSelected = -1;
    MakeChoiceGui gui(convertedTable, rect);
    sf::Image screen;
    screen.CopyScreen(*App);
    sf::Sprite screenSprite(screen);
    while (App->IsOpened())
    {
        sf::Event Event;
        while (App->GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                //App->Close();
                exit(0);
            if (Event.Type == sf::Event::KeyPressed)
            {
                if (Event.Key.Code == sf::Key::Escape)
                    return -1;
                if (Event.Key.Code == sf::Key::F5)
                    takeScreen = true;
                if (Event.Key.Code == sf::Key::Return)
                    pressedReturn = true;
            }
            if (Event.Type == sf::Event::MouseButtonPressed)
            {
                if (Event.MouseButton.Button == sf::Mouse::Right)
                    return -1;
            }
            gui.getGui().HandleEvent(Event);
        }

        if ((gui.getSelected(selected, prevSelected) && selected == prevSelected)
            || pressedReturn)
        {
            if (selected >= 0)
                return selected;
            pressedReturn = false;
        }

        App->Clear();
            App->Draw(screenSprite);
            gui.getGui().Render(*App);
        App->Display();

        if (takeScreen)
        {
            sf::Image screenshot;
            screenshot.CopyScreen(*App);
            if (!screenshot.SaveToFile("screen.jpg"))
                cerr << "Error : cannot save screenshot.\n";
            takeScreen = false;
        }
    }
    return -1;
}
