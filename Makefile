LIBS_ROOT = ./lib
INC = -I./include -I./common -I./syscfg -I./devices -I./communicate
CFLAGS = -g -Werror -O0 -std=c++11
LDFLAGS = -Wl,--export-dynamic
SOFLAGS = -O0 -g -std=c++11 -fPIC -shared

PLUGS_DIR = ./plug-in/$(pj)
PLUGS = 

BUILDDIR = ./build

OUTPUT_SO_DIR = $(BUILDDIR)/$(pj)
INSTALL_SO_DIR = $(BUILDDIR)/plugs

ifeq ($(tg),arm)
	HOST = arm-linux-gnueabihf-
	LIBS_ROOT = ./lib_arm
endif

LIBS_DIR := -L$(LIBS_ROOT)/core -L$(LIBS_ROOT)/jsoncpp -L$(LIBS_ROOT)/modbus 
LIBS := -lcore -ljsoncpp -lmodbus -lpthread -ldl

-include $(PLUGS_DIR)/plug.mk

all:SmartEMS all_plugs install

SYSCFG_SRC = $(wildcard ./syscfg/*.cpp)
SYSCFG_OBJ = $(SYSCFG_SRC:.cpp=.o)

DEVICE_SRC = $(wildcard ./devices/*.cpp)
DEVICE_OBJ = $(DEVICE_SRC:.cpp=.o)

TOOLS_SRC = $(wildcard ./common/*.cpp)
TOOLS_OBJ = $(TOOLS_SRC:.cpp=.o)

COMM_SRC = $(wildcard ./communicate/*.cpp)
COMM_OBJ = $(COMM_SRC:.cpp=.o)

APP_SRC = ./main.cpp
APP_OBJ = $(APP_SRC:.cpp=.o)

SmartEMS:$(SYSCFG_OBJ) $(APP_OBJ) $(DEVICE_OBJ) $(TOOLS_OBJ) $(COMM_OBJ)
	$(HOST)g++ $(CFLAGS) $^ -o $(BUILDDIR)/$@ $(LIBS_DIR) $(LIBS) $(LDFLAGS)

%.o:%.cpp
	$(HOST)g++ -c $(CFLAGS) $< -o $@ $(INC)

%.so:$(PLUGS_DIR)/%/*.cpp
	$(HOST)g++ $(SOFLAGS)  $^ -o $(OUTPUT_SO_DIR)/$@ $(INC) $(PLUGS_INC)

all_plugs:check_dir $(PLUGS)

check_dir:
	test -d $(OUTPUT_SO_DIR) || mkdir -p $(OUTPUT_SO_DIR)

install:
	rm -f $(INSTALL_SO_DIR)/*
	cp $(OUTPUT_SO_DIR)/* $(INSTALL_SO_DIR)/ -f

clean:
	rm -f $(APP_OBJ) $(DEVICE_OBJ) $(TOOLS_OBJ) $(COMM_OBJ) $(SYSCFG_OBJ)

