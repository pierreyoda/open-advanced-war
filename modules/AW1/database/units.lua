--[[ units.lua
Part of AW1 module for Open Advanced War
Describes units. ]]

local function addAllAnims(unit, image)
	local anim = db.Anim("base", image) anim:addFrame(0, 0)
	unit:addAnim(anim, "Orange Star")
	anim:clear():addFrame(20, 0) unit:addAnim(anim, "Blue Moon")
	anim:clear():addFrame(40, 0) unit:addAnim(anim, "Green Earth")
	anim:clear():addFrame(60, 0) unit:addAnim(anim, "Yellow Comet")
end

-- Infantry
soldier = db.Unit("Infantry") soldier:setPropulsion("InfantryProp")
soldier:addIntFeature("move", 3):addIntFeature("cost", 1000)
addAllAnims(soldier, "soldier.png")

-- Mech (Mechanized Infantry)
mech = db.Unit("Mech") mech:setPropulsion("InfantryProp")
mech:addIntFeature("move", 2):addIntFeature("cost", 3000)
addAllAnims(mech, "mech.png")

-- Recon
recon = db.Unit("Recon") recon:setPropulsion("TiresProp")
recon:addIntFeature("move", 8):addIntFeature("cost", 4000)
addAllAnims(recon, "recon.png")

-- APC (Armored Personnel Carrier)
apc = db.Unit("APC") apc:setPropulsion("TreadsProp")
apc:addIntFeature("move", 6):addIntFeature("cost", 5000)
addAllAnims(apc, "apc.png")

-- Tank (basic)
tank = db.Unit("Tank") tank:setPropulsion("TreadsProp")
tank:addIntFeature("move", 6):addIntFeature("cost", 7000):addIntFeature("fuel", 70)
addAllAnims(tank, "tank.png")

-- Tank M (Medium)
tankM = db.Unit("Tank M") tankM:setPropulsion("TreadsProp")
tankM:addIntFeature("move", 5):addIntFeature("cost", 16000):addIntFeature("fuel", 50)
addAllAnims(tankM, "tankM.png")

-- Artillery
artillery = db.Unit("Artillery") artillery:setPropulsion("TreadsProp")
artillery:addIntFeature("move", 5):addIntFeature("cost", 6000):addIntFeature("fuel", 50)
addAllAnims(artillery, "artillery.png")

-- Rockets (Rocket Launcher)
rocketLauncher = db.Unit("Rockets") rocketLauncher:setPropulsion("TiresProp")
rocketLauncher:addIntFeature("move", 5):addIntFeature("cost", 15000):addIntFeature("fuel", 50)
addAllAnims(rocketLauncher, "rocketLauncher.png")

-- Anti-Air (Anti-Air Tank)
aaTank = db.Unit("Anti-Air") aaTank:setPropulsion("TreadsProp")
aaTank:addIntFeature("move", 6):addIntFeature("cost", 8000):addIntFeature("fuel", 60)
addAllAnims(aaTank, "aaTank.png")

-- Missiles (Anti-Air Missile Launcher)
aaMissiles = db.Unit("Missiles") aaMissiles:setPropulsion("TiresProp")
aaMissiles:addIntFeature("move", 4):addIntFeature("cost", 12000):addIntFeature("fuel", 50)
addAllAnims(aaMissiles, "aaMissiles.png")

-- Finally adding all to database
database:addUnit(soldier):addUnit(mech):addUnit(recon):addUnit(apc):addUnit(tank)
	:addUnit(tankM):addUnit(artillery):addUnit(rocketLauncher):addUnit(aaTank):addUnit(aaMissiles)

-- Propulsions
local propInfantry = db.Propulsion("InfantryProp")
propInfantry:addCanMoveTo("Road", 1):addCanMoveTo("Plain", 1):addCanMoveTo("Forest", 1):addCanMoveTo("River", 2):addCanMoveTo("Mountain", 2)
local propTreads = db.Propulsion("TreadsProp")
propTreads:addCanMoveTo("Road", 1):addCanMoveTo("Plain", 2):addCanMoveTo("Forest", 3)
local propTires = db.Propulsion("TiresProp")
propTires:addCanMoveTo("Road", 1):addCanMoveTo("Plain", 2):addCanMoveTo("Forest", 3)
database:addPropulsion(propInfantry):addPropulsion(propTreads):addPropulsion(propTires)
