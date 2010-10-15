--[[ ingame_units.lua
Part of Native module for Open Advanced War
Manages units in-game. ]]

-- Return the "movement currency" (move points) of a given unit.
function getUnitMovePoints(unit)
	return (unit:getIntCaracteristic("move") -- we stocked the value on unit creation (onGameEntityPlaced)
		+ game:getGlobalAffector("move")
		--+ game:getSpecificAffector("move", unit:owner(), unit:type())
		)	
end

--[[ Returns if a unit can move on a given tile.
unitDb Const pointer to database unit.
tile Tile's name.
]]
function canUnitMoveTo(unitDb, tile)
	if (unitDb == nil) then
		return false
	end
	local propulsionDb = database:findPropulsion(unitDb:propulsion())
	print (unitDb:propulsion())
	if (propulsionDb == nil) then
		return false
	end
	return (propulsionDb:canMoveTo(tile))
end

--[[ Decides if a unit can be placed.
name Unit's type.
pos Unit's position.
]]
function canPlaceUnit(name, pos)
	return (not game:isUnitPresent(pos) and 
		canUnitMoveTo(name, game:getMapPtr():getTileType(pos)))
end