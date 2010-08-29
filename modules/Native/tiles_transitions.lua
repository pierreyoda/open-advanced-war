--[[ tiles_transitions.lua
Part of Native module for Open Advanced War
Manges tiles transitions in-game. ]]

function tileSurroundedBy(pos, tileType, map, by4)
	if (by4 == true) then
		return (map:getTile(pos.x-1, pos.y) == tileType
			and map:getTile(pos.x+1, pos.y) == tileType
			and map:getTile(pos.x, pos.y-1) == tileType
			and map:getTile(pos.x, pos.y+1) == tileType)
	end
	for i = -1, 1, 1 do
		for j = -1, 1, 1 do
			if (map:getTile(pos.x+i, pos.x+j) ~= tileType) then
				return false
			end
		end
	end
	return true
end

function onTilePlaced(pos, map) -- When a tile is placed on map ; position of that tile
	local tile = getTilePtr(pos)
	local left, right, up, down = map:getTilePtr(pos.x-1, pos.y):orientation(), 
		map:getTilePtr(pos.x+1, pos.y):orientation(),
		map:getTilePtr(pos.x, pos.y-1):orientation(), 
		map:getTilePtr(pos.x, pos.y+1):orientation()
	-- Transitions
	if (tile:type() == "Mountain") then
		if (tileSurroundedBy(pos, "Mountain", map, true)) then
			tile:playAnim("munain_surrunded_by_4")
			return
		end
	end
	-- Orientable tiles
	if (not tile:isOrientable()) then
		return
	end
	if (left == RIGHT and right == RIGHT) then
		tile:playAnim("base_right")
	end
end