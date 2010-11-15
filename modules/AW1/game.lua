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