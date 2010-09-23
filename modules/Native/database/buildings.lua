--[[ buildings.lua
Part of Native module for Open Advanced War
Describes tiles used. ]]

-- Barrack
barrack = db.Building("Barrack", 1000, true) 
anim = db.Anim("base", "barrack.png")
anim:addFrame(0, 0)
barrack:setResistance(20):addProduction("Infantry", true):addProduction(jeep:name(), false):addAnim(anim)

-- Finally adding all to database
database:addBuilding(barrack)
