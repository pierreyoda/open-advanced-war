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

function addItemToTerrainList(editorGui, class, type_, anim, frameId)
	subrect = sf.IntRect(0, 0, 20, 20)
	local image = extractAnimationFromDatabase(class, type_, anim, frameId)
	editorGui:addItemToTerrainList(type_, gFph:getFilepath(image), subrect)
end

-- Called to build terrain list (tiles and buildings) in editor GUI.
function buildEditorTerrainList(editorGui)
	addItemToTerrainList(editorGui, TILE, "Plain", "base")
	addItemToTerrainList(editorGui, TILE, "Road", "base_up")
	addItemToTerrainList(editorGui, TILE, "River", "base_right")
	addItemToTerrainList(editorGui, TILE, "River", "base_up")
	addItemToTerrainList(editorGui, TILE, "River", "base_inter4")
	addItemToTerrainList(editorGui, BUILDING, "Barrack", "base")
end