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

vm:include("ingame_units.lua;ingame_map.lua;ai.lua;graphic_effects.lua;editor.lua", MODULE_DIR)
	
gFph:scanDirectory(MODULE_DIR) -- scanning module directory (searching for resources)

-- Called when a GameEntity is placed on map.
function onGameEntityPlaced(entity)
	local class = entity:getClass()
	if (class == TILE and game:getMapPtr() ~= nil) then
		onTilePlaced(entity, game:getMapPtr())
	elseif (class == UNIT) then
		entity:playAnim("base_right")
		local dbUnit = database:findUnit(entity:type())
		entity:setIntCaracteristic("move", dbUnit:findIntCaracteristic("move"))
		entity:setIntCaracteristic("fuel", dbUnit:findIntCaracteristic("fuel"))
	end
end