cc=g++

.PHONY:all
all:udpServer udpClient
udpServer:udpServer.cpp
	$(cc) -o $@ $^ -std=c++11
udpClient:udpClient.cpp
	$(cc) -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f udpServer udpClient
