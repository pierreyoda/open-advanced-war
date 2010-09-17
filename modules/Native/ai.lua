--[[ ai.lua
Part of Native module for Open Advanced War
Manges Artificial Intelligence (ennemy computer). ]]

--[[ Determines if a is in the range of another one.
attacker Attacker.
target Target.
--]]
function isUnitWithinRange(attacker, target)
	local range = attacker:getCaracteristic(range)
	if (range <= 0) then -- not found or invalid
		return false
	end
	return true
end

--[[ Called to determine if a unit can attack another one.
unit Attacking unit.
target Requested target.
]]
function canAttackUnit(unit, target)
	if (not unit:getBoolCaracteristic("attackAfterMove")) then
		return false
	end
	return isUnitWithinRange(unit, target)
end

--[[ Decides if a unit can be placed.
name Unit's type.
pos Unit's position.
]]
function canPlaceUnit(name, pos)
	return (not game:isUnitPresent(pos))
end