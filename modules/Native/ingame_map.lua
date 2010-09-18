--[[ tiles_transitions.lua
Part of Native module for Open Advanced War
Manges tiles transitions in-game. ]]

--[[ Decides if a building can be placed.
name Building's type.
pos Building's position.
map Reference to map.
force If true tile will be set to "Plain" and building allowed.
]]
function canPlaceBuilding(name, pos, map, force)
	if (not map:isInsideMap(pos)) then
		return false
	end
	local ok = true
	if (force) then
		map:setTile(pos, "Plain")
	elseif (map:getTileType(pos) ~= "Plain") then
		ok = false
	end
	return ok
end

-- For convenience (shorter)
UNDEFINED, RIGHT, LEFT, UP, DOWN = 
	GameEntity.UNDEFINED,
	GameEntity.RIGHT,
	GameEntity.LEFT,
	GameEntity.UPWARD,
	GameEntity.DOWNWARD

function orientationToAnimName(orientation) -- Surcharge
	return orientationToAnimName(orientation, false)
end
--[[ Returns animation name belong game entity's orientation.
Param equivalence : does (left and right) and (up and down) correspond to the same images? ]]
function orientationToAnimName(orientation, equivalence)
	if (orientation == RIGHT or (equivalence and orientation == LEFT)) then
		return "base_right"
	elseif (orientation == LEFT) then
		return "base_left"
	elseif (orientation == UP or (equivalence and orientation == DOWN)) then
		return "base_upward"
	elseif (orientation == DOWN) then
		return "base_downward"
	else -- Unknown or undefined
		return "base"
	end
end

-- From a given orientation returns a relative pos (if unknown returns "pos")
function relativePosition(pos, orientation)
	toReturn = sf.Vector2i(pos) -- copy (and not reference!)
	if (orientation == LEFT) then
		toReturn.x = toReturn.x-1
	elseif (orientation == RIGHT) then
		toReturn.x = toReturn.x+1
	elseif (orientation == UP) then
		toReturn.y = toReturn.y-1
	elseif (orientation == DOWN) then
		toReturn.y = toReturn.y+1
	end
	return toReturn
end

-- Checks if tile is surrounded by 4 (by4) or 8 (not by4) tiles of "tileType"
function tileSurroundedBy(pos, tileType, map, by4)
	if (tileType == "Road") then
		return (map:getTileType(relativePosition(pos, LEFT)) == tileType
			and map:getTileType(relativePosition(pos, RIGHT)) == tileType
			and map:getTileType(relativePosition(pos, UP)) == tileType
			and map:getTileType(relativePosition(pos, DOWN)) == tileType)
	end
	for i = -1, 1, 1 do
		for j = -1, 1, 1 do
			local npos = sf.Vector2i(pos.x+i, pos.y+j)
			if (not npos == pos and map:getTileType(npos) ~= tileType) then
				return false
			end
		end
	end
	return true
end

-- Called when a tile is oriented (and from lua function onTilePlaced)
function onTileOriented(tile, map, pos)
	--[[ Here we can do some stuff about eventual orientation-dependent tiles 
(there are not in this module for now). ]]
end

called = false -- global, for test

-- Called when a tile is placed on map (with function  Map::setTile)
function onTilePlaced(tile, map)
	if (not called) then -- test road intersection
		called = true
		for i = 0, 25, 1 do
			map:setTile(i, 2, "Forest")
		end
		map:setTile(3, 1, "Road");
		map:setTile(5, 5, "Road")
		map:setTile(7, 5, "Road")
		map:setTile(6, 4, "Road")
		map:setTile(6, 6, "Road")
		map:setTile(6, 5, "Road")
		game:spawnUnit(0, "Tank", sf.Vector2i(15, 10))
		game:spawnUnit(0, "HMMWV", sf.Vector2i(20, 10))
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
			toPlay = "base_highdensity"
		end
	elseif (type == "Forest") then
		if (tileSurroundedBy(pos, "Mountain", map, true)) then
			toPlay = "base_highdensity"
		end
	elseif  (type == "Road") then
		if (tileSurroundedBy(pos, "Road", map, true)) then
			toPlay = "base_inter4" -- Intersection of 4
			map:setTileAnim(relativePosition(pos, LEFT), "base_right")
			map:setTileAnim(relativePosition(pos, RIGHT), "base_right")
			map:setTileAnim(relativePosition(pos, UP), "base_upward")
			map:setTileAnim(relativePosition(pos, DOWN), "base_upward")
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