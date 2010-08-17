frame1, frame2 = db.Frame(0, 0, 0.5), db.Frame(20, 0, 0.5)
frame1.duration, frame1.x = 1.0, 40
anim1 = db.Anim("move_right", "tank.png")
anim1:addFrame(frame1):addFrame(frame2)
tilePlain = db.Tile("plain")
tilePlain:addAnim(anim1):addAnim(anim1)
tilePlain:setProtection(1)
io.write(anim1:name(), " ", anim1:image()) print()