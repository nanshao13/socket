cc=g++

.PHONY:all
all:udpClient udpServer
udpClient:udpClient.cc
	$(cc) -o $@ $^ -pthread -std=c++11
udpServer:udpServer.cc
	$(cc) -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -r udpClient udpServer
