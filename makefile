CC = g++

all: fragListTest ListSpeedTest

fragListTest:
	g++ fragListTest.cpp -std=c++0x -o fragTest

ListSpeedTest:
	$(CC) SpeedTests.cpp -std=c++0x -o speedTest 
