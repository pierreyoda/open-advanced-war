--[[ main.lua
Part of Native module for Open Advanced War
Module's entry in-game. ]]

-- "Constantes" DO NOT MODIFY!
MODULE_NAME, MODULE_DIR = "Native", "modules/Native"
DATABASE_PATH = "database.xml"
ICON_PATH = "tank.png" -- Window's icon (will be extracted)
-- /"Constantes"

-- For convenience (shorter)
NONE, TILE, BUILDING, UNIT = 
	GameEntity.NONE,
	GameEntity.TILE,
	GameEntity.BUILDING,
	GameEntity.UNIT

vm:include("ingame_map.lua;graphic_effects.lua;ai.lua", MODULE_DIR)

-- Return the speed of a given unit.
function getUnitSpeed(unit)
	return (unit:getIntCaracteristic("speed") 
		+ game:getGlobalAffector("speed")
		--+ game:getSpecificAffector("speed", unit:owner(), unit:type())
		)
end

--[[ Decides if a unit can be placed.
name Unit's type.
pos Unit's position.
]]
function canPlaceUnit(name, pos)
	return (not game:isUnitPresent(pos))
end

-- Called when a GameEntity is placed on map.
function onGameEntityPlaced(entity)
	local class = entity:getClass()
	if (class == TILE and game:getMapPtr() ~= nil) then
		onTilePlaced(entity, game:getMapPtr())
	elseif (class == UNIT) then
		entity:playAnim("base_right")
		local dbUnit = database:findUnit(entity:type())
		entity:setIntCaracteristic("speed", dbUnit:findIntCaracteristic("speed"))
		print "unit spawned!"
	end
end