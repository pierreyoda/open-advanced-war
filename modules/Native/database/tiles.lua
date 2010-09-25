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
anim:clear():setImage("forest.png"):addFrame(0, 0) -- We can use same animation instance for simple tiles (clearing it before)
forest:addAnim(anim)
-- Road
road = db.Tile("Road")
road:setProtection(0)
anim = db.Anim("base_right", "road.png") anim:addFrame(65, 21)
anim2 = db.Anim("base_up", "road.png") anim2:addFrame(65, 0)
anim3 = db.Anim("base_inter4", "road.png") anim3:addFrame(21, 21)
road:addAnim(anim):addAnim(anim2):addAnim(anim3)
anim = db.Anim("base_left_up", "road.png") anim:addFrame(42, 42)
anim2 = db.Anim("base_right_up", "road.png") anim2:addFrame(0, 42)
anim3 = db.Anim("base_left_down", "road.png") anim3:addFrame(42, 0)
anim4 = db.Anim("base_right_down", "road.png") anim4:addFrame(0, 0)
road:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)
anim = db.Anim("base_inter3_up", "road.png") anim:addFrame(21, 42)
anim2 = db.Anim("base_inter3_down", "road.png") anim2:addFrame(21, 0)
anim3 = db.Anim("base_inter3_right", "road.png") anim3:addFrame(0, 21)
anim4 = db.Anim("base_inter3_left", "road.png") anim4:addFrame(42, 21)
road:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)
-- River
river = db.Tile("River") river:setProtection(1)
anim = db.Anim("base_right", "river.png") anim:addFrame(80, 0)
anim2 = db.Anim("base_up", "river.png") anim2:addFrame(80, 21)
anim3 = db.Anim("base_inter4", "river.png") anim3:addFrame(21, 21)
river:addAnim(anim):addAnim(anim2):addAnim(anim3)
anim = db.Anim("base_left_up", "river.png") anim:addFrame(42, 42)
anim2 = db.Anim("base_right_up", "river.png") anim2:addFrame(0, 42)
anim3 = db.Anim("base_left_down", "river.png") anim3:addFrame(42, 0)
anim4 = db.Anim("base_right_down", "river.png") anim4:addFrame(0, 0)
river:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)
anim = db.Anim("base_inter3_up", "river.png") anim:addFrame(21, 42)
anim2 = db.Anim("base_inter3_down", "river.png") anim2:addFrame(21, 0)
anim3 = db.Anim("base_inter3_right", "river.png") anim3:addFrame(0, 21)
anim4 = db.Anim("base_inter3_left", "river.png") anim4:addFrame(42, 21)
river:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)

-- Finally adding all to database
database:addTile(plain):addTile(forest):addTile(road):addTile(river)

