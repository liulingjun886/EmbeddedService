PLUGS_INC = -I$(PLUGS_DIR)/Devices

PLUGS = EMS.so PV.so ess.so de.so

EMS.so:$(PLUGS_DIR)/Devices/*.cpp
	$(HOST)g++ $(SOFLAGS)  $^ -o $(OUTPUT_SO_DIR)/$@ $(INC) $(PLUGS_INC)
