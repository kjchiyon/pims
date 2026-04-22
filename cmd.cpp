// cmd.cpp
// COMMANDS
#include <iostream>
#include <iomanip>  // for cout manipulation
#include <fstream>  // for reading and writing file
#include <string>   // for stoi
#include "cmd.hpp"
#include "data.hpp"
#include "adt.hpp"

// ---------------- Input ----------------

std::string get_input(std::string message) {
    std::cout << message << "\n> ";
    std::string answer;
    std::cin >> answer;
    return answer;
}

// ---------------- Commands ----------------

void command_add_item(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    if (inventory.find_item(item_id) != -1) {
        std::cout << "\nItem already exists.";
        return;
    }
    // get item desc
    std::string desc;
    std::cout << "\nEnter item description:\n> ";
    std::cin.ignore();
    std::getline(std::cin, desc);
    // add item
    inventory.add_item(item_id, desc);
    std::cout << "\nItem added successfully.";
}

void command_add_batch(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    int item_index = inventory.find_item(item_id);
    if (item_index == -1) {
        std::cout << "\nItem not found.";
        return;
    }
    // get batch stock
    int stock;
    try {
        stock = std::stoi(get_input("\nEnter stock amount"));
    } catch (std::invalid_argument const& ex) {
        std::cout << "\nInvalid input, not a number.";
        return;
    }
    // get expiry date
    std::cout << "\nProvide the expiry date separately.";
    int d, m, y;
    try {
        d = std::stoi(get_input("\nEnter the day of expiry date:"));
        m = std::stoi(get_input("\nEnter the month of expiry date:"));
        y = std::stoi(get_input("\nEnter the year of expiry date:"));
    } catch (std::invalid_argument const& ex) {
        std::cout << "\nInvalid input, not a number.";
        return;
    }
    Date expiry(d, m, y);
    if (!expiry.is_valid()) {
        std::cout << "\nInvalid input, not a real date.";
        return;
    }
    // get desc
    std::string desc;
    std::cout << "\nEnter batch description:\n> ";
    std::cin.ignore();
    std::getline(std::cin, desc);
    // add batch
    int batch_index = inventory.get_item(item_index)->get_batches_size();
    inventory.get_item(item_index)->add_batch(stock, get_today(), expiry, desc);
    std::string batch_id = inventory.get_item(item_index)->get_batch(batch_index)->get_batch_id();
    inventory.add_transaction(item_id, batch_id, get_today(), TransactionType::RESTOCK, stock);
    std::cout << "\nNew batch added successfully.";
}

void command_edit_item_desc(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    int item_index = inventory.find_item(item_id);
    if (item_index == -1) {
        std::cout << "\nItem not found.";
        return;
    }
    // get new desc
    std::string desc;
    std::cout << "\nEnter new description:\n> ";
    std::cin.ignore();
    std::getline(std::cin, desc);
    // set new desc
    inventory.get_item(item_index)->set_item_description(desc);
    std::cout << "\nItem description updated.";
}

void command_edit_batch_desc(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    int item_index = inventory.find_item(item_id);
    if (item_index == -1) {
        std::cout << "\nItem not found.";
        return;
    }
    // get batch id
    std::string batch_id = get_input("\nEnter batch id (must not include whitespace):");
    int batch_index = inventory.get_item(item_index)->find_batch(batch_id);
    if (batch_index == -1) {
        std::cout << "\nBatch not found.\n";
        return;
    }
    // get new desc
    std::string desc;
    std::cout << "\nEnter new batch description:\n> ";
    std::cin.ignore();
    std::getline(std::cin, desc);
    // set new desc
    inventory.get_item(item_index)->get_batch(batch_index)->set_batch_description(desc);
    std::cout << "\nBatch description updated.";
}

