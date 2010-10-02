--[[ categories.lua
Part of AW1 module for Open Advanced War
Describes units categories used. ]]

-- Infantry : infantry units
infantry = db.Category("Infantry")
infantry:addItem("Soldier"):addItem("Bazooka")
-- Finally adding all to database
database:addCategory(infantry)