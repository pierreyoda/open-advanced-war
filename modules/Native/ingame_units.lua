--[[ ingame_units.lua
Part of Native module for Open Advanced War
Manages units in-game. ]]

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