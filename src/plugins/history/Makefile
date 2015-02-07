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
printer: history_common.h printer/history_printer.h printer/stream_printer.h printer_driver.c printer/history_printer.c printer/stream_printer.c	
	#Copy the plugin configuration to the output directory
	cp history_printer.dshp $(OUT_DIR)/
	#Compile the binaries for the history printer
	$(cc) -o $(PLUG_DIR)/print_history printer_driver.c printer/history_printer.c printer/stream_printer.c

#Make the history saver binaries
saver: history_common.h saver/history_saver.h saver_driver.c saver/history_saver.c
	#Copy the plugin configuration to the output directory
	cp history_saver.dshp $(OUT_DIR)/
	#Compile the binaries for the history saver
	$(cc) -o $(PLUG_DIR)/save_history saver_driver.c saver/history_saver.c