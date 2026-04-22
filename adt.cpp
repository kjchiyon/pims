// adt.cpp
// ABSTRACT DATA TYPES

#include <iostream>
#include <sstream>  // for string manipulation
#include <iomanip>  // for cout manipulation
#include <chrono>   // for get_today()
#include "data.hpp"
#include "adt.hpp"

// ---------------- Date ----------------

Date::Date(
    int p_day,
    int p_month,
    int p_year
) :
    day(p_day),
    month(p_month),
    year(p_year)
{}

int Date::get_day(void) { return day; }
int Date::get_month(void) { return month; }
int Date::get_year(void) { return year; }

bool Date::is_after(Date other_date) {
    // check if this date after other date
    if (year > other_date.year) return true;
    if (year < other_date.year) return false;
    if (month > other_date.month) return true;
    if (month < other_date.month) return false;
    return day > other_date.day;
}

bool Date::is_valid(void) {
    // check basic range
    if (year < 1 || month < 1 || month > 12 || day < 1 || day > 31) return false;
    // check leap year
    int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    bool is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (is_leap) {
        days_in_month[1] = 29;
    }
    // check leap month
    if (day > days_in_month[month - 1]) return false;
    // valid date
    return true;
}

// ---------------- Batch ----------------

Batch::Batch(
    std::string p_id,
    std::string p_desc,
    Date p_added,
    Date p_expiry,
    int p_stock
) :
    batch_id(p_id),
    batch_description(p_desc),
    added_date(p_added),
    expiry_date(p_expiry),
    batch_stock(p_stock)
{}

std::string Batch::get_batch_id(void) { return batch_id; }
std::string Batch::get_batch_description(void) { return batch_description; }
Date Batch::get_added_date(void) { return added_date; }
Date Batch::get_expiry_date(void) { return expiry_date; }
int Batch::get_batch_stock(void) { return batch_stock; }

void Batch::set_batch_stock(int p_stock) { batch_stock = p_stock; }
void Batch::set_batch_description(std::string description) { batch_description = description; }

bool Batch::is_expired(void) {
    // check expired by comparing expiry_date with today
    return !expiry_date.is_after(get_today());
}

// ---------------- Item ----------------

Item::Item(
    std::string id,
    std::string description,
    int p_record
) :
    item_id(id),
    item_description(description),
    batch_record(p_record)
{}

std::string Item::get_item_id(void) { return item_id; }
std::string Item::get_item_description(void) { return item_description; }
int Item::get_batch_record(void) { return batch_record; }
int Item::get_batches_size(void) { return item_batches.get_size(); }

int Item::get_total_quantity(void) {
    // sum stock amount of all batches of this item
    int total = 0;
    for (int i = 0; i < get_batches_size(); ++i) {
        total += get_batch(i)->get_batch_stock();
    }
    return total;
}

int Item::get_expired_quantity(void) {
    // sum expired stock amount of all batches of this item
    int total = 0;
    for (int i = 0; i < get_batches_size(); ++i) {
        Batch* b = get_batch(i);
        if (b->is_expired()) total += b->get_batch_stock();
    }
    return total;
}

Batch* Item::get_batch(int p_index) {
    // check index
    if (p_index < 0 || p_index >= get_batches_size()) {
        std::cout << "Batch::get_batch index out of range";
        return nullptr;
    }
    // return batch reference
    return &item_batches.get_node(p_index)->data;
}

void Item::set_item_description(std::string description) {
    item_description = description;
}

int Item::find_batch(std::string batch_id) {
    // linear search
    for (int i = 0; i < item_batches.get_size(); ++i) {
        Batch* b = &item_batches.get_node(i)->data;
        if (b && b->get_batch_id() == batch_id) return i;
    }
    return -1;
}

void Item::add_batch(int p_stock, Date added_date, Date p_expiry, std::string p_desc, std::string batch_id) {
    // check if theres custom batch id
    if (batch_id == "") {
        // create new unique batch id
        std::ostringstream oss;
        oss << "b" << batch_record++;
        batch_id = oss.str();
    }
    // add new batch
    Batch new_batch(batch_id, p_desc, added_date, p_expiry, p_stock);
    item_batches.insert_node(item_batches.get_size(), new_batch);
}

void Item::remove_batch(int p_index) {
    // check index
    if (p_index < 0 || p_index >= get_batches_size()) {
        std::cout << "Batch::remove_batch index out of range";
        return;
    }
    // remove batch
    return item_batches.delete_node(p_index);
}

// ---------------- Transaction ----------------

Transaction::Transaction(
    std::string p_tid,
    std::string p_iid,
    std::string p_bid,
    Date p_date,
    TransactionType p_type,
    int p_change
) :
    transaction_id(p_tid),
    item_id(p_iid),
    batch_id(p_bid),
    transaction_date(p_date),
    transaction_type(p_type),
    stock_change(p_change)
{}

std::string Transaction::get_transaction_id(void) { return transaction_id; }
std::string Transaction::get_item_id(void) { return item_id; }
std::string Transaction::get_batch_id(void) { return batch_id; }
Date Transaction::get_transaction_date(void) { return transaction_date; }
TransactionType Transaction::get_transaction_type(void) { return transaction_type; }
int Transaction::get_stock_change(void) { return stock_change; }

// ---------------- Inventory ----------------

int Inventory::get_items_size(void) { return items.get_size(); }
int Inventory::get_transactions_size(void) { return transactions.get_size(); }
int Inventory::get_transaction_record(void) { return transaction_record; }
Item* Inventory::get_item(int index) { return &items.get_node(index)->data; }
Transaction* Inventory::get_transaction(int index) { return transactions.get_index(index); }

void Inventory::set_transaction_record(int p_record) {
    transaction_record = p_record;
}

void Inventory::add_item(std::string item_id, std::string description, int batch_record) {
    Item new_item(item_id, description, batch_record);
    items.insert_node(items.get_size(), new_item);
}

int Inventory::find_item(std::string item_id) {
    // linear search
    for (int i = 0; i < items.get_size(); ++i) {
        if (get_item(i)->get_item_id() == item_id) return i;
    }
    return -1;
}

void Inventory::remove_item(int index) {
    items.delete_node(index);
}

void Inventory::add_transaction(
    std::string p_iid,
    std::string p_bid,
    Date p_date,
    TransactionType p_type,
    int p_change
) {
    std::ostringstream oss;
    oss << "t" << transaction_record++;
    std::string tid = oss.str();
    Transaction new_tran(tid, p_iid, p_bid, p_date, p_type, p_change);
    transactions.enqueue(new_tran);
}

// ---------------- Helpers ----------------

Date get_today(void) {
    // get current time
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&t);
    // create and return Date object
    Date today(localTime->tm_mday, 1 + localTime->tm_mon, 1900 + localTime->tm_year);
    return today;
}
