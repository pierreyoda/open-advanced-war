--[[ main.lua
Part of Native module for Open Advanced War
Module's entry in-game. ]]

-- "Constantes" DO NOT MODIFY!
MODULE_NAME, MODULE_DIR = "Native", "modules/Native/"
DATABASE_PATH = "database.xml"
ICON_PATH = "tank.png" -- Window's icon (will be extracted)
-- /"Constantes"

-- For convenience (shorter)
NONE, TILE, BUILDING, UNIT = 
	GameEntity.NONE,
	GameEntity.TILE,
	GameEntity.BUILDING,
	GameEntity.UNIT

vm:include("ai.lua;ingame_map.lua;graphic_effects.lua", MODULE_DIR)