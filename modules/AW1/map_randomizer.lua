--[[ map_randomizer.lua
Part of AW1 module for Open Advanced War
Algorithm to create a new random map. ]]

local percentages = { plain = 60, sea = 30, forest = 5, mountain = 5 }
function randomMap(map)
	local size, done = map:size(), { plain = 0, sea = 0, forest = 0, mountain = 0 }
	local total = size.x*size.y
	local function percent(number)
		return (number/total) * 100
	end
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
end