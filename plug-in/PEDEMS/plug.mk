PLUGS_INC = -I$(PLUGS_DIR)/Devices

PLUGS = EMS.so SG1100UD-MV.so YT-320.so SC5000UD-MV-P3.so DGTest.so

EMS.so:$(PLUGS_DIR)/Devices/*.cpp $(PLUGS_DIR)/PEDEMS/*.cpp
	$(HOST)g++ $(SOFLAGS)  $^ -o $(OUTPUT_SO_DIR)/$@ $(INC) $(PLUGS_INC)
