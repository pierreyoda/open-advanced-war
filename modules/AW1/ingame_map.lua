--[[ tiles_transitions.lua
Part of AW1 module for Open Advanced War
Manges tiles transitions in-game. ]]

-- For convenience (shorter)
UNDEFINED, RIGHT, LEFT, UP, DOWN, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT = 
	GameEntity.UNDEFINED,
	GameEntity.RIGHT,
	GameEntity.LEFT,
	GameEntity.UPWARD,
	GameEntity.DOWNWARD,
	5, 6, 7, 8

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
	elseif (orientation == UPLEFT) then
		toReturn.x, toReturn.y = toReturn.x-1, toReturn.y-1
	elseif (orientation == UPRIGHT) then
		toReturn.x, toReturn.y = toReturn.x+1, toReturn.y-1
	elseif (orientation == DOWNLEFT) then
		toReturn.x, toReturn.y = toReturn.x-1, toReturn.y+1
	elseif (orientation == DOWNRIGHT) then
		toReturn.x, toReturn.y = toReturn.x+1, toReturn.y+1
	end
	return toReturn
end

-- Called when a tile is oriented (and from lua function onTilePlaced)
function onTileOriented(tile, map, pos)
	--[[ Here we can do some stuff about eventual orientation-dependent tiles 
(there are not in this module for now). ]]
end

function isTileOfGivenType(type_, pos, map, building)
	return (map:getTileType(pos) == type_ or 
		(building ~= nil and building ~= "" 
		and map:getBuildingType(pos) == building))
end

--[[ [EXTERNAL] NB : method from project OpenAWars, under GPL licence.
* See : http://code.google.com/p/openawars/
--]]
function checkCoherencyForRoad(pos, map)
	local roadAround = 0 -- number of road tiles around
	local verticalRoad, horizontalRoad = false, false -- is vertical/horizontal road
	local onLeft, onRight, onUp, onDown = false, false, false, false
	-- Checking around cases
	if (isTileOfGivenType("Road", relativePosition(pos, LEFT), map, "HQ")) then
		roadAround = roadAround+1
		horizontalRoad, onLeft = true, true
	end	
	if (isTileOfGivenType("Road", relativePosition(pos, RIGHT), map, "HQ")) then
		roadAround = roadAround+1
		horizontalRoad, onRight = true, true
	end	
	if (isTileOfGivenType("Road", relativePosition(pos, UP), map, "HQ")) then
		roadAround = roadAround+1
		verticalRoad, onUp = true, true
	end
	if (isTileOfGivenType("Road", relativePosition(pos, DOWN), map, "HQ")) then
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
	local riverAround = 0 -- number of river tiles around
	local verticalRoad, horizontalRoad = false, false -- is vertical/horizontal river
	local onLeft, onRight, onUp, onDown = false, false, false, false
	-- Checking around cases
	if (isTileOfGivenType("River", relativePosition(pos, LEFT), map)) then
		riverAround = riverAround+1
		horizontalRoad, onLeft = true, true
	end	
	if (isTileOfGivenType("River", relativePosition(pos, RIGHT), map)) then
		riverAround = riverAround+1
		horizontalRoad, onRight = true, true
	end	
	if (isTileOfGivenType("River", relativePosition(pos, UP), map)) then
		riverAround = riverAround+1
		verticalRoad, onUp = true, true
	end
	if (isTileOfGivenType("River", relativePosition(pos, DOWN), map)) then
		riverAround = riverAround+1
		verticalRoad, onDown = true, true
	end
	local anim = "base_right"
	-- Image choice
	if (riverAround == 0) then
		anim = "base_right"
	elseif (riverAround ==1) then
		if (verticalRoad) then
			anim = "base_up"
		elseif (horizontalRoad) then
			anim = "base_right"
		end
	elseif (riverAround == 2) then -- line or corner
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
	elseif (riverAround == 3) then -- T intersection ("inter_3")
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
	elseif (riverAround == 4) then
		anim = "base_inter4"
	end
	map:setTileAnim(pos, anim)
