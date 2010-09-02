--[[ units.lua
Part of Native module for Open Advanced War
Describes units. ]]

-- Jeep (HMMWV : High Mobility Multipurpose Wheeled Vehicule)
jeep = db.Unit("HMMWV")
anim = db.Anim("base", "hmmwv.png") anim:addFrame(0, 0)
jeep:addAnim(anim)

-- Tank (basic)
tank = db.Unit("Tank")
anim:clear():setImage("tank.png"):addFrame(0, 0)
tank:addAnim(anim)

-- Finally adding all to database
database:addUnit(jeep):addUnit(tank)