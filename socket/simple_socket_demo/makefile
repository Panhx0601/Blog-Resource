all: server client
server: server.cpp
	g++ $^ -o server
client: client.cpp
	g++ $^ -o client
clean:
	rm -f server client
