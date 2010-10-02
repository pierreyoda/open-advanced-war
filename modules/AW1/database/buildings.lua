--[[ buildings.lua
Part of AW1 module for Open Advanced War
Describes tiles used. ]]

-- Base
base = db.Building("Base", 1000, true) 
anim = db.Anim("base", "base.png")
anim:addFrame(0, 0)
base:setResistance(20):addProduction("Infantry", true):addAnim(anim)
anim:clear():addFrame(20, 0) base:addAnim(anim, "Orange Star")

-- Finally adding all to database
database:addBuilding(base)
