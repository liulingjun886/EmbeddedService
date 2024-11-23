PLUGS_INC = -I$(PLUGS_DIR)/BaseDevices

PLUGS = IACES.so testpcs.so testbcmu.so testmeter.so testbmu.so testliquidcooler.so

IACES.so:$(PLUGS_DIR)/BaseDevices/*.cpp $(PLUGS_DIR)/IACES/*.cpp
	$(HOST)g++ $(SOFLAGS) $^ -o $(OUTPUT_SO_DIR)/$@ $(INC) $(PLUGS_INC)
