--[[ main.lua
Part of AW1 module for Open Advanced War
This is an official module.
AW1 = Advance Wars 1 - Project to "recreate" the first Advance Wars (GBA)
Module's entry in-game. ]]

-- "Constantes" DO NOT MODIFY!
vm:include("shared_globals.lua", 
	"modules/AW1/") -- we cannot use the global MODULE_DIR here...
gFph:scanDirectory(MODULE_DIR) -- scanning module directory (searching for resources)
ICON_PATH = gFph:getFilepath("icon.png") -- Window's icon (will be extracted)
-- /"Constantes"

-- For convenience (shorter)
NONE, TILE, BUILDING, UNIT = 
	GameEntity.NONE,
	GameEntity.TILE,
	GameEntity.BUILDING,
	GameEntity.UNIT
	
-- Game variables
editor_toPlaceType, editor_toPlace, editor_toPlaceFaction = NONE, "", ""
IN_GAME, IN_EDITOR = false, false

function setUnitInfos(unit, replaceIfNeeded)
	local replace = (replaceIfNeeded == nil or replaceIfNeeded)
	dbUnit = database:findUnit(unit:type())
	if (dbUnit == nil) then
		return
	end
	if (replace or unit:getIntFeature("move", -1) == -1) then
		unit:setIntFeature("move", dbUnit:findIntFeature("move"))
	end
	if (replace or unit:getIntFeature("fuel", -1) == -1) then
		unit:setIntFeature("fuel", dbUnit:findIntFeature("fuel"))
	end
end
function findUnit(pos)
	for i = 0, 3 do -- in all armies
		local army = game:getArmy(i)
		if (army ~= nil) then
			local unitPtr = army:getUnitPtr(pos)
			if (unitPtr ~= nil) then
				return unitPtr
			end
		end
	end
	return nil
end

-- Includes
vm:include("tools.lua", "modules/")
vm:include("ingame_units.lua;ingame_map.lua;ai.lua;graphic_effects.lua;"
	.. "editor.lua;game.lua;map_randomizer.lua", MODULE_DIR)

-- Main menu
function mainMenu()
	local options = { "Play game", "Launch map editor" }
	local selection = -1
	while (selection <= 0) do -- while no selection
		selection = game:getChoiceFromTable(options, #options, 
			sf.FloatRect(0, 0, SCREEN_W, SCREEN_H))+1
		if (selection == 1) then
			IN_GAME = true
			game:addGui("GameGui")
			game:newMap()
			game:loadMap("map.xml")
		elseif (selection == 2) then
			IN_EDITOR = true
			game:addGui("EditorGui")
			game:newMap()
			randomMap(game:getMapPtr())
			return
		end
	end
end

-- Called when a GameEntity is placed on map.
function onGameEntityPlaced(entity)
	local class = entity:getClass()
	local map = game:getMapPtr()
	if (class == TILE and map ~= nil) then
		onTilePlaced(entity, map)
	elseif (class == UNIT) then
		entity:playAnim("base_right")
		setUnitInfos(entity)
	elseif (class == BUILDING and map ~= nil) then
		onBuildingPlaced(entity, map)
	end
end

-- Event listener
function onEvent(event)
	if (IN_GAME and not IN_EDITOR and game:getMapPtr() ~= nil) then
		if (event.Type == sf.Event.MousePressed
			and event.MouseButton.Button == sf.Mouse.Left) then
			if (event.MouseButton.Y <= GUI_START_H) then -- not in GUI space
				local pos = GameEntity.pixelsToTiles(
					sf.Vector2i(event.MouseButton.X, event.MouseButton.Y))
				if (game:isUnitPresent(pos)) then -- unit present
					highlighPossibleMoveTo(pos)
				elseif (game:getMapPtr():getBuildingType(pos) ~= "") then -- building present
					makeBuildChoice(pos)
				end
			end
		end
	end
	if (event.Type == sf.Event.KeyPressed and event.Key.Code == sf.Key.Escape) then
		print "Goodbye!"
	end
end

local prevPos = nullPos
-- Input listener
function listenInput(input)
	if (IN_EDITOR and not IN_GAME) then
		if (input:IsMouseButtonDown(sf.Mouse.Left)) then
			if (input:GetMouseY() <= GUI_START_H) then -- not in GUI space
				local pos = GameEntity.pixelsToTiles(sf.Vector2i(input:GetMouseX(),
					input:GetMouseY()))
				if (pos ~= prevPos) then
					editor_place(pos)
					prevPos = pos
				end
			end
		end
	end
end