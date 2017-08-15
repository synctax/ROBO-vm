CFLAGS=-std=c++11

all: robo-assemble

install: robo-assemble
	cp robo-assemble /usr/local/bin/robo-assemble

robo-assemble: lexer.o main.o
	$(CXX) $(CFLAGS) lexer.o main.o -o robo-assemble

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

lexer.o: Lexer.h Lexer.cpp
	$(CXX) $(CFLAGS) -c Lexer.cpp

clean:
	rm -f *.o robo-assemble
