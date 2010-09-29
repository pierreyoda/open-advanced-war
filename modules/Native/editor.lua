--[[ editor.lua
Part of Native module for Open Advanced War
Manages map editor. ]]

function extractAnimationFromDatabase(class, type_, anim, frameId)
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
	local animPtr = ptr:findAnim(anim)
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

function addSpriteItemToList(list, class, type_, anim, frameId)
	subrect = sf.IntRect(0, 0, 20, 20)
	local image = extractAnimationFromDatabase(class, type_, anim, frameId)
	list:addItem(type_, gFph:getFilepath(image), subrect)
end

function buildEditorTerrainList(verticalSpriteList)
	addSpriteItemToList(verticalSpriteList, TILE, "Plain", "base")
	addSpriteItemToList(verticalSpriteList, TILE, "Forest", "base")
	addSpriteItemToList(verticalSpriteList, TILE, "Road", "base_up")
	addSpriteItemToList(verticalSpriteList, TILE, "River", "base_right")
	addSpriteItemToList(verticalSpriteList, BUILDING, "Barrack", "base")
end

function onEditorGuiListItemSelected(itemId)
	local class = GameEntity.findClassFromType(itemId)
	if (class == BUILDING) then
		game:setEditorBuilding(itemId)
	elseif (class == TILE) then
		game:setEditorTile(itemId)
		game:setEditorBuilding("") -- do not place buildings anymore
	end
end

-- Called to build terrain list (tiles and buildings) in editor GUI.
function buildEditorGui(editorGui)
	editorGui:addVerticalSpriteList("terrain", -- id
		sf.Vector2f(500, 90), -- size
		sf.Vector2f(50, 5), -- padding with gui space start
		"buildEditorTerrainList") -- build function to call (see higher)	
	editorGui:addVerticalSpriteList("a", -- id
		sf.Vector2f(90, 90), -- size
		sf.Vector2f(590, 5), -- padding with gui space start
		"buildEditorTerrainList") -- build function to call (see higher)
end