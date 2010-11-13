--[[ main.lua
Part of AW1 module for Open Advanced War
This is an official module.
AW1 = Advance Wars 1 - Project to "recreate" the first Advance Wars (GBA)
Module's entry in-game. ]]

-- "Constantes" DO NOT MODIFY!
vm:include("shared_globals.lua", 
	"modules/AW1/") -- we cannot use the global MODULE_DIR here...
ICON_PATH = "tank.png" -- Window's icon (will be extracted)
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

-- Includes
vm:include("tools.lua", "modules/")
vm:include("ingame_units.lua;ingame_map.lua;ai.lua;graphic_effects.lua;"
	.. "editor.lua;map_randomizer.lua", MODULE_DIR)
	
gFph:scanDirectory(MODULE_DIR) -- scanning module directory (searching for resources)

-- Main menu
function mainMenu()
	local options = { "Play game", "Launch map editor" }
	local selection = -1
	while (selection <= 0) do -- while no selection
		selection = game:getChoiceFromTable(options, #options, 
			sf.FloatRect(0, 0, SCREEN_W, SCREEN_H))+1
		if (selection == 1) then
			IN_GAME = true
			game:newMap()
			game:loadMap("map.xml")
		elseif (selection == 2) then
			IN_EDITOR = true
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
		local dbUnit = database:findUnit(entity:type())
		entity:setIntCaracteristic("move", dbUnit:findIntCaracteristic("move"))
		entity:setIntCaracteristic("fuel", dbUnit:findIntCaracteristic("fuel"))
	elseif (class == BUILDING and map ~= nil) then
		onBuildingPlaced(entity, map)
	end
end

-- Event listener
function onEvent(event)
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