print "--- Starting test script ---"
io.write("Hello world, from ",_VERSION," !\n")
print "* Starting database test *"
-- Translation test
-- Spanish test
trans:selectLang("es")
print(trans:tr("soldier")) -- Not translated
trans:translateItem("soldier", "soldado") -- Translating
print(trans:tr("soldier")) -- Translated
-- French test
trans:selectLang("fr")
trans:translateItem("soldier", "soldat")
print(trans:tr("soldier"))
-- / Translation Test

vm:include("test_database.lua") -- Can be included only once ; separe by ";"
print "* Ending database test *"

gFph:addFile("test.txt", "data/test.txt") -- Will replace if existing
gFph:addFile("test.txt", "data/test2.txt", false) -- Will not replace
print(gFph:getFilepath("test.txt")) -- Will return empty string if not existing

print "--- Ending test script ---"