end

function checkCoherencyForSea(pos, map)
	local nbSeaAround, nbSeaDiagonal = 0, 0
	local verticalSea, horizontalSea = false, false
	local onLeft, onRight, onUp, onDown = false, false, false, false
	local onUpLeft, onUpRight, onDownLeft, onDownRight = false, false, false, false
	local beachOnLeft, beachOnRight, beachOnUp, beachOnDown = false, false, false, false
	
	local function seaOrBridge(pos)
		return (isTileOfGivenType("Sea", pos, map) 
			or map:getBuildingType(pos) == "Bridge"
			or isTileOfGivenType("", pos, map)) -- null tiles (out of the map)
	end
	
	-- Checking all adjacent tiles
	if (seaOrBridge(relativePosition(pos, LEFT))) then
		nbSeaAround = nbSeaAround+1
		horizontalSea, onLeft = true, true
		if (isTileOfGivenType("Beach", relativePosition(pos, LEFT), map)) then
			beachOnLeft = true
		end
	end
	if (seaOrBridge(relativePosition(pos, RIGHT))) then
		nbSeaAround = nbSeaAround+1
		horizontalSea, onRight = true, true
		if (isTileOfGivenType("Beach", relativePosition(pos, RIGHT), map)) then
			beachOnRight = true
		end
	end
	if (seaOrBridge(relativePosition(pos, UP))) then
		nbSeaAround = nbSeaAround+1
		verticalSea, onUp = true, true
		if (isTileOfGivenType("Beach", relativePosition(pos, UP), map)) then
			beachOnUp = true
		end
	end
	if (seaOrBridge(relativePosition(pos, DOWN))) then
		nbSeaAround = nbSeaAround+1
		verticalSea, onDown = true, true
		if (isTileOfGivenType("Beach", relativePosition(pos, DOWN), map)) then
			beachOnDown = true
		end
	end
	if (seaOrBridge(relativePosition(pos, UPLEFT))) then
		nbSeaDiagonal = nbSeaDiagonal+1
		onUpLeft = true
	end	
	if (seaOrBridge(relativePosition(pos, UPRIGHT))) then
		nbSeaDiagonal = nbSeaDiagonal+1
		onUpRight = true
	end	
	if (seaOrBridge(relativePosition(pos, DOWNLEFT))) then
		nbSeaDiagonal = nbSeaDiagonal+1
		onDownLeft = true
	end	
	if (seaOrBridge(relativePosition(pos, DOWNRIGHT))) then
		nbSeaDiagonal = nbSeaDiagonal+1
		onDownRight = true
	end
	
	-- Image choice
	local anim = "base_deep"
	if (nbSeaAround == 0) then
		anim = "base_alone"
	elseif (nbSeaAround == 1) then
		if (onLeft) then
			anim = "base_end_left"
		elseif (onRight) then
			anim = "base_end_right"
		elseif (onUp) then
			anim = "base_end_up"
		elseif (onDown) then
			anim = "base_end_down"
		end
	elseif (nbSeaAround == 2) then -- line or corner
		-- line case
			if (verticalSea and not horizontalSea) then
				anim = "base_up"
			elseif (horizontalSea and not verticalSea) then
				anim = "base_right"
			end
		-- corner case
		if (onLeft and onUp) then
			anim = "base_left_up"
			if (onUpLeft) then -- coast
				anim = anim .. "_deep"
			end
		end		
		if (onRight and onUp) then
			anim = "base_right_up"
			if (onUpRight) then -- coast
				anim = anim .. "_deep"
			end
		end		
		if (onLeft and onDown) then
			anim = "base_left_down"
			if (onDownLeft) then -- coast
				anim = anim .. "_deep"
			end
		end		
		if (onRight and onDown) then
			anim = "base_right_down"
			if (onDownRight) then -- coast
				anim = anim .. "_deep"
			end
		end
	elseif (nbSeaAround == 3) then -- T intersection or corner
		anim = "base_inter3_down"
		if (onLeft and onRight and onUp) then
			anim = "base_inter3_up"
			if (onUpLeft or onUpRight) then
				anim = anim .. "_deep"
			end
		end
		if (onLeft and onRight and onDown) then
			anim = "base_inter3_down"
			if (onDownLeft or onDownRight) then
				anim = anim .. "_deep"
			end
		end
		if (onUp and onDown and onLeft) then
			anim = "base_inter3_left"
			if (onUpLeft or onDownLeft) then
				anim = anim .. "_deep"
			end
		end
		if (onUp and onDown and onRight) then
			anim = "base_inter3_right"
			if (onUpRight or onDownRight) then
				anim = anim .. "_deep"
			end
		end
	elseif (nbSeaAround ==4) then -- 4 intersection
		anim = "base_inter4"
		if (nbSeaDiagonal >= 1) then
			anim = "base_deep"
		end
	end
	
	map:setTileAnim(pos, anim)
