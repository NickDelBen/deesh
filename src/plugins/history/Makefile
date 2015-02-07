#Executables to compile
all: clean prep printer saver

#Compiler to use for the make
cc=cc
#Directory for main binaries
OUT_DIR=bin
#Folder for this plugin
PLUG_DIR=bin/history

#Removes any previously made stuffs
clean:
	#Remove all output
	rm -rf $(OUT_DIR)

#Create the directory tree
prep:
	#Create the output directory if it does not exist
	mkdir -p $(OUT_DIR)
	#Create the plugin directory if it deos not exist
	mkdir -p $(PLUG_DIR)

#Make the history printer binaries
printer: src/history_common.h src/printer/history_printer.h src/printer/stream_printer.h src/printer_driver.c src/printer/history_printer.c src/printer/stream_printer.c	
	#Copy the plugin configuration to the output directory
	cp cfg/history_printer.dshp $(OUT_DIR)/
	#Compile the binaries for the history printer
	$(cc) -o $(PLUG_DIR)/print_history src/printer_driver.c src/printer/history_printer.c src/printer/stream_printer.c

#Make the history saver binaries
saver: src/history_common.h src/saver/history_saver.h src/saver_driver.c src/saver/history_saver.c
	#Copy the plugin configuration to the output directory
	cp cfg/history_saver.dshp $(OUT_DIR)/
	#Compile the binaries for the history saver
	$(cc) -o $(PLUG_DIR)/save_history src/saver_driver.c src/saver/history_saver.c