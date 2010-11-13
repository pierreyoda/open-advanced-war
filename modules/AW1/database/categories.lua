--[[ categories.lua
Part of AW1 module for Open Advanced War
Describes units categories used. ]]

-- Infantry : infantry units
infantry = db.Category("InfantryCategory")
infantry:addItem("Infantry"):addItem("Mech")
-- Finally adding all to database
database:addCategory(infantry)