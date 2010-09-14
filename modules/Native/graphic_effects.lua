--[[ graphic_effects.lua
Part of Native module for Open Advanced War
Manages custom graphic effects. ]]

tileOveredFilter = sf.Color(175, 200, 250, 240)
cursorFrame = db.Frame(0, 0, 32, 32, 1)
cursorAnim = db.Anim("base", "cursor.png") cursorAnim:addFrame(cursorFrame)
cursor = XSprite() cursor:playAnim(cursorAnim)

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
		--game:drawXSprite(cursor)
		return
	elseif (class == BUILDING) then
		print "Over a building!"
		entity:xsprite():setFilter(sf.Color(55, 55, 55))
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