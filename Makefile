MAIN = Metro-planning.cpp
HEAD  = metro.h
Metroplan: main.o trave.o findp.o findl.o lio.o metro.o
	g++ -o $@ $+

main.o: $(HEAD) $(MAIN)
	g++ -c $(MAIN) -o $@

trave.o: $(HEAD) Trave_metro.cpp
	g++ -c Trave_metro.cpp -o $@

findp.o: $(HEAD) Findpath.cpp
	g++ -c Findpath.cpp -o $@

findl.o: $(HEAD) Findline.cpp
	g++ -c Findline.cpp -o $@

lio.o: $(HEAD) Line_io.cpp
	g++ -c Line_io.cpp -o $@

metro.o: $(HEAD) Metro.cpp
	g++ -c Metro.cpp -o $@
