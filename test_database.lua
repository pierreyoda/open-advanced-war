-- Pausable clock test
a, b = PausableClock(true), PausableClock(false)
c = 0
while (c < 10e5) do -- sleep
c=c+1
end
io.write(a:getElapsedTime(), " ", b:getElapsedTime()) print()