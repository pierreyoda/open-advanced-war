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

-- Called when a tile is oriented (and from lua function onTilePlaced)
function onTileOriented(tile, map, pos)
	--[[ Here we can do some stuff about eventual orientation-dependent tiles 
(there are not in this module for now). ]]
end

function isTileOfGivenType(type_, pos, map)
	if (type_ == "") then
		return false
	end
	return (map:getTileType(pos) == type_)
end

--[[ [EXTERNAL] NB : method from project OpenAWars, under GPL licence.
* See : http://code.google.com/p/openawars/
--]]
function checkCoherencyForRoad(pos, map)
	local roadAround = 0 -- number of road tiles around
	local verticalRoad, horizontalRoad = false, false -- is vertical/horizontal road
	local onLeft, onRight, onUp, onDown = false, false, false, false
	-- Checking around cases
	if (isTileOfGivenType("Road", relativePosition(pos, LEFT), map)) then
		roadAround = roadAround+1
		horizontalRoad, onLeft = true, true
	end	
	if (isTileOfGivenType("Road", relativePosition(pos, RIGHT), map)) then
		roadAround = roadAround+1
		horizontalRoad, onRight = true, true
	end	
	if (isTileOfGivenType("Road", relativePosition(pos, UP), map)) then
		roadAround = roadAround+1
		verticalRoad, onUp = true, true
	end
	if (isTileOfGivenType("Road", relativePosition(pos, DOWN), map)) then
		roadAround = roadAround+1
		verticalRoad, onDown = true, true
	end
	local anim = "base_right"
	-- Image choice
	if (roadAround == 0) then
		anim = "base_right"
	elseif (roadAround ==1) then
		if (verticalRoad) then
			anim = "base_up"
		elseif (horizontalRoad) then
			anim = "base_right"
		end
	elseif (roadAround == 2) then -- line or corner
		-- Line case:
		if (verticalRoad and not horizontalRoad) then
			anim = "base_up" -- horizontal
		end
		if (horizontalRoad and not verticalRoad) then
			anim = "base_right" -- vertical
		end
		-- turn
		if (onLeft and onUp) then
			anim = "base_left_up"
		end
		if (onRight and onUp) then
			anim = "base_right_up"
		end
		if (onLeft and onDown) then
			anim = "base_left_down"
		end
		if (onRight and onDown) then
			anim = "base_right_down"
		end
	elseif (roadAround == 3) then -- T intersection ("inter_3")
		if (onLeft and onRight and onUp) then
			anim = "base_inter3_up"
		end
		if (onLeft and onRight and onDown) then
			anim = "base_inter3_down"
		end
		if (onUp and onDown and onLeft) then
			anim = "base_inter3_left"
		end
		if (onUp and onDown and onRight) then
			anim = "base_inter3_right"
		end
	elseif (roadAround == 4) then
		anim = "base_inter4"
	end
	map:setTileAnim(pos, anim)
end

function checkCoherencyForRiver(pos, map)
	local roadAround = 0 -- number of river tiles around
	local verticalRoad, horizontalRoad = false, false -- is vertical/horizontal river
	local onLeft, onRight, onUp, onDown = false, false, false, false
	-- Checking around cases
	if (isTileOfGivenType("River", relativePosition(pos, LEFT), map)) then
		roadAround = roadAround+1
		horizontalRoad, onLeft = true, true
	end	
	if (isTileOfGivenType("River", relativePosition(pos, RIGHT), map)) then
		roadAround = roadAround+1
		horizontalRoad, onRight = true, true
	end	
	if (isTileOfGivenType("River", relativePosition(pos, UP), map)) then
		roadAround = roadAround+1
		verticalRoad, onUp = true, true
	end
	if (isTileOfGivenType("River", relativePosition(pos, DOWN), map)) then
		roadAround = roadAround+1
		verticalRoad, onDown = true, true
	end
	local anim = "base_right"
	-- Image choice
	if (roadAround == 0) then
		anim = "base_right"
	elseif (roadAround ==1) then
		if (verticalRoad) then
			anim = "base_up"
		elseif (horizontalRoad) then
			anim = "base_right"
		end
	elseif (roadAround == 2) then -- line or corner
		-- Line case:
		if (verticalRoad and not horizontalRoad) then
			anim = "base_up" -- horizontal
		end
		if (horizontalRoad and not verticalRoad) then
			anim = "base_right" -- vertical
		end
		-- turn
		if (onLeft and onUp) then
			anim = "base_left_up"
		end
		if (onRight and onUp) then
			anim = "base_right_up"
		end
		if (onLeft and onDown) then
			anim = "base_left_down"
		end
		if (onRight and onDown) then
			anim = "base_right_down"
		end
	elseif (roadAround == 3) then -- T intersection ("inter_3")
		if (onLeft and onRight and onUp) then
			anim = "base_inter3_up"
		end
		if (onLeft and onRight and onDown) then
			anim = "base_inter3_down"
		end
		if (onUp and onDown and onLeft) then
			anim = "base_inter3_left"
		end
		if (onUp and onDown and onRight) then
			anim = "base_inter3_right"
		end
	elseif (roadAround == 4) then
		anim = "base_inter4"
	end
	map:setTileAnim(pos, anim)
