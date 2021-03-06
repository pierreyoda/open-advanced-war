--[[ database.lua
Part of Native module for Open Advanced War
Insert module's informations into database. 
DO NOT INCLUDE IN-GAME! ; use it with DatabaseConverter tool 
(same thing for other databases files) ]]

-- "Constantes" (DO NOT MODIFY)
MODULE_DIR = "modules/Native/"
DATABASE_FILES_PATH = MODULE_DIR .. "database/"
MODULE_NAME = "Native"
-- /"Constantes" (DO NOT MODIFY)

vm:include("tiles.lua;translations.lua;categories.lua;units.lua;buildings.lua",
	DATABASE_FILES_PATH)