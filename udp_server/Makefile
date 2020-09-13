.PHONY:all 
all:udp-client udp-server 	

udp-client:udp-client.cc 
	g++ -o $@ $^
udp-server:udp-server.cc 
	g++ -o $@ $^

.PHONY:clean
clean:
	rm -f udp-client udp-server 



