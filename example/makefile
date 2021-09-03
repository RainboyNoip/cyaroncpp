CXX=g++
CFLAGS= -D LOCALTEST -I../include -I../include/core -g

SRC=$(wildcard *.cpp)
TARGET=$(patsubst %.cpp,%,$(SRC))

all: $(TARGET)
	@echo done

% : %.cpp
	$(CXX) $(CFLAGS) -o $@.out $<

clean:
	rm -rf $(TARGET)
