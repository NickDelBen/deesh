#Executables to compile
all: binaries history

#Compiler to use for the make
cc=cc
#Directory for main binaries
BIN_DIR=bin
#Directory for plugin binaries
BIN_PLUGIN_DIR=bin/plugins

#Create the main binary directory if it does not exist
binaries:
	mkdir -p $(BIN_DIR)

#Compile the history plugin
history: src/plugins/history/history.h src/plugins/history/history_plugin.h src/plugins/history/linked_list/linked_list.h src/plugins/history/linked_list/list_node.h src/plugins/history/driver.c src/plugins/history/history.c src/plugins/history/history_plugin.c src/plugins/history/linked_list/linked_list.c src/plugins/history/linked_list/list_node.c
	#Create the plugin binary directory if it does not exis
	mkdir -p $(BIN_PLUGIN_DIR)
	$(cc) -o $(BIN_PLUGIN_DIR)/history src/plugins/history/driver.c src/plugins/history/history.c src/plugins/history/history_plugin.c src/plugins/history/linked_list/linked_list.c src/plugins/history/linked_list/list_node.c