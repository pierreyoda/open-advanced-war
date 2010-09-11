--[[ graphic_effects.lua
Part of Native module for Open Advanced War
Manages custom graphic effects. ]]

tileOveredFilter = sf.Color(175, 200, 250, 240)

function tileOvered(tile)
	tile:xsprite():setFilter(tileOveredFilter)
end

--[[ Called when mouse is over a game entity.
entity Reference to the game entity.
]]
function onMouseOverGameEntity(entity)
	if (entity == nil) then
		return
	end
	local class = entity:getClass()
	if (class == TILE) then
		tileOvered(entity)
		return
	elseif (class == BUILDING) then
		print "Over a building!"
	end
end

--[[ Called when mouse is no more over a game entity.
entity Reference to the game entity.
]]
function onMouseNoMoreOverGameEntity(entity)
	if (entity == nil) then
		return
	end
	entity:xsprite():useFilter(false)
end