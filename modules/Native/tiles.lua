--[[ tiles.lua
Part of Native module for Open Advanced War
Describes tiles used. ]]

-- Plain
plain = db.Tile("Plain")
plain:setProtection(1)
anim = db.Anim("base", "plain.png") anim:addFrame(0, 0)
plain:addAnim(anim)
-- Forest
forest = db.Tile("Forest")
forest:setProtection(3)
anim:clear():setImage("forest.png"):addFrame(0, 0):addFrame(20, 0) -- We can use same animation instance for simple tiles
forest:addAnim(anim)
-- Road
road = db.Tile("Road", true)
road:setProtection(0)
-- road:setAutoRotation(true) -- For conveniance (no need to create lots of animations).
anim = db.Anim("tile_right", "road.png") anim:addFrame(0, 0)
anim2 = db.Anim("tile_intersection4", "road.png") anim2:addFrame(20, 0)
anim3 = db.Anim("tile_intersection3", "road.png") anim3:addFrame(40, 0)
road:addAnim(anim):addAnim(anim2):addAnim(anim3)

-- Finally adding all to database
database:addTile(plain):addTile(forest):addTile(road)

-- Pointer test
tilePtr = database:findTile("Pjlain") -- null pointer
database:addTile(tilePtr) -- will not crash
tilePtr = database:findTile("Forest")
tilePtr:setProtection(5) -- modify original
database:addTile(tilePtr) -- ignored : already present