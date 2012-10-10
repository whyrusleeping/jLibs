CC=g++

all: fragListTest ListSpeedTest vectorTest matrixTest

fragListTest:
	g++ fragListTest.cpp -std=c++0x -o fragTest

ListSpeedTest:
	$(CC) SpeedTests.cpp -std=c++0x -o speedTest 

vectorTest:
	$(CC) vectorTest.cpp -std=c++0x -o vecTest

matrixTest:
	$(CC) matrixTest.cpp -std=c++0x -o matrixTest
