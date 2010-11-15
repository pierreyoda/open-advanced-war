--[[ map_randomizer.lua
Part of AW1 module for Open Advanced War
Algorithm to create a new random map. ]]

local percentages = { plain = 60, sea = 30, forest = 5, mountain = 5 }
local hqLocationPercentages = { left = 20, top = 20, width = 40, height = 40 }
local two_HQs_gap = { min = 3 }
local TRY_LIMIT = 100
function randomMap(map)
	local size, done = map:size(), { plain = 0, sea = 0, forest = 0, mountain = 0 }
	local total = size.x*size.y
	local function percent(number)
		return (number/total) * 100
	end
	local function percentOf(percentage, of)
		return (percentage/100) * of
	end
	-- Randomizing terrain
	for i = 0, size.x-1 do
		for j = 0, size.y-1 do
			local number = math.random(100) -- number between 1 and 100
			if (percent(done.mountain) < percentages.mountain and
				number >= 100-percentages.mountain) then
				map:setTile(i, j, "Mountain")
				done.mountain = done.mountain+1
			elseif (percent(done.forest) < percentages.forest and
				number >= 100-percentages.mountain-percentages.forest) then
				map:setTile(i, j, "Forest")
				done.forest = done.forest+1
			elseif (percent(done.sea) < percentages.sea and
				number >= percentages.plain) then
				map:setTile(i, j, "Sea")
				done.sea = done.sea+1
			else
				map:setTile(i, j, "Plain")
				done.plain = done.plain+1
			end
		end
	end
-- Placing buildings
	local hqLocationRect = sf.IntRect(percentOf(hqLocationPercentages.left, size.x),
		percentOf(hqLocationPercentages.top, size.y),
		percentOf(hqLocationPercentages.width, size.x),
		percentOf(hqLocationPercentages.height, size.y))
	-- First HQ
	local x, y = -1, -1
	for i = 1, TRY_LIMIT do
		x, y = math.random(hqLocationRect.x, hqLocationRect.x+hqLocationRect.w),
			math.random(hqLocationRect.y, hqLocationRect.y+hqLocationRect.h)
		if (map:placeBuilding(x, y, "HQ", "Orange Star", true)) then
			break
		end
	end
	-- Second HQ
	local x2, y2 = -1, -1
	for i = 1, TRY_LIMIT do
		x2, y2 = math.random(hqLocationRect.x, hqLocationRect.x+hqLocationRect.w),
			math.random(hqLocationRect.y, hqLocationRect.y+hqLocationRect.h)
		if ((x2 ~= x and y2 ~= y) and math.abs(x2-x) >= two_HQs_gap.min 
			and math.abs(y2-y) >= two_HQs_gap.min) then
			if (map:placeBuilding(x2, y2, "HQ", "Blue Moon", true)) then
				break
			end
		end
	end
	
end