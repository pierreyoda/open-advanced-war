--[[ graphic_effects.lua
Part of Native module for Open Advanced War
Manages custom graphic effects. ]]

tileOveredFilter, buildingOveredFilter = sf.Color(230, 255, 255, 228), sf.Color(191, 255, 191, 150)
playerUnitOveredFilter = sf.Color(130, 105, 255, 170)
cursorFrame = db.Frame(0, 0, 32, 32, 1)
cursorAnim = db.Anim("base", "cursor.png") cursorAnim:addFrame(cursorFrame)
cursor = XSprite() cursor:playAnim(cursorAnim)

function tileOvered(tile)
	tile:xsprite():setFilter(tileOveredFilter)
end

function buildingOvered(building)
	building:xsprite():setFilter(buildingOveredFilter)
end

function unitOvered(unit)
	--if (game:isCurrentPlayer(unit:owner()) then -- is a unit of the current player
		unit:xsprite():setFilter(playerUnitOveredFilter)
	--else -- is an enemy unit
		--unit:xsprite():setFilter(enemyUnitOveredFilter)
	--end
end

function drawCursorHere(position)
	game:stopDrawingXSprite("cursor") -- clearing
	cursor:playAnim(cursorAnim) -- reset of animation
	cursor:SetPosition(GameEntity.tilesToPixels(position)) -- setting position (in tiles)
	game:startDrawingXSprite(cursor, "cursor") -- adding cursor to rendering list
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
		drawCursorHere(entity:position())
		return
	elseif (class == BUILDING) then
		buildingOvered(entity)
		drawCursorHere(entity:position())
	elseif (class == UNIT) then
		unitOvered(entity)
		drawCursorHere(entity:position())
	end
end

--[[ Called when mouse is no more over a game entity.
entity Reference to the game entity.
]]
function onMouseNoMoreOverGameEntity(entity)
	if (entity == nil) then
		return
	end
	game:stopDrawingXSprite("cursor") -- stop rendering cursor (if rendered)
	if (entity:xspriteConst():isFilterUsed()) then -- if a filter is applied...
		entity:xsprite():useFilter(false) -- ...stop applying it
	end
end