end

function checkCoherencyForMountain(pos, map)
	map:setTileAnim(pos, "base_high")
end
function checkCoherencyForPlain(pos, map)
	if (isTileOfGivenType("Mountain", relativePosition(pos, LEFT), map)) then -- if mountain on left
		map:setTileAnim(pos, "base_shady")
	else
		map:setTileAnim(pos, "base")
	end
end

function checkCoherency(pos, map)
	local type_ = map:getTileType(pos)
	if (type_ == "") then
		return
	elseif (type_ == "Plain") then
		return checkCoherencyForPlain(pos, map)
	elseif (type_ == "Road") then
		return checkCoherencyForRoad(pos, map)
	elseif (type_ == "River") then
		return checkCoherencyForRiver(pos, map)
	elseif (type_ == "Sea") then
		return checkCoherencyForSea(pos, map)
	elseif (type_ == "Beach") then
		return checkCoherencyForBeach(pos, map)
	elseif (type_ == "Mountain") then
		return checkCoherencyForMountain(pos, map)
	end
end

function checkCoherencyAround(pos, map)
	checkCoherency(relativePosition(pos, LEFT), map)
	checkCoherency(relativePosition(pos, RIGHT), map)
	checkCoherency(relativePosition(pos, UP), map)
	checkCoherency(relativePosition(pos, DOWN), map)
	-- Diagonals
	checkCoherency(relativePosition(pos, UPLEFT), map);
	checkCoherency(relativePosition(pos, UPRIGHT), map);
	checkCoherency(relativePosition(pos, DOWNLEFT), map);
	checkCoherency(relativePosition(pos, DOWNRIGHT), map);
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
		and isTileOfGivenType("River", relativePosition(pos, UPLEFT), map)) then
		return false
	end	
	-- Top right corner
	if (isTileOfGivenType("River", relativePosition(pos, RIGHT), map)
		and isTileOfGivenType("River", relativePosition(pos, UP), map)
		and isTileOfGivenType("River", relativePosition(pos, UPRIGHT), map)) then
		return false
	end	
	-- Bottom left corner
	if (isTileOfGivenType("River", relativePosition(pos, LEFT), map)
		and isTileOfGivenType("River", relativePosition(pos, DOWN), map)
		and isTileOfGivenType("River", relativePosition(pos, DOWNLEFT), map)) then
		return false
	end	
	-- Bottom right corner
	if (isTileOfGivenType("River", relativePosition(pos, RIGHT), map)
		and isTileOfGivenType("River", relativePosition(pos, DOWN), map)
		and isTileOfGivenType("River", relativePosition(pos, DOWNRIGHT), map)) then
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
	local pos = tile:position()
	-- Checking for buildings and units
	local building = map:getBuilding(pos)
	if (building ~= nil) then -- if building there
		if (building:type() == "HQ") then
			local id = factionToID(building:faction())
			if (id >= 1 and id <= 4) then
				game:stopDrawingXSprite("HQ_up_" .. id)
			end
		end
		map:removeBuilding(pos)
	end
	eraseUnitIfNeeded(pos) -- Deleting present unit if needed
	-- Transitions (specific)
	checkCoherencyAround(pos, map)
	checkCoherency(pos, map)
