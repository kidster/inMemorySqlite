#g++ test.cpp -lsqlite3 -lstdc++
CXX=g++
LIBFLAGS=-lsqlite3 -lstdc++

SRCS=test.cpp

all: test

test: 
	g++ $(SRCS) $(LIBFLAGS)

clean:
	rm a.out
