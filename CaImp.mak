vpath %.cpp c:/cpp/canew c:/cpp/fortify
vpath %.hpp c:/cpp/canew c:/cpp/fortify
vpath %.h c:/cpp/canew c:/cpp/fortify
#CFLAGS = -O6 -Wall -Ic:/cpp/fortify -Ic:/cpp/canew -DGRAPHICS -DFORTIFY -fexternal-templates -DTEST
CFLAGS = -O6 -Wall -Ic:/cpp/fortify -Ic:/cpp/canew -DGRAPHICS -fexternal-templates 
%o : %cpp
	gcc $(CFLAGS) -c $<

O = mcaimp.o caimp.o cadis.o cabase.o r250.o bgi.o fortify.o 

L = -lgpp -lm -lgrx20 -lpc

gcaimp : $(O)
	gcc -o gcaimp $(O) $(L)
	stubify gcaimp
	del gcaimp

# DEPENDENCIES

mcaimp.o: mcaimp.cpp 

caimp.o: caimp.cpp caimp.hpp cadis.hpp cadis.cpp

cadis.o: cadis.cpp cadis.hpp cell.hpp smattpl.hpp

cabase.o: cabase.cpp cabase.hpp

r250.o: r250.cpp r250.h

bgi.o: bgi.cpp

fortify.o : fortify.cpp fortify.h ufortify.h

clean:
	del *.o
	del *.bak
