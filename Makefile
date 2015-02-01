#Executables to compile
all: binaries plugin-history

#Compiler to use for the make
cc=cc
#Directory for main binaries
BIN_DIR=bin
#Directory for plugin binaries
BIN_PLUGIN_DIR=bin/plugins

#Compile the main application binaries
binaries: src/driver.c src/plugin_controller.h src/plugin_controller.c src/plugin.h src/plugin.c src/linked_list/linked_list.h src/linked_list/linked_list.c src/linked_list/list_node.h src/linked_list/list_node.c
	#Create the main binary directory if it does not exist
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_PLUGIN_DIR)
	$(cc) -o $(BIN_DIR)/deesh src/driver.c src/plugin_controller.c src/plugin.c src/linked_list/linked_list.c src/linked_list/list_node.c

#Compile the history plugin
plugin-history: src/plugins/history/history.h src/plugins/history/history_plugin.h src/plugins/history/linked_list/linked_list.h src/plugins/history/linked_list/list_node.h src/plugins/history/driver.c src/plugins/history/history.c src/plugins/history/history_plugin.c src/plugins/history/linked_list/linked_list.c src/plugins/history/linked_list/list_node.c
	#Copy the plugin configuration file to the plugin directory
	cp -f src/plugins/history/history.dshp $(BIN_PLUGIN_DIR)/history.dshp
	#Create the history directory if it does not exis
	mkdir -p $(BIN_PLUGIN_DIR)/history
	$(cc) -o $(BIN_PLUGIN_DIR)/history/history src/plugins/history/driver.c src/plugins/history/history.c src/plugins/history/history_plugin.c src/plugins/history/linked_list/linked_list.c src/plugins/history/linked_list/list_node.c