--[[ editor.lua
Part of AW1 module for Open Advanced War
Manages map editor. ]]

function editor_place(pos)
	local map = game:getMapPtr()
	if (map == nil) then
		return
	end
	if (editor_toPlaceType == TILE) then
		map:setTile(pos, editor_toPlace)
	elseif (editor_toPlaceType == BUILDING) then
		map:placeBuilding(pos, editor_toPlace, editor_toPlaceFaction, true)
	elseif (editor_toPlaceType == UNIT and editor_toPlaceFaction ~= "") then
		local id = factionToID(editor_toPlaceFaction)-1
		if (id >= 0 and id <= 3) then
			if (game:getArmy(id) == nil) then -- no army of that faction
				game:addArmy(id, editor_toPlaceFaction)
			end
			game:spawnUnit(id, editor_toPlace, pos)
		end
	end
end

function extractAnimationFromDatabase(class, type_, anim, faction, frameId)
	local ptr = nil
	if (class == TILE) then
		ptr = database:findTile(type_)
	elseif (class == BUILDING) then
		ptr = database:findBuilding(type_)
	elseif (class == UNIT) then
		ptr = database:findUnit(type_)
	else
		return ""
	end
	if (ptr == nil) then
		return ""
	end
	if (faction == nil) then
		faction = "" -- null faction
	end
	local animPtr = ptr:findAnim(anim, faction)
	if (animPtr == nil) then
		return ""
	end
	if (frameId == nil) then -- not defined
		frameId = 0
	end
	local framePtr = animPtr:getFrame(frameId)
	if (framePtr == nil) then
		return ""
	end
	subrect = sf.IntRect(framePtr.x, framePtr.y, framePtr.w, framePtr.h)
	return animPtr:image()
end

function addSpriteItemToList(list, class, type_, anim, faction, frameId)
	subrect = sf.IntRect(0, 0, 20, 20)
	local image = extractAnimationFromDatabase(class, type_, anim, faction, frameId)
	list:addItem(type_, gFph:getFilepath(image), subrect)
end

function addOrangeStarSpriteItemToList(list, class, type_, anim, frameId)
	addSpriteItemToList(list, class, type_, anim, "Orange Star", frameId)
end
function addBlueMoonSpriteItemToList(list, class, type_, anim, frameId)
	addSpriteItemToList(list, class, type_, anim, "Blue Moon", frameId)
end
function addGreenEarthSpriteItemToList(list, class, type_, anim, frameId)
	addSpriteItemToList(list, class, type_, anim, "Green Earth", frameId)
end
function addYellowCometSpriteItemToList(list, class, type_, anim, frameId)
	addSpriteItemToList(list, class, type_, anim, "Yellow Comet", frameId)
end

function buildEditorTerrainList(verticalSpriteList)
	addSpriteItemToList(verticalSpriteList, TILE, "Plain", "base")
	addSpriteItemToList(verticalSpriteList, TILE, "Forest", "base")
	addSpriteItemToList(verticalSpriteList, TILE, "Mountain", "base")
	addSpriteItemToList(verticalSpriteList, TILE, "Road", "base_up")
	addSpriteItemToList(verticalSpriteList, TILE, "River", "base_right")
	addSpriteItemToList(verticalSpriteList, TILE, "Sea", "base_deep")
	addSpriteItemToList(verticalSpriteList, BUILDING, "Base", "base")
end

function buildEditorOrangeStarList(verticalSpriteList)
	verticalSpriteList:addItem("HQ", gFph:getFilepath("hq.png"), sf.IntRect(0, 2, 20, 38))
	addOrangeStarSpriteItemToList(verticalSpriteList, BUILDING, "Base", "base")
	addOrangeStarSpriteItemToList(verticalSpriteList, UNIT, "Infantry", "base")
	addOrangeStarSpriteItemToList(verticalSpriteList, UNIT, "Tank", "base")
end
function buildEditorBlueMoonList(verticalSpriteList)
	verticalSpriteList:addItem("HQ", gFph:getFilepath("hq.png"), sf.IntRect(20, 2, 20, 38))
	addBlueMoonSpriteItemToList(verticalSpriteList, BUILDING, "Base", "base")
	addBlueMoonSpriteItemToList(verticalSpriteList, UNIT, "Infantry", "base")
