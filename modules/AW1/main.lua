--[[ main.lua
Part of AW1 module for Open Advanced War
This is an official module.
AW1 = Advance Wars 1 - Project to "recreate" the first Advance Wars (GBA)
Module's entry in-game. ]]

-- "Constantes" DO NOT MODIFY!
MODULE_NAME, MODULE_DIR = "AW1", "modules/AW1/"
DATABASE_PATH = --[[MODULE_DIR .. (later)]] "aw1_db.xml" -- Database's path (will be extracted)
ICON_PATH = "tank.png" -- Window's icon (will be extracted)
-- /"Constantes"

-- For convenience (shorter)
NONE, TILE, BUILDING, UNIT = 
	GameEntity.NONE,
	GameEntity.TILE,
	GameEntity.BUILDING,
	GameEntity.UNIT

vm:include("graphic_effects.lua;ai.lua;ingame_units.lua", 
	"modules/Native/") -- we don't need to reimplement all...
vm:include("ingame_map.lua;editor.lua", MODULE_DIR)
	
gFph:scanDirectory(MODULE_DIR) -- scanning module directory (searching for resources)

-- Called when a GameEntity is placed on map.
function onGameEntityPlaced(entity)
	local class = entity:getClass()
	if (class == TILE and game:getMapPtr() ~= nil) then
		onTilePlaced(entity, game:getMapPtr())
	elseif (class == UNIT) then
		entity:playAnim("base_right")
		local dbUnit = database:findUnit(entity:type())
		entity:setIntCaracteristic("speed", dbUnit:findIntCaracteristic("speed"))
	end
end