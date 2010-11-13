--[[ units.lua
Part of AW1 module for Open Advanced War
Describes units. ]]

-- Infantry
soldier = db.Unit("Infantry") soldier:setPropulsion("InfantryProp")
anim = db.Anim("base", "soldier.png") anim:addFrame(0, 0)
soldier:addIntCaracteristic("move", 3):addIntCaracteristic("cost", 1000):addAnim(anim, "Orange Star")
anim:clear():addFrame(20, 0) soldier:addAnim(anim, "Blue Moon")
anim:clear():addFrame(40, 0) soldier:addAnim(anim, "Green Earth")
anim:clear():addFrame(60, 0) soldier:addAnim(anim, "Yellow Comet")

-- Tank (basic)
tank = db.Unit("Tank") tank:setPropulsion("TreadsProp")
anim:clear():setImage("tank.png"):addFrame(0, 0)
tank:addAnim(anim, "Orange Star")
tank:addIntCaracteristic("move", 6):addIntCaracteristic("fuel", 70):addIntCaracteristic("cost", 7000)

-- Finally adding all to database
database:addUnit(soldier):addUnit(tank)

-- Propulsions
local propInfantry = db.Propulsion("InfantryProp")
propInfantry:addCanMoveTo("Road", 1):addCanMoveTo("Plain", 1):addCanMoveTo("Forest", 1):addCanMoveTo("River", 2):addCanMoveTo("Mountain", 2)
local propTreads = db.Propulsion("TreadsProp")
propTreads:addCanMoveTo("Road", 1):addCanMoveTo("Plain", 2):addCanMoveTo("Forest", 3)
database:addPropulsion(propInfantry):addPropulsion(propTreads)

