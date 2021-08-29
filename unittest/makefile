CXX=g++
CFLAGS="-lboost_system"

SRC=$(wildcard *.hpp)
TARGET=$(patsubst %.cpp,%,$(SRC))

all: $(SRC) main.cpp std
	g++ -g -std=c++17 -o test main.cpp -I../include -I../include/core
	./test

std: std.cpp
	g++ -g -std=c++17 -o std std.cpp

clean:
	rm -rf $(TARGET)