void command_edit_batch_stock(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    int item_index = inventory.find_item(item_id);
    if (item_index == -1) {
        std::cout << "\nItem not found.";
        return;
    }
    // get batch id
    std::string batch_id = get_input("\nEnter batch id (must not include whitespace):");
    int batch_index = inventory.get_item(item_index)->find_batch(batch_id);
    if (batch_index == -1) {
        std::cout << "\nBatch not found.";
        return;
    }
    // get transaction type
    int type;
    std::cout << "\nEnter transaction type (number 1-4):"
        << "\n1 = SOLD\n2 = REFUND\n3 = RESTOCK\n4 = DISCARD";
    try {
        type = std::stoi(get_input());
    } catch (std::invalid_argument const& ex) {
        std::cout << "\nInvalid input, not a number.";
        return;
    }
    if (type < 1 || type > 4) {
        std::cout << "\nInvalid input, number not between 1 and 4.";
        return;
    }
    // get update amount and update
    int amount;
    if (type == 1 || type == 4) { // decrease
        try {
            amount = std::stoi(get_input("\nEnter sold/discard amount:"));
        } catch (std::invalid_argument const& ex) {
            std::cout << "\nInvalid input, not a number.";
            return;
        }
        int after = inventory.get_item(item_index)->get_batch(batch_index)->get_batch_stock() - amount;
        if (after < 0) {
            std::cout << "\nInvalid input, amount entered is more than remaining stock.";
            return;
        }
        inventory.add_transaction(item_id, batch_id, get_today(), static_cast<TransactionType>(type-1), amount);
        inventory.get_item(item_index)->get_batch(batch_index)->set_batch_stock(after);
        std::cout << "\nBatch stock updated.";
    }
    if (type == 2 || type == 3) { // increase
        try {
            amount = std::stoi(get_input("\nEnter refund/restock amount"));
        } catch (std::invalid_argument const& ex) {
            std::cout << "\nInvalid input, not a number.";
            return;
        }
        int after = inventory.get_item(item_index)->get_batch(batch_index)->get_batch_stock() + amount;
        inventory.add_transaction(item_id, batch_id, get_today(), static_cast<TransactionType>(type-1), amount);
        inventory.get_item(item_index)->get_batch(batch_index)->set_batch_stock(after);
        std::cout << "\nBatch stock updated.";
    }
}

void command_delete_item(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    int item_index = inventory.find_item(item_id);
    if (item_index == -1) {
        std::cout << "\nItem not found.";
        return;
    }
    // delete item
    inventory.add_transaction(item_id, "DELETEITEM", get_today(), TransactionType::DISCARD, inventory.get_item(item_index)->get_total_quantity());
    inventory.remove_item(item_index);
    std::cout << "\nItem deleted.";
}

void command_delete_batch(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    int item_index = inventory.find_item(item_id);
    if (item_index == -1) {
        std::cout << "\nItem not found.";
        return;
    }
    // get batch id
    std::string batch_id = get_input("\nEnter batch id (must not include whitespace):");
    int batch_index = inventory.get_item(item_index)->find_batch(batch_id);
    if (batch_index == -1) {
        std::cout << "\nBatch not found.";
        return;
    }
    // delete batch
    inventory.add_transaction(item_id, batch_id, get_today(), TransactionType::DISCARD, inventory.get_item(item_index)->get_batch(batch_index)->get_batch_stock());
    inventory.get_item(item_index)->remove_batch(batch_index);
    std::cout << "\nBatch deleted.";
}

void command_list_items(Inventory& inventory) {
    // sort item_id in alphabetical ascending order using selection sort
    int n = inventory.get_items_size();
    Item* sorted_items[n];
    // copy LinkedList items into sorted_items array
    for (int i = 0; i < n; ++i) {
        sorted_items[i] = inventory.get_item(i);
    }
    // selection sort
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (sorted_items[j]->get_item_id() < sorted_items[min_idx]->get_item_id())
                min_idx = j;
        }
        std::swap(sorted_items[i], sorted_items[min_idx]);
    }
    // list all items in inventory
    std::cout << "\nItems are listed below as item ID (ascending) and total stock quantity:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "\n"
            << sorted_items[i]->get_item_id() << " - quantity: "
            << sorted_items[i]->get_total_quantity();
    }
}

