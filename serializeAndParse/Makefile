cc=g++
.PHONY:all
all:calClient calServer
calClient:calClient.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp
calServer:calServer.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp -lpthread 
.PHONY:clean
clean:
	rm -f calClient calServer