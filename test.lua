print "--- Starting test script ---"
io.write("Hello world, from ",_VERSION," !\n")
classe = Hello("Test class from LUA script file")
classe:blah()
classe:speek(56)
classe:speek("Hello from Lua!")
test:blah()
test = Hello("lua reassignated c++ test class")
test:blah()
test:speek(57)
test:speek("Script rocks!")

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

print "--- Ending test script ---"
