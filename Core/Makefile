ifeq ($(tg),arm)
HOST=arm-linux-gnueabihf-
endif

ROOT_DIR = ./
INCLUDEPATH = -I./include
SRCPATH = ./src
SRC = $(wildcard ./src/*.cpp)

LIBOBJ := $(SRC:.cpp=.o)

./src/%.o : ./src/%.cpp
	$(HOST)g++ -c -g $< -o $@ $(INCLUDEPATH) -std=c++11

libcore.a : $(LIBOBJ)
	$(HOST)ar -rcs $@ $^

test : ./Test.cpp
	$(HOST)g++ -g $< -o $@ $(INCLUDEPATH) -L. -lcore -lpthread

clean:
	rm -f ./src/*.o
