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
	if (unitDb == nil or tile == nil or tile == "") then
		return false
	end
	local propulsionDb = database:findPropulsion(unitDb:propulsion())
	if (propulsionDb == nil) then
		return false
	end
	return (propulsionDb:canMoveTo(tile))
end

function highlighPossibleMoveTo(pos)
	local map = game:getMapPtr()
	if (map == nil) then
		return
	end
	local unit = findUnit(pos)
	if (unit == nil) then -- unit not found
		return
	end
	local dbUnit = database:findUnit(unit:type())
	if (dbUnit == nil) then
		return
	end
	-- getting move points (speed)
	local move = unit:getIntFeature("move", 0)
	if (move <= 0) then -- if not found, search in database
		move = dbUnit:findIntFeature("move")
		if (move <= 0) then
			return
		end
		unit:setIntFeature("move", dbUnit:findIntFeature("move"))
	end
	-- finding possible cases where unit can move to
	for i = -move, move do
		local absI = math.abs(i)
		for j = -(move-absI), move-absI do
			local pos2 = sf.Vector2i(pos.x + i, pos.y + j)
			if (pos2 ~= pos and not game:isUnitPresent(pos2) and
				canUnitMoveTo(dbUnit, map:getTileType(pos2))) then
				applyEffectOnTile(pos2, sf.Color(255, 230, 255, 128))
			end
		end
	end
end

--[[ Decides if a unit can be placed.
name Unit's type.
pos Unit's position.
]]
function canPlaceUnit(name, pos)
	return (not game:isUnitPresent(pos) and 
		canUnitMoveTo(name, game:getMapPtr():getTileType(pos)))
end