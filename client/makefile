CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: clean StompWCIClient 

StompWCIClient: bin/GameSummary.o bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/UserData.o bin/ClientInputHandler.o bin/StompServerHandler.o 
	g++ -o bin/StompWCIClient bin/ClientInputHandler.o bin/UserData.o  bin/ConnectionHandler.o bin/StompServerHandler.o  bin/StompClient.o bin/GameSummary.o $(LDFLAGS)
#
bin/GameSummary.o: src/GameSummary.cpp
	g++ $(CFLAGS) -o bin/GameSummary.o src/GameSummary.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp
#
bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp


bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp


bin/UserData.o: src/UserData.cpp
	g++ $(CFLAGS) -o bin/UserData.o src/UserData.cpp


bin/ClientInputHandler.o: src/ClientInputHandler.cpp
	g++ $(CFLAGS) -o bin/ClientInputHandler.o src/ClientInputHandler.cpp

bin/StompServerHandler.o: src/StompServerHandler.cpp
	g++ $(CFLAGS) -o bin/StompServerHandler.o src/StompServerHandler.cpp


.PHONY: clean

clean:
	rm -f bin/*