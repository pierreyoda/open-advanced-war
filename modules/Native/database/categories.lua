--[[ categories.lua
Part of Native module for Open Advanced War
Describes units categories used. ]]

-- Infantry : infantry units
infantry = db.Category("Infantry")
infantry:addItem("Soldier")
-- Finally adding all to database
database:addCategory(infantry)