--[[ main.lua
Part of Native module for Open Advanced War
Module's entry in-game. ]]

moduleName, moduleDir = "Native", "modules/Native/"
database = "database.xml"

-- For convenience (shorter)
NONE, TILE, BUILDING, UNIT = 
	GameEntity.NONE,
	GameEntity.TILE,
	GameEntity.BUILDING,
	GameEntity.UNIT

vm:include("ai.lua;ingame_map.lua;graphic_effects.lua", moduleDir)