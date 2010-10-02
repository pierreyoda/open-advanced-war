--[[ database.lua
Part of AW1 module for Open Advanced War
Insert module's informations into database. 
DO NOT INCLUDE IN-GAME! ; use it with DatabaseConverter tool 
(same thing for other databases files) ]]

-- "Constantes" (DO NOT MODIFY)
MODULE_DIR = "modules/AW1/"
DATABASE_FILES_PATH = MODULE_DIR .. "database/"
MODULE_NAME = "AW1"
-- /"Constantes" (DO NOT MODIFY)

vm:include("tiles.lua;translations.lua;categories.lua;units.lua;buildings.lua",
	DATABASE_FILES_PATH)