end

function checkCoherency(pos, map)
	local type_ = map:getTileType(pos)
	if (type_ == "") then
		return
	end
	if (type_ == "Road") then
		return checkCoherencyForRoad(pos, map)
	elseif (type_ == "River") then
		return checkCoherencyForRiver(pos, map)
	end
	return ""
end

function checkCoherencyAround(pos, map)
	checkCoherency(relativePosition(pos, LEFT), map)
	checkCoherency(relativePosition(pos, RIGHT), map)
	checkCoherency(relativePosition(pos, UP), map)
	checkCoherency(relativePosition(pos, DOWN), map)
	-- Diagonals
	checkCoherency(sf.Vector2i(pos.x-1,pos.y+1), map); -- Down Left
	checkCoherency(sf.Vector2i(pos.x+1,pos.y+1), map); -- Down Right
	checkCoherency(sf.Vector2i(pos.x-1,pos.y-1), map); -- Up Left
	checkCoherency(sf.Vector2i(pos.x+1,pos.y-1), map); -- Up Right
end
-- [/EXTERNAL]

called = false -- global, for test

function canPlaceRiver(pos, map)
	if (map:getTileType(pos) == "Sea") then
		return false
	end
	-- Top left corner
	if (isTileOfGivenType("River", relativePosition(pos, LEFT), map)
		and isTileOfGivenType("River", relativePosition(pos, UP), map)
		and isTileOfGivenType("River", sf.Vector2i(pos.x-1, pos.y-1), map)) then
		return false
	end	
	-- Top right corner
	if (isTileOfGivenType("River", relativePosition(pos, RIGHT), map)
		and isTileOfGivenType("River", relativePosition(pos, UP), map)
		and isTileOfGivenType("River", sf.Vector2i(pos.x+1, pos.y-1), map)) then
		return false
	end	
	-- Bottom left corner
	if (isTileOfGivenType("River", relativePosition(pos, LEFT), map)
		and isTileOfGivenType("River", relativePosition(pos, DOWN), map)
		and isTileOfGivenType("River", sf.Vector2i(pos.x-1, pos.y+1), map)) then
		return false
	end	
	-- Bottom right corner
	if (isTileOfGivenType("River", relativePosition(pos, RIGHT), map)
		and isTileOfGivenType("River", relativePosition(pos, DOWN), map)
		and isTileOfGivenType("River", sf.Vector2i(pos.x+1, pos.y+1), map)) then
		return false
	end
	return true
end

--[[ Decides if a tile can be placed or not.
type_ Tile's type.
pos Tile's position.
]]
function canPlaceTile(type_, pos, map)
	if (type_ == "River") then
		return canPlaceRiver(pos, map)
	end
	return true
end

-- Called when a tile is placed on map (function  Map::setTile)
function onTilePlaced(tile, map)
	if (tile == nil or map == nil) then
		return
	end
	if (not called) then -- test road intersection
		called = true
		for i = 0, 25, 1 do
			map:setTile(i, 2, "Forest")
		end
		game:spawnUnit(0, "Tank", sf.Vector2i(15, 10))
		game:spawnUnit(0, "HMMWV", sf.Vector2i(20, 10))
	end
	-- Transitions (specific)
	local pos = tile:position()
	map:removeBuilding(pos)
	checkCoherencyAround(pos, map)
	checkCoherency(pos, map)
end

-- Called when a building is placed on map (function  Map::placeBuilding)
function onBuildingPlaced(building, map)
	if (building == nil or map == nil) then
		return
	end
	map:setTile(building:position(), "Plain")
end

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
	if (not force and map:getTileType(pos) ~= "Plain") then
		return false
	end
	return not map:isBuildingPresent(pos)
end