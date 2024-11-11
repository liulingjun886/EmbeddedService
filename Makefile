
LIBS_ROOT = ./lib
INC = -I./include -I./common -I./syscfg -I./devices -I./communicate -I./EnergySchedule
CFLAGS = -g -Werror -O0 -std=c++11
LDFLAGS = -Wl,--export-dynamic
SOFLAGS = -O0 -g -std=c++11 -fPIC -shared
OUTPUT_SO_DIR = ./plug_in

ifeq ($(tg),arm)
HOST = arm-linux-gnueabihf-
LIB_DIR = ./lib_arm
endif

#LIBS := -L$(LIB_DIR)/core -lcore -L$(LIB_DIR)/jsoncpp -ljsoncpp -L$(LIB_DIR)/modbus -lmodbus -lpthread -ldl
LIBS_DIR := -L$(LIBS_ROOT)/core -L$(LIBS_ROOT)/jsoncpp -L$(LIBS_ROOT)/modbus 
LIBS := -lcore -ljsoncpp -lmodbus -lpthread -ldl

all:SmartEMS IACES.so testpcs.so testbcmu.so testmeter.so testbmu.so testliquidcooler.so

SYSCFG_SRC = $(wildcard ./syscfg/*.cpp)
SYSCFG_OBJ = $(SYSCFG_SRC:.cpp=.o)

DEVICE_SRC = $(wildcard ./devices/*.cpp)
DEVICE_OBJ = $(DEVICE_SRC:.cpp=.o)

TOOLS_SRC = $(wildcard ./common/*.cpp)
TOOLS_OBJ = $(TOOLS_SRC:.cpp=.o)

COMM_SRC = $(wildcard ./communicate/*.cpp)
COMM_OBJ = $(COMM_SRC:.cpp=.o)

ES_SRC = $(wildcard ./EnergySchedule/*.cpp)
ES_OBJ = $(ES_SRC:.cpp=.o)

APP_SRC = ./main.cpp
APP_OBJ = $(APP_SRC:.cpp=.o)

SmartEMS:$(SYSCFG_OBJ) $(APP_OBJ) $(DEVICE_OBJ) $(TOOLS_OBJ) $(COMM_OBJ) $(ES_OBJ)
	$(HOST)g++ $(CFLAGS) $^ -o $@ $(LIBS_DIR) $(LIBS) $(LDFLAGS)

%.o:%.cpp
	$(HOST)g++ -c $(CFLAGS) $< -o $@ $(INC)

clean:
	rm -f $(APP_OBJ) $(DEVICE_OBJ) $(TOOLS_OBJ) $(COMM_OBJ) $(SYSCFG_OBJ) $(ES_OBJ)

%.so:./plug-in/%/*.cpp 
	$(HOST)g++ $(SOFLAGS)  $^ -o $(OUTPUT_SO_DIR)/$@ $(INC)