void command_list_batches(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    int item_index = inventory.find_item(item_id);
    if (item_index == -1) {
        std::cout << "\nItem not found.";
        return;
    }
    // list all batches
    std::cout << "\n\nBatches are listed below as batch ID, date added (descending) and description:\n";
    for (int i = inventory.get_item(item_index)->get_batches_size() - 1; i >= 0; --i) {
        Batch* batch = inventory.get_item(item_index)->get_batch(i);
        Date added = batch->get_added_date();
        std::cout << "\n"
            << batch->get_batch_id()
            << " - date added: " << added.get_day() << "-"
            << added.get_month() << "-"
            << added.get_year() << ", desc: "
            << batch->get_batch_description();
    }
}

void command_list_transactions(Inventory& inventory) {
    // list all past transactions
    std::cout << "\nTransactions are listed below as transaction ID, type,\nitem id, batch id, transaction amount and date added (ascending):\n";
    for (int i = 0; i < inventory.get_transactions_size(); ++i) {
        Transaction* tr = inventory.get_transaction(i);
        if (tr) {
            std::cout << "\n" << tr->get_transaction_id() << " - Type: ";
            switch (tr->get_transaction_type()) {
            case TransactionType::SOLD:
                std::cout << "SOLD";
                break;
            case TransactionType::REFUND:
                std::cout << "REFUND";
                break;
            case TransactionType::RESTOCK:
                std::cout << "RESTOCK";
                break;
            case TransactionType::DISCARD:
                std::cout << "DISCARD";
                break;
            }
            std::cout << ", Item: " << tr->get_item_id()
                << ", Batch: " << tr->get_batch_id()
                << ", Change: " << tr->get_stock_change()
                << ", Date: " << tr->get_transaction_date().get_day() << "-"
                << tr->get_transaction_date().get_month() << "-"
                << tr->get_transaction_date().get_year();
        }
    }
}

void command_view_item(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    int item_index = inventory.find_item(item_id);
    if (item_index == -1) {
        std::cout << "\nItem not found.";
        return;
    }
    // print item info
    Item* item = inventory.get_item(item_index);
    std::cout << "\nItem ID: " << item->get_item_id() << "\n"
        << "Description: " << item->get_item_description() << "\n"
        << "Total stock: " << item->get_total_quantity() << "\n"
        << "Expired stock: " << item->get_expired_quantity() << "\n";
}

void command_view_batch(Inventory& inventory) {
    // get item id
    std::string item_id = get_input("\nEnter item id (must not include whitespace):");
    int item_index = inventory.find_item(item_id);
    if (item_index == -1) {
        std::cout << "\nItem not found.";
        return;
    }
    // get batch id
    std::string batch_id;
    std::cout << "\nEnter batch id\n> ";
    std::cin >> batch_id;
    int batch_index = inventory.get_item(item_index)->find_batch(batch_id);
    if (batch_index == -1) {
        std::cout << "\nBatch not found.";
        return;
    }
    // print batch info
    Batch* batch = inventory.get_item(item_index)->get_batch(batch_index);
    Date added = batch->get_added_date();
    Date expiry = batch->get_expiry_date();
    std::cout << "\nBatch ID: " << batch_id << "\n"
        << "Description: " << batch->get_batch_description() << "\n"
        << "Added: " << added.get_day() << "-" << added.get_month() << "-" << added.get_year() << "\n"
        << "Expiry: " << expiry.get_day() << "-" << expiry.get_month() << "-" << expiry.get_year() << "\n"
        << "Stock: " << batch->get_batch_stock() << "\n";
}

void command_report(Inventory& inventory) {
    // generate inventory report
    std::cout << "\nInventory Report:\n";
    // process all items
    for (int i = 0; i < inventory.get_items_size(); ++i) {
        // get transactions
        Item* item = inventory.get_item(i);
        int sold_quantity = 0;
        int refund_quantity = 0;
        for (int j = 0; j < inventory.get_transactions_size(); ++j) {
            Transaction* tx = inventory.get_transaction(j);
            if (tx->get_item_id() == item->get_item_id()) {
                if (tx->get_transaction_type() == SOLD) {
                    sold_quantity += tx->get_stock_change();
                } else if (tx->get_transaction_type() == REFUND) {
                    refund_quantity += tx->get_stock_change();
                }
            }
        }
        // print item report
        std::cout << "- " << item->get_item_id()
            << " | Sold: " << sold_quantity
            << " | Refund: " << refund_quantity
            << " | Batches: " << item->get_batches_size()
            << " | Total: " << item->get_total_quantity()
            << " | Expired: " << item->get_expired_quantity()
            << "\n";
    }
}

