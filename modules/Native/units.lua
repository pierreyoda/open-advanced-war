--[[ units.lua
Part of Native module for Open Advanced War
Describes units. ]]

-- Jeep (HMMWV : High Mobility Multipurpose Wheeled Vehicule)
jeep = db.Unit("HMMWV")
anim = db.Anim("base_right", "jeep.png") anim:addFrame(0, 0)
jeep:addAnim(anim)
jeep:addIntCaracteristic("move", 8)
jeep:addIntCaracteristic("max_fuel", 70)
jeep:addIntCaracteristic("max_ammo_M2", 8)
-- jeep:addWeapon(machineGunM2)

-- Tank (basic)
tank = db.Unit("Tank")
anim:clear():setImage("tank.png"):addFrame(0, 0)
tank:addAnim(anim)

-- Finally adding all to database
database:addUnit(jeep):addUnit(tank)