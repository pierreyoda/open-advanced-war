--[[ game.lua
Part of AW1 module for Open Advanced War
Manages game. ]]

function buildGameGui(gameGui)
	gameGui:addButton("endTurnButton",
		trans:tr("End turn"),
		sf.Vector2f(80, 40),
		sf.Vector2f(700, 55))
end

function onGameGuiButtonClicked(buttonId)
	if (buttonId == "endTurnButton") then
		print "End turn!"
	end
end