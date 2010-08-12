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

-- Translation test
print "* Starting database test *"
-- Spanish test
trans:selectLang("es")
print(trans:tr("soldier", true)) -- Not translated
trans:translateItem("soldier", "soldado") -- Translating
print(trans:tr("soldier", true)) -- Translated
-- French test
trans:selectLang("fr")
trans:translateItem("soldier", "soldat")
print(trans:tr("soldier", true))
print "* Ending database test *"
-- / Translation Test

print "Pre-processor test:"
test = "from 'test.lua'"
vm:include("test_included.lua") -- Can be included only once ; separe by ";"
print(test2)

print "--- Ending test script ---"