end
function buildEditorGreenEarthList(verticalSpriteList)
	verticalSpriteList:addItem("HQ", gFph:getFilepath("hq.png"), sf.IntRect(40, 2, 20, 38))
	addGreenEarthSpriteItemToList(verticalSpriteList, BUILDING, "Base", "base")
	addGreenEarthSpriteItemToList(verticalSpriteList, UNIT, "Infantry", "base")
end
function buildEditorYellowCometList(verticalSpriteList)
	verticalSpriteList:addItem("HQ", gFph:getFilepath("hq.png"), sf.IntRect(60, 2, 20, 38))
	addYellowCometSpriteItemToList(verticalSpriteList, BUILDING, "Base", "base")
	addYellowCometSpriteItemToList(verticalSpriteList, UNIT, "Infantry", "base")
end

function onEditorGuiListItemSelected(listName, itemId)
	local class = GameEntity.findClassFromType(itemId)
	if (listName == "terrain") then
		editor_toPlaceFaction = "" -- null faction (neutral for buildings)
	elseif (listName == "OrangeStar") then
		editor_toPlaceFaction = "Orange Star"
	elseif (listName == "BlueMoon") then
		editor_toPlaceFaction = "Blue Moon"
	elseif(listName == "GreenEarth") then
		editor_toPlaceFaction = "Green Earth"	
	elseif(listName == "YellowComet") then
		editor_toPlaceFaction = "Yellow Comet"
	end
	editor_toPlaceType, editor_toPlace = class, itemId
end

local CARAC_SIZE = 7 -- determined with tests
local width = 300
local caracLimit = width / CARAC_SIZE
local MAP_FILE = "map.xml"
function onEditorGuiButtonClicked(buttonId)
	local function printElapsedTime(timer, save)
		if (save) then
			io.write("Map saving")
		else
			io.write("Map loading")
		end
		io.write(" time : ", timer:getElapsedTime())
		print()
	end
	local timer = PausableClock(true)
	if (buttonId == "saveButton") then
		timer:start()
		if (game:saveMap(MAP_FILE)) then
			printElapsedTime(timer, true)
		end
	elseif (buttonId == "loadButton") then
		timer:start()
		if (game:loadMap(MAP_FILE)) then
			printElapsedTime(timer, false)
		end
	elseif (buttonId == "randomButton") then
		randomMap(game:getMapPtr())
	elseif (buttonId == "toGameButton") then
		IN_GAME, IN_EDITOR = true, false
		if (game:guiExists("GameGui")) then
			game:setCurrentGui("GameGui")
		else
			game:addGui("GameGui")
		end
	end
end

-- Called to build terrain list (tiles and buildings) in editor GUI.
function buildEditorGui(editorGui)
	editorGui:addVerticalSpriteList("terrain", -- id
		sf.Vector2f(90, 90), -- size
		sf.Vector2f(50, 5), -- padding with gui space start
		"buildEditorTerrainList") -- build function to call (see higher)	
	editorGui:addVerticalSpriteList("OrangeStar",
		sf.Vector2f(60, 90),
		sf.Vector2f(300, 5),
		"buildEditorOrangeStarList")	
	editorGui:addVerticalSpriteList("BlueMoon",
		sf.Vector2f(60, 90),
		sf.Vector2f(390, 5),
		"buildEditorBlueMoonList")	
	editorGui:addVerticalSpriteList("GreenEarth",
		sf.Vector2f(60, 90),
		sf.Vector2f(470, 5),
		"buildEditorGreenEarthList")	
	editorGui:addVerticalSpriteList("YellowComet",
		sf.Vector2f(60, 90),
		sf.Vector2f(560, 5),
		"buildEditorYellowCometList")
	editorGui:addButton("saveButton", --id
		trans:tr("Save"), -- text
		sf.Vector2f(80, 40), -- size
		sf.Vector2f(700, 5)) -- padding with gui space start	
	editorGui:addButton("loadButton",
		trans:tr("Load"),
		sf.Vector2f(80, 40),
		sf.Vector2f(700, 55))
	trans:selectLang("fr")
	trans:translateItem("Random", "Al�atoire")
	editorGui:addButton("randomButton",
		trans:tr("Random"),
		sf.Vector2f(80, 40),
		sf.Vector2f(200, 55))	
	editorGui:addButton("toGameButton",
		trans:tr("To game"),
		sf.Vector2f(80, 40),
		sf.Vector2f(200, 5))
end