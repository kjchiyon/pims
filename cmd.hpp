// cmd.hpp
// COMMANDS

#ifndef CMD_HPP
#define CMD_HPP

#include "adt.hpp"

// input
std::string get_input(std::string message = "");

// commands
void command_add_item(Inventory& inventory);
void command_add_batch(Inventory& inventory);
void command_edit_item_desc(Inventory& inventory);
void command_edit_batch_stock(Inventory& inventory);
void command_edit_batch_desc(Inventory& inventory);
void command_delete_item(Inventory& inventory);
void command_delete_batch(Inventory& inventory);
void command_list_items(Inventory& inventory);
void command_list_batches(Inventory& inventory);
void command_list_transactions(Inventory& inventory);
void command_view_item(Inventory& inventory);
void command_view_batch(Inventory& inventory);
void command_report(Inventory& inventory);

// file handling
void save_inventory(Inventory& inventory, const std::string& filename);
void load_inventory(Inventory& inventory, const std::string& filename);

#endif // CMD_HPP
