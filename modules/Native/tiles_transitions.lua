--[[ tiles_transitions.lua
Part of Native module for Open Advanced War
Manges tiles transitions in-game. ]]

-- For convenience (shorter)
UNDEFINED, RIGHT, LEFT, UP, DOWN = 
	GameEntity.UNDEFINED,
	GameEntity.RIGHT,
	GameEntity.LEFT,
	GameEntity.UPWARD,
	GameEntity.DOWNWARD
	
-- Returns animation name belong game entity's orientation
function orientationToAnimName(orientation)
	if (orientation == RIGHT) then
		return "base_right"
	elseif (orientation == LEFT) then
		return "base_left"
	elseif (orientation == UP) then
		return "base_upward"
	elseif (orientation == DOWN) then
		return "base_downward"
	else -- Unknown or undefined
		return "base"
	end
end

-- Checks if tile is surrounded by 4 (by4) or 8 (not by4) tiles of "tileType"
function tileSurroundedBy(pos, tileType, map, by4)
	if (by4 == true) then
		return (map:getTileType(pos.x-1, pos.y) == tileType
			and map:getTileType(pos.x+1, pos.y) == tileType
			and map:getTileType(pos.x, pos.y-1) == tileType
			and map:getTileType(pos.x, pos.y+1) == tileType)
	end
	for i = -1, 1, 1 do
		for j = -1, 1, 1 do
			if (map:getTileType(pos.x+i, pos.x+j) ~= tileType) then
				return false
			end
		end
	end
	return true
end

-- From a given orientation returns a relative pos (if unknown returns "pos")
function relativePosition(pos, orientation)
	toReturn = pos
	if (orientation == LEFT) then
		pos.x = pos.x-1
	elseif (orientation == RIGHT) then
		pos.x = pos.x+1
	elseif (orientation == UP) then
		pos.y = pos.y-1
	elseif (orientation == DOWN) then
		pos.y = pos.y+1
	end
	return toReturn
end

-- Called when a tile is oriented (and from lua function onTilePlaced)
function onTileOriented(tile, map, pos)
	--[[if (tile == nil or map == nil) then
		return
	end
	local left, right, up, down = 
		map:getTileOrientation(pos.x-1, pos.y), 
		map:getTileOrientation(pos.x+1, pos.y),
		map:getTileOrientation(pos.x, pos.y-1), 
		map:getTileOrientation(pos.x, pos.y+1)
	tile:playAnim(orientationToAnimName(orientation))
	tile:setOrientation(orientation)]]
	
	-- Here we can manage eventual orientation-dependent tiles (there are not in this module for now)
end

called = false -- global, for test

-- Called when a tile is placed on map (with function  Map::setTile)
function onTilePlaced(tile, map)
	if (not called) then -- test road intersection
		called = true
		map:setTile(5, 5, "Road")
		map:setTile(7, 5, "Road")
		map:setTile(6, 4, "Road")
		map:setTile(6, 6, "Road")
		map:setTile(6, 5, "Road")
	end
	if (tile == nil or map == nil) then
		return
	end
	local pos, toPlay, type = tile:position(), "base", tile:type()
	local left, right, up, down =
			map:getTileType(pos.x-1, pos.y),
			map:getTileType(pos.x+1, pos.y),
			map:getTileType(pos.x, pos.y-1),
			map:getTileType(pos.x, pos.y+1)
	-- Transitions (specific)
	if (type == "Mountain") then
		if (tileSurroundedBy(pos, "Mountain", map, true)) then
			toPlay = "muntain_surrunded_by_4"
		end
	elseif  (type == "Road") then
		if (tileSurroundedBy(pos, "Road", map, true)) then
			toPlay = "base_inter4" -- Intersection of 4
			map:setTileAnim(relativePosition(pos, LEFT), "base_right")
			map:setTileAnim(relativePosition(pos, RIGHT), "base_right")
			map:setTileAnim(relativePosition(pos, UP), "base_upward")
			map:setTileAnim(relativePosition(pos, DOWN), "base_downward")
		elseif (tileSurroundedBy(pos, "Road", map, false)) then
			toPlay = "base_center8" -- Surrounded by 8
		elseif (up == "Road" or down == "Road") then
			toPlay = "base_upward"
		else
			toPlay = "base_right"
		end
	end
	--onTileOriented(tile, map, pos)
	tile:playAnim(toPlay)
end