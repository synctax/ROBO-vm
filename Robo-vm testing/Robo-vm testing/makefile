CFLAGS=-std=c++11

all: robo-boot

install: robo-boot
	cp robo-boot /usr/local/bin/robo-boot

robo-boot: main.o monitor.o vm.o
	$(CXX) $(CFLAGS) vm.o monitor.o main.o -o robo-boot -lncurses

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

monitor.o: Monitor.h Monitor.cpp
	$(CXX) $(CFLAGS) -c	Monitor.cpp

vm.o: VM.h VM.cpp
	$(CXX) $(CFLAGS) -c VM.cpp

clean:
	rm -f *.o robo-boot