// ---------------- File Handling ----------------

void save_inventory(Inventory& inventory, const std::string& filename) {
    // open file
    std::ofstream ofile(filename);
    if (!ofile) {
        std::cerr << "\nError opening file for saving.";
        return;
    }
    // save items
    ofile << inventory.get_items_size() << "\n";
    for (int i = 0; i < inventory.get_items_size(); ++i) {
        Item* item = inventory.get_item(i);
        ofile << item->get_item_id() << " "
            << item->get_batch_record() << " "
            << item->get_batches_size() << "\n"
            << item->get_item_description() << "\n";
        // save batches
        for (int j = 0; j < item->get_batches_size(); ++j) {
            Batch* batch = item->get_batch(j);
            Date added  = batch->get_added_date();
            Date expiry = batch->get_expiry_date();
            ofile << batch->get_batch_id() << " "
                << added.get_day()  << " " << added.get_month()  << " " << added.get_year()  << " "
                << expiry.get_day() << " " << expiry.get_month() << " " << expiry.get_year() << " "
                << batch->get_batch_stock() << "\n"
                << batch->get_batch_description() << "\n";
        }
    }
    // save transactions
    ofile << inventory.get_transaction_record() << " " << inventory.get_transactions_size() << "\n";
    for (int i = 0; i < inventory.get_transactions_size(); ++i) {
        Transaction* transaction = inventory.get_transaction(i);
        Date date = transaction->get_transaction_date();
        ofile << transaction->get_transaction_id() << " "
            << transaction->get_item_id() << " "
            << transaction->get_batch_id() << " "
            << date.get_day() << " " << date.get_month() << " " << date.get_year() << " "
            << (int)transaction->get_transaction_type() << " "
            << transaction->get_stock_change() << "\n";
    }
    // close file
    ofile.close();
    std::cout << "\nInventory saved to: " << filename;
}

void load_inventory(Inventory& inventory, const std::string& filename) {
    // open file
    std::ifstream ifile(filename);
    if (!ifile) {
        std::cerr << "\nError opening file: " << filename;
        return;
    }
    // clear inventory
    Inventory new_inv;
    inventory = new_inv;
    // read items
    int items_size;
    ifile >> items_size;
    for (int i = 0; i < items_size; ++i) {
        // get item info
        std::string item_id, item_desc;
        int batch_record, batches_size;
        ifile >> item_id >> batch_record >> batches_size;
        std::getline(ifile, item_desc); // discard newline
        std::getline(ifile, item_desc);
        // add item
        inventory.add_item(item_id, item_desc, batch_record);
        // read batches
        for ( int j = 0; j < batches_size; ++j) {
            // get batch info
            std::string batch_id, batch_desc;
            int ad, am, ay, ed, em, ey, stock;
            ifile >> batch_id
                >> ad >> am >> ay
                >> ed >> em >> ey
                >> stock;
            std::getline(ifile, batch_desc); // discard newline
            std::getline(ifile, batch_desc);
            Date added(ad, am, ay);
            Date expiry(ed, em, ey);
            // add batch
            inventory.get_item(i)->add_batch(stock, added, expiry, batch_desc, batch_id);
        }
    }
    // read transactions
    int transaction_record, transactions_size;
    ifile >> transaction_record >> transactions_size;
    for (int i = 0; i < transactions_size; ++i) {
        // get transaction
        std::string transaction_id, item_id, batch_id;
        int ad, am, ay, type, amount;
        ifile >> transaction_id >> item_id >> batch_id
            >> ad >> am >> ay >> type >> amount;
        Date added(ad, am, ay);
        // add transaction
        inventory.add_transaction(item_id, batch_id, added, static_cast<TransactionType>(type), amount);
    }
    // close file
    ifile.close();
    std::cout << "\nInventory read from: " << filename;
}
