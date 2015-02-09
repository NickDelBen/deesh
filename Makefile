#Executables to compile
all: clean prep binaries plugin-history

#Compiler to use for the make
cc=cc
#Directory for main binaries
BIN_DIR=bin
#Directory for plugin binaries
BIN_PLUGIN_DIR=bin/plugins
#History plugin source directory
HISTORY_PLUG_SRC=src/plugins/history

#Remove any previously built files
clean:
	#Remove any binaries from this output directory
	rm -rf $(BIN_DIR)
	#Remove any binaries the history plugin has made
	make -C $(HISTORY_PLUG_SRC) clean

#Create the directory structure required for output
prep:
	#Create output directory
	mkdir -p $(BIN_DIR)
	#Create the plugin directory
	mkdir -p $(BIN_PLUGIN_DIR)

#Compile the main application binaries
binaries: src/deesh.h src/descriptors.h src/executor.h src/plugin.h src/plugin_controller.h src/commands/command.h src/commands/parsed_command.h src/linked_list/linked_list.h src/linked_list/list_node.h src/driver.c src/deesh.c src/descriptors.c src/executor.c src/plugin.c src/plugin_controller.c src/commands/command.c src/commands/parsed_command.c src/linked_list/linked_list.c src/linked_list/list_node.c
	#Compile the shell binaries
	$(cc) -o $(BIN_DIR)/deesh src/driver.c src/deesh.c src/descriptors.c src/executor.c src/plugin.c src/plugin_controller.c src/commands/command.c src/commands/parsed_command.c src/linked_list/linked_list.c src/linked_list/list_node.c

#Compile the history plugin binaries
plugin-history:
	#Execute the history makefile
	make -C $(HISTORY_PLUG_SRC)
	#Move the generated files from the history plugin binaries to the plugin directory
	cp -rf $(HISTORY_PLUG_SRC)/bin/* $(BIN_PLUGIN_DIR)/
	#Clean the output directory for the history plugin
	make -C $(HISTORY_PLUG_SRC) clean