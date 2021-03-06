--[[ tiles.lua
Part of AW1 module for Open Advanced War
Describes tiles used. ]]

-- Plain
plain = db.Tile("Plain")
plain:setProtection(1)
anim = db.Anim("base", "plain.png") anim:addFrame(0, 0)
anim2 = db.Anim("base_shady", "plain.png") anim2:addFrame(20, 0)
plain:addAnim(anim):addAnim(anim2)
-- Forest
forest = db.Tile("Forest")
forest:setProtection(3)
anim:clear():setImage("forest.png"):addFrame(0, 0) -- We can use same animation instance for simple tiles (clearing it before)
forest:addAnim(anim)
-- Mountain
mountain = db.Tile("Mountain")
mountain:setProtection(4)
anim:clear():setImage("mountain.png"):addFrame(0, 0)
anim2 = db.Anim("base_high", "mountain.png") anim2:addFrame(db.Frame(20, 0, 20, 22, 1))
mountain:addAnim(anim):addAnim(anim2)
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
-- Sea
sea = db.Tile("Sea") sea:setProtection(1)
anim = db.Anim("base_alone", "sea.png") anim:addFrame(110, 0)
anim2 = db.Anim("base_right", "sea.png") anim2:addFrame(21, 0)
anim3 = db.Anim("base_up", "sea.png") anim3:addFrame(0, 0)
anim4 = db.Anim("base_deep", "sea.png") anim4:addFrame(110, 21) -- ~= "inter4_deep"
sea:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)
anim = db.Anim("base_left_up", "sea.png") anim:addFrame(42, 63)
anim2 = db.Anim("base_right_up", "sea.png") anim2:addFrame(0, 63)
anim3 = db.Anim("base_left_down", "sea.png") anim3:addFrame(42, 21)
anim4 = db.Anim("base_right_down", "sea.png") anim4:addFrame(0, 21)
sea:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)
anim = db.Anim("base_left_up_deep", "sea.png") anim:addFrame(63, 0)
anim2 = db.Anim("base_right_up_deep", "sea.png") anim2:addFrame(42, 0)
anim3 = db.Anim("base_left_down_deep", "sea.png") anim3:addFrame(63, 21)
anim4 = db.Anim("base_right_down_deep", "sea.png") anim4:addFrame(63, 42)
sea:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)
anim = db.Anim("base_inter3_up", "sea.png") anim:addFrame(21, 63)
anim2 = db.Anim("base_inter3_down", "sea.png") anim2:addFrame(21, 21)
anim3 = db.Anim("base_inter3_right", "sea.png") anim3:addFrame(0, 42)
anim4 = db.Anim("base_inter3_left", "sea.png") anim4:addFrame(42, 42)
sea:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)
anim = db.Anim("base_inter3_up_deep", "sea.png") anim:addFrame(84, 21)
anim2 = db.Anim("base_inter3_down_deep", "sea.png") anim2:addFrame(84, 0)
anim3 = db.Anim("base_inter3_right_deep", "sea.png") anim3:addFrame(0, 90)
anim4 = db.Anim("base_inter3_left_deep", "sea.png") anim4:addFrame(21, 90)
sea:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)
anim = db.Anim("base_end_up", "sea.png") anim:addFrame(110, 90)
anim2 = db.Anim("base_end_down", "sea.png") anim2:addFrame(110, 69)
anim3 = db.Anim("base_end_left", "sea.png") anim3:addFrame(89, 90)
anim4 = db.Anim("base_end_right", "sea.png") anim4:addFrame(89, 69)
sea:addAnim(anim):addAnim(anim2):addAnim(anim3):addAnim(anim4)
anim = db.Anim("base_inter4", "sea.png") anim:addFrame(21, 42)
sea:addAnim(anim)

-- Finally adding all to database
database:addTile(plain):addTile(forest):addTile(road):addTile(mountain):addTile(river):addTile(sea)
