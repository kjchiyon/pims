// adt.hpp
// ABSTRACT DATA TYPES

// note:
// these are the classes you might use in your part of program
// please understand them and apply in your code

#ifndef ADT_HPP
#define ADT_HPP

#include "data.hpp"

/// enum for transaction
enum TransactionType {
    SOLD,
    REFUND,
    RESTOCK,
    DISCARD
};

/// Date class
class Date
{
private:
    int day;
    int month;
    int year;
public:
    ///
    Date(int p_day, int p_month, int p_year);
    /// returns the day of date
    int get_day(void);
    /// returns the month of date
    int get_month(void);
    /// returns the year of date
    int get_year(void);
    /// check if this date is after another provided date
    bool is_after(Date otherDate);
    /// check if this date is valid
    bool is_valid(void);
};

/// Batch class
class Batch
{
private:
    std::string batch_id;
    std::string batch_description;
    Date added_date;
    Date expiry_date;
    int batch_stock;
public:
    // Batch constructor
    Batch(std::string id, std::string description, Date added, Date expiry, int stock);
    /// returns batch id
    std::string get_batch_id(void);
    /// returns batch description
    std::string get_batch_description(void);
    /// returns added date
    Date get_added_date(void);
    /// returns expiry date
    Date get_expiry_date(void);
    /// returns stock amount
    int get_batch_stock(void);
    /// sets batch description
    void set_batch_description(std::string batch_description);
    /// sets batch stock
    void set_batch_stock(int stock);
    /// checks if batch is expired
    bool is_expired(void);
};

/// Item class
class Item
{
private:
    std::string item_id;
    std::string item_description;
    int batch_record; // for unique batch id
    LinkedList<Batch> item_batches;
public:
    // Item constructor
    Item(std::string id, std::string description, int batch_record);
    /// returns item id
    std::string get_item_id(void);
    /// returns item description
    std::string get_item_description(void);
    /// returns batch_record
    int get_batch_record(void);
    /// returns size of batches
    int get_batches_size(void);
    /// counts and returns total quantity
    int get_total_quantity(void);
    /// counts and returns quantity of expired
    int get_expired_quantity(void);
    /// returns reference to batch object by index
    Batch* get_batch(int index);
    /// sets batch description by batch id
    void set_item_description(std::string description);
    /// adds a batch with unique id and returns its index
    void add_batch(int amount, Date added_date, Date expiry_date, std::string description = "", std::string batch_id = "");
    /// returns index by batch id
    int find_batch(std::string batch_id);
    /// removes batch by index
    void remove_batch(int index);
};

//
class Transaction
{
private:
    std::string transaction_id;
    std::string item_id;
    std::string batch_id;
    Date transaction_date;
    TransactionType transaction_type;
    int stock_change;
public:
    // Transaction constructor
    Transaction(
        std::string transaction_id,
        std::string item_id,
        std::string batch_id,
        Date transaction_date,
        TransactionType transaction_type,
        int stock_change
    );
    /// return transaction id
    std::string get_transaction_id(void);
    /// return item id
    std::string get_item_id(void);
    /// return batch id
    std::string get_batch_id(void);
    /// return transaction date
    Date get_transaction_date(void);
    /// return transaction description
    TransactionType get_transaction_type(void);
    /// return stock change
    int get_stock_change(void);
};

/// class for inventory
class Inventory
{
private:
    LinkedList<Item> items;
    Queue<Transaction> transactions;
    int transaction_record = 0; // used for unique transaction id
public:
    /// returns size of items
    int get_items_size(void);
    /// returns size of transactions
    int get_transactions_size(void);
    /// returns transaction record
    int get_transaction_record(void);
    /// return item object by index
    Item* get_item(int index);
    /// return transaction object
    Transaction* get_transaction(int index);
    /// sets transaction record
    void set_transaction_record(int transaction_record);
    /// add item with item id
    void add_item(std::string item_id, std::string description, int batch_record = 0);
    /// return item object index by item id
    int find_item(std::string item_id);
    /// remove item with index
    void remove_item(int item_index);
    /// add transaction with auto generated id
    void add_transaction(
        std::string item_id,
        std::string batch_id,
        Date transaction_date,
        TransactionType transaction_type,
        int stock_change
    );
    /// return transaction object index by transaction id
    int find_transaction(std::string transaction_id);
};

/// returns a Date with today's date
Date get_today(void);

#endif // ADT_HPP
