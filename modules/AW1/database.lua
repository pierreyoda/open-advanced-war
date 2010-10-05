--[[ database.lua
Part of AW1 module for Open Advanced War
Insert module's informations into database. 
DO NOT INCLUDE IN-GAME! ; use it with DatabaseConverter tool 
(same thing for other databases files) ]]

vm:include("shared_globals.lua", 
	"modules/AW1/") -- we cannot use the global MODULE_DIR here...

vm:include("tiles.lua;translations.lua;categories.lua;units.lua;buildings.lua",
	MODULE_DIR .. "database/")