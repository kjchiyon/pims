// main.cpp
// build options: -Wextra -Wall -std=c++11

#include <iostream>
#include "adt.hpp"
#include "cmd.hpp"

// prototypes
void display_welcome_message(void);
void display_help_menu(void);
bool login(void);
bool prompt(void);
bool match_command(std::string command);

// inventory
Inventory inventory;

// MAIN
int main() {
    // welcome
    display_welcome_message();
    // login
    if (!login()) return 0;
    // load data
    load_inventory(inventory, "pims.inventory");
    // program loop
    display_help_menu();
    bool looping = true;
    while (looping) {
        looping = prompt();
    }
    // end
    std::cout << "\nExiting program.\n\n\n";
    return 0;
}

/// display welcome message
void display_welcome_message() {
    // welcome message
    std::cout << "\n"
        << "++=======================================================++\n"
        << "||                                                       ||\n"
        << "||                      Welcome to                       ||\n"
        << "||                                                       ||\n"
        << "||    ooooooooo.   ooooo ooo        ooooo  .oooooo..o    ||\n"
        << "||    `888   `Y88. `2A2' `88.       .888' d8P'    `Y8    ||\n"
        << "||     888   .d88'  CKJ   888b     d'888  Y88bo.         ||\n"
        << "||     888ooo88P'   KXX   8 Y88. .P  888   `*Y8888o.     ||\n"
        << "||     888          CZC   8  `888'   888       `*Y88b    ||\n"
        << "||     888          HAE   8    Y     888  oo     .d8P    ||\n"
        << "||    o888o        o888o o8o        o888o 8'*88888P'     ||\n"
        << "||                                                       ||\n"
        << "||       [ Pharmacy Inventory Management System ]        ||\n"
        << "||          developed by TDS6213 Lab2A Group 2           ||\n"
        << "||                                                       ||\n"
        << "++=======================================================++";
}

/// display help menu and info
void display_help_menu() {
    // help menu and info
    std::cout << "\n\n"
        << "============================[ Help Menu ]==============================\n\n"
        << "How to use:\n"
        << "    This application is command based,\n"
        << "    run actions by typing commands in the console.\n"
        << "    The commands are self explanatory.\n"
        << "    More intructions will appear after entering a command.\n\n"
        << "Commands list:\n"
        << "- additem <item id> <item description>\n"
        << "- addbatch <item id> <stock amount> <expiry date> <batch description>\n"
        << "- edititemdesc <item id> <new item description>\n"
        << "- editbatchdesc <item id> <batch id> <new batch description>\n"
        << "- editbatchstock <item id> <batch id> <transaction type> <change amount>\n"
        << "- deleteitem <item id>\n"
        << "- deletebatch <item id> <batch id>\n"
        << "- listitem\n"
        << "- listbatch <item id>\n"
        << "- listtransaction\n"
        << "- viewitem <item id>\n"
        << "- viewbatch <item id> <batch id>\n"
        << "- report\n"
        << "- save\n"
        << "- load\n"
        << "- exit/end\n"
        << "Commands can be shortened, for example:\n"
        << "- additem -> ai\n"
        << "- deleteitem -> di\n"
        << "- viewbatch -> vb\n\n"
        << "===========================[ End of Menu ]=============================";
}

/// simple login process
bool login(void) {
    const std::string password = "admin123";
    // max 3 attempts
    std::cout << "\n";
    for (int i = 0; i < 3; ++i) {
        std::string attempt = get_input("\n\nEnter password: ");
        if (attempt == password) return true;
        else std::cout << "Wrong password, you have " << 2 - i << " attempts left";
    }
    std::cout << "\n\nAuthentication failed, exiting application.\n";
    return false;
}

/// get user command input
bool prompt(void) {
    // prompt user to enter a command
    std::string command = get_input("\n\n\nEnter a command, type \"help\" for more info.");
    return match_command(command);
}

/// match string to commands, also determines if user manually ended the program
bool match_command(std::string command) {
    // if statements to match string to commands
    if (command == "exit" || command == "end" || command == "e") {
        return false; // kill program loop
    } else
    if (command == "help" || command == "h") {
        display_help_menu();
    } else
    if (command == "additem" || command == "ai") {
        command_add_item(inventory);
    } else
    if (command == "addbatch" || command == "ab") {
        command_add_batch(inventory);
    } else
    if (command == "edititemdesc" || command == "eid") {
        command_edit_item_desc(inventory);
    } else
    if (command == "editbatchdesc" || command == "ebd") {
        command_edit_batch_desc(inventory);
    } else
    if (command == "editbatchstock" || command == "ebs") {
        command_edit_batch_stock(inventory);
    } else
    if (command == "deleteitem" || command == "di") {
        command_delete_item(inventory);
    } else
    if (command == "deletebatch" || command == "db") {
        command_delete_batch(inventory);
    } else
    if (command == "listitem" || command == "li") {
        command_list_items(inventory);
    } else
    if (command == "listbatch" || command == "lb") {
        command_list_batches(inventory);
    } else
    if (command == "listtransaction" || command == "lt") {
        command_list_transactions(inventory);
    } else
    if (command == "viewitem" || command == "vi") {
        command_view_item(inventory);
    } else
    if (command == "viewbatch" || command == "vb") {
        command_view_batch(inventory);
    } else
    if (command == "report" || command == "r") {
        command_report(inventory);
    } else
    if (command == "save" || command == "s") {
        save_inventory(inventory, "pims.inventory");
    } else
    if (command == "load" || command == "l") {
        load_inventory(inventory, "pims.inventory");
    } else {
        // unknown command input
        std::cout << "\nUnknown command: " << command;
    }
    return true;
}