end

-- Erase a unit if present and if cannot stay there
function eraseUnitIfNeeded(pos)
	local nbOfArmies = game:nbOfArmies()
	local map = game:getMapPtr()
	if (map == nil) then
		return
	end
	for i = 0, nbOfArmies, 1 do
		local army = game:getArmy(i)
		if (army ~= nil) then
			local unit = army:getUnitPtr(pos)
			if (unit ~= nil and not canUnitMoveTo(
				database:findUnit(unit:type()), map:getTileType(pos))) then
				army:removeUnit(pos)
				break -- we can stop (1 unit per tile)
			end
		end
	end
end

nullPos = sf.Vector2i(-1, -1)
local hqPos = { nullPos, nullPos, nullPos, nullPos }
local hqUpAnims = { db.Anim("hq_up_1", "hq.png"),
	db.Anim("hq_up_2", "hq.png"),
	db.Anim("hq_up_3", "hq.png"),
	db.Anim("hq_up_4", "hq.png") }
for i = 1, 4, 1 do
	hqUpAnims[i]:addFrame((i-1)*20, 2)
end

-- Returns faction ID from faction name
function factionToID(faction)
	if (faction == "Orange Star") then
		return 1
	elseif (faction == "Blue Moon") then
		return 2
	elseif (faction == "Green Earth") then
		return 3
	elseif (faction == "Yellow Comet") then
		return 4
	elseif (faction == "") then
		return 0 -- neutral
	else
		return -1 -- null faction
	end
end
-- Draw up part of HQ
function drawHqUpperPart(pos, id, filter)
	if (pos == nil or id == nil or id < 1 or id > 4) then
		return
	end
	local animId = "HQ_up_" .. id
	game:stopDrawingXSprite(animId)
	local xsprite = XSprite() xsprite:playAnim(hqUpAnims[id])
	if (filter ~= nil) then
		xsprite:setFilter(filter)
	end
	xsprite:SetPosition(GameEntity.tilesToPixels(relativePosition(pos, UP)))
	game:startDrawingXSprite(xsprite, animId)
end

-- Called when a building is placed on map (function  Map::placeBuilding)
function onBuildingPlaced(building, map)
	if (building == nil or map == nil) then
		return
	end
	local id = factionToID(building:faction())
	if (building:type() == "HQ" and id >= 1 and id <= 4) then
		-- Removing older HQ if needed
		if (hqPos[id] ~= building:position()) then -- would remove the placed HQ
				local buildingType = map:getBuildingType(hqPos[id])
			if (buildingType == "" or buildingType == "HQ") then
				map:removeBuilding(hqPos[id])
				hqPos[id] = building:position()
				-- Finally checking if placed over another HQ
				for i = 1, 4, 1 do
					if (i ~= id) then
						if (hqPos[i] == hqPos[id]) then
							hqPos[i] = nullPos -- otherwise would remove new HQ when older will be replaced
							game:stopDrawingXSprite("HQ_up_" .. i) -- removing upper part
							break
						end
					end
				end
			end
		end
		drawHqUpperPart(hqPos[id], id)
	end
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
	if (force) then
		map:setTile(pos, "Plain")
		map:removeBuilding(pos)
	end
	return map:isBuildingPresent(pos)
end

-- Called when a map is loaded (function Game::loadMap)
function onMapLoaded(map)
	local size = map:size()
	hqPos = { map:findBuildingPos("HQ", "Orange Star"), 
		map:findBuildingPos("HQ", "Blue Moon"),
		map:findBuildingPos("HQ", "Green Earth"),
		map:findBuildingPos("HQ", "Yellow Comet") }
	for i = 0, size.y, 1 do
		for j = 0, size.x, 1 do
			local pos = sf.Vector2i(j, i)
			checkCoherency(pos, map) -- checking tile graphical coherency (could be an older version...)
		end
	end
	-- Checking HQ graphical coherency
	for i = 1, 4, 1 do
		drawHqUpperPart(hqPos[i], i)
	end
end