main: main.o bm.o
	g++ -o main main.o bm.o

learn: learn.o bm.o
	g++ -o learn learn.o bm.o

main.o: main.cpp bm.h
	g++ -c main.cpp

learn.o: learn.cpp bm.h
	g++ -c learn.cpp

bm.o: bm.cpp bm.h
	g++ -c bm.cpp

clean:
	rm main.o bm.o