# This script creates an object file (.o) of the ADP Framework code, 
# and then creates a static library (.a) of the ADP framework which
# can be included in other projects (just include  ADPFrameworkLib.a
# in the g++ arg list)

rm obj/*
rm lib/*
g++ -I ./include -c src/*
mv *.o obj/
ar rvs libADPFramework.a obj/*
mv libADPFramework.a lib/