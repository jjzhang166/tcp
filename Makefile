.PHONY:all
all:tcp_client tcp_server

tcp_server:tcp_server.cpp
	g++ -o $@ $^ -lpthread -D_V3_
tcp_client:tcp_client.cpp
	g++ -o $@ $^

.PHONY:clean
clean:
	rm -f tcp_client tcp_server
