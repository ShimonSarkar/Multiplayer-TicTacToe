CC  = g++
CXX = g++

INCLUDES = 

CFLAGS   = -std=c++11 -g -Wall $(INCLUDES)
CXXFLAGS = -std=c++11 -g -Wall $(INCLUDES)

LDFLAGS = 
LDLIBS  = 

.PHONY: default
default: server client

server: server.o
	$(CXX) $(LDFLAGS) -o server server.o $(LDLIBS)

client: client.o
	$(CXX) $(LDFLAGS) -o client client.o $(LDLIBS)

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp -o server.o

client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp -o client.o

.PHONY: clean
clean:
	rm -f *.o *~ a.out core server client

.PHONY: clean_objs
clean_objs:
	rm -f *.o *~

.PHONY: run_server
run_server: server
	./server

.PHONY: run_client
run_client: client
	./client

.PHONY: all
all: clean default

.PHONY: all_clean
all_clean: clean default clean_objs
