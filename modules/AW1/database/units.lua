--[[ units.lua
Part of AW1 module for Open Advanced War
Describes units. ]]

-- Soldier
soldier = db.Unit("Soldier") soldier:setPropulsion("InfantryProp")
anim = db.Anim("base", "soldier.png") anim:addFrame(0, 0)
soldier:addAnim(anim, "Orange Star")

-- Tank (basic)
tank = db.Unit("Tank")
anim:clear():setImage("tank.png"):addFrame(0, 0)
tank:addAnim(anim, "Orange Star")

-- Finally adding all to database
database:addUnit(soldier):addUnit(tank)

-- Propulsions
local propInfantry = db.Propulsion("InfantryProp")
propInfantry:addCanMoveTo("Plain", 1)
propInfantry:addCanMoveTo("Forest", 1)
propInfantry:addCanMoveTo("River", 2)
database:addPropulsion(propInfantry)

