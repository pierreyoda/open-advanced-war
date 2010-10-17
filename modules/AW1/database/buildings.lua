--[[ buildings.lua
Part of AW1 module for Open Advanced War
Describes tiles used. ]]

-- Base
base = db.Building("Base", 1000, true) 
anim = db.Anim("base", "base.png")
anim:addFrame(0, 0)
base:setResistance(20):addProduction("Infantry", true):addAnim(anim)
anim:clear():addFrame(20, 0) base:addAnim(anim, "Orange Star")
anim:clear():addFrame(40, 0) base:addAnim(anim, "Blue Moon")

-- HQ (Headquarters)
hq = db.Building("HQ", 1000, true)
hq:setResistance(20)
anim:clear():setImage("hq.png"):addFrame(db.Frame(0, 2, 20, 38, 1)) hq:addAnim(anim, "Orange Star")
anim:clear():addFrame(db.Frame(20, 2, 20, 38, 1)) hq:addAnim(anim, "Blue Moon")

-- Finally adding all to database
database:addBuilding(base):addBuilding(hq)
