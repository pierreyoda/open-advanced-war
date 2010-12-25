--[[ game.lua
Part of AW1 module for Open Advanced War
Manages game. ]]

function buildGameGui(gameGui)
	gameGui:addButton("toEditorButton",
		trans:tr("To editor"),
		sf.Vector2f(80, 40),
		sf.Vector2f(700, 55))
end

function onGameGuiButtonClicked(buttonId)
	if (buttonId == "toEditorButton") then
		IN_GAME, IN_EDITOR = false, true
		if (game:guiExists("EditorGui")) then
			game:setCurrentGui("EditorGui")
		else
			game:addGui("EditorGui")
		end
	end
end

local CARAC_SIZE = 7 -- determined with tests
local width = 300
local caracLimit = width / CARAC_SIZE
local function alignRight(table_, index, separator)
	local text = table_[index]
	local size, separatorPos = #text, text:find(separator)
	if (separatorPos == nil) then -- not found
		return
	end
	-- Adding first part
	local temp = text:sub(0, separatorPos-1)
	local temp2 = text:sub(separatorPos+1, size)
	-- Adding needed spaces
	local needed = caracLimit - #temp - #temp2
	if (#temp2 < 5) then -- again, determined by some tests
		needed = needed+1
	end
	for i = 1, needed do
		temp = temp .. " "
	end
	-- Adding second part
	table_[index] = temp .. temp2 -- text is a copy!
end

function makeBuildChoice(pos)
	local building = game:getMapPtr():getBuilding(pos)
	if (building == nil) then
		return
	end
	local buildingType, factionId = building:type(), factionToID(building:faction())-1
	if (factionId < 0 or factionId > 3) then
		return
	end
	local possible = { }
	if (buildingType == "Base") then
		for i = 1, #landUnits do
			local dbUnit = database:findUnit(landUnits[i])
			if (dbUnit ~= nil) then
				possible[#possible+1] = landUnits[i] .. "=" .. dbUnit:findIntFeature("cost")
				alignRight(possible, #possible, "=")
			end
		end
	else
		return
	end
	local choice = game:getChoiceFromTable(possible, #possible,
		sf.FloatRect(10, 10, width, 300))+1
	if (choice < 1) then
		return
	end
	local toBuild = ""
	if (buildingType == "Base") then
		toBuild = landUnits[choice]
	end
	if (game:getArmy(factionId) == nil) then -- no army of that faction
		game:addArmy(factionId, building:faction())
	end
	game:spawnUnit(factionId, toBuild, pos)
end