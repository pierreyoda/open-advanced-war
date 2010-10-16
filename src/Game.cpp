#include <SFML/Graphics.hpp>
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

Game::Game() : m_mapPtr(0), m_editorGui()
{

}

Game::~Game()
{
    delete m_mapPtr;
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

    bool error = false;
    CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
        "buildEditorGui", error, &m_editorGui)

    m_mapPtr = new Map(); // for test - loading default map
    m_armies.push_back(new ArmyGeneral(m_armies.size(), "US"));
}

bool Game::saveMap(const string &filename)
{
    if (m_mapPtr == 0)
        return false;
    try
    {
        ofstream file(filename.c_str());
        if (!file)
            throw string("Game - Error : cannot save map into " + filename + ".");
        boost::archive::xml_oarchive archive(file);
        archive << boost::serialization::make_nvp("map", *m_mapPtr);
    }
    catch (const string &error)
    {
        cout << error << "\n";
        return false;
    }
    return true;
}

bool Game::loadMap(const string &filename)
{
    return false;
}

void Game::onMouseOver(const sf::Vector2i &mousePos)
{
    const sf::Vector2i mousePosTiles(GameEntity::pixelsToTiles(mousePos));
    Unit *ptr = 0;
    static Unit *prevUnit = 0;
    for (unsigned int i = 0; i < m_armies.size(); i++)
        if (m_armies[i] != 0)
        {
            Unit *ptr2 = m_armies[i]->getUnitPtr(mousePosTiles);
            if (ptr2 == 0)
                continue;
            ptr = ptr2;
            break;
        }
    static bool luaError =false, luaError2 = false;
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
        if (!m_unit.empty())
            spawnUnit(0, m_unit, mousePosTiles);
        else if (!m_building.empty())
            m_mapPtr->placeBuilding(mousePosTiles, m_building, m_faction, true);
        else
            m_mapPtr->setTile(mousePosTiles, m_tile);
    }
}

void Game::listenEvent(const sf::Event &Event)
{
    if (Event.Type == sf::Event::MouseMoved)
    {
        const int x = Event.MouseMove.X, y = Event.MouseMove.Y;
        onMouseOver(sf::Vector2i(x, y));
    }
    m_editorGui.handleEvent(Event);
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
    ptr->addUnit(pos, type, m_faction);
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
    if (target == 0)
        return;
    m_editorGui.render(*target, frametime);
    /*static sf::Shape mask = sf::Shape::Rectangle(sf::FloatRect(0, 0, SCREEN_W, SCREEN_H), sf::Color::Black);
    target->Draw(mask); // to mask sfgui's cursor in game (map) part*/
    if (m_mapPtr != 0)
        m_mapPtr->renderTo(*target);
    for (unsigned int i = 0; i < m_armies.size(); i++)
    {
        if (m_armies[i] != 0)
            drawArmy(*target, *m_armies[i]);
    }
    for (list<p_renderingInfos>::iterator iter = m_renderingList.begin();
        iter != m_renderingList.end(); iter++)
    {
        iter->first.update();
        target->Draw(iter->first);
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
