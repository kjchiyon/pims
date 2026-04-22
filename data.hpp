// data.hpp
// DATA STRUCTURES

#ifndef DATA_HPP
#define DATA_HPP

#include <iostream>

/// node structure for linked list nodes
template <typename T>
struct Node
{
    // node data
    T data;
    // pointer to next node
    Node* next;
    // Node constructor
    Node(const T& value) : data(value), next(nullptr) {}
};

/// singly linked list implementation
template <typename T>
class LinkedList {
private:
    // pointer to the first node in list
    Node<T>* head;
    // list size
    int size;
public:
    // LinkedList constructor
    LinkedList() : head(nullptr), size(0) {}

    // LinkedList destructor
    ~LinkedList() {
        // free all nodes
        Node<T>* current_node = head;
        while (current_node != nullptr) {
            Node<T>* next_node = current_node->next;
            delete current_node;
            current_node = next_node;
        }
        head = nullptr;
    }

    /// returns the size of the linked list
    int get_size(void) { return size; }

    /// returns reference to node by index
    Node<T>* get_node(int p_index) {
        // check index
        if (p_index < 0 || p_index >= size) {
            std::cout << "LinkedList::get_node index out of range";
            return nullptr;
        }
        // get node reference
        Node<T>* node = head;
        for (int i = 0; i < p_index; ++i) {
            node = node->next;
        }
        return node;
    }

    /// inserts node by index
    void insert_node(int p_index, T p_data) {
        // check index
        if (p_index < 0 || p_index > size) {
            std::cout << "LinkedList::insert_node index out of range";
            return;
        }
        // insert node and reconnect
        Node<T>* new_node = new Node<T>(p_data);
        if (p_index == 0) {
            new_node->next = head;
            head = new_node;
        } else {
            Node<T>* prev_node = head;
            for (int i = 0; i < p_index - 1; ++i) {
                prev_node = prev_node->next;
            }
            new_node->next = prev_node->next;
            prev_node->next = new_node;
        }
        ++size;
    }

    /// deletes node by index and shrink list
    void delete_node(int p_index) {
        // check index
        if (p_index < 0 || p_index >= size) {
            std::cout << "LinkedList::delete_node index out of range";
            return;
        }
        // reconnect and free node
        Node<T>* node;
        if (p_index == 0) {
            node = head;
            head = head->next;
        } else {
            Node<T>* prev_node = head;
            for (int i = 0; i < p_index - 1; ++i) {
                prev_node = prev_node->next;
            }
            node = prev_node->next;
            prev_node->next = node->next;
        }
        delete node;
        --size;
    }
};

/// linear queue implementation with LinkedList-style nodes
template <typename T>
class Queue {
private:
    // pointer to first node
    Node<T>* head;
    // pointer to last node
    Node<T>* tail;
    // queue size
    int size;
public:
    /// Queue constructor
    Queue() : head(nullptr), tail(nullptr), size(0) {}

    /// get queue size
    int get_size(void) { return size; }

    /// get data at front
    T* get_front(void) {
        if (!head) {
            std::cout << "Queue::get_front empty queue";
            return nullptr;
        }
        return &(head->data);
    }

    /// get data at back
    T* get_back(void) {
        if (!tail) {
            std::cout << "Queue::get_back empty queue";
            return nullptr;
        }
        return &(tail->data);
    }

    /// get data at index
    T* get_index(int p_index) {
        // check index
        if (p_index < 0 || p_index >= size) {
            std::cout << "Queue::get_index index out of range";
            return nullptr;
        }
        // traverse to index
        Node<T>* current = head;
        for (int i = 0; i < p_index; i++) {
            current = current->next;
        }
        return &(current->data);
    }

    /// add data to back
    void enqueue(T data) {
        // create new node
        Node<T>* new_node = new Node<T>(data);
        // insert node and change tail
        if (!tail) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        size++;
    }

    /// remove data at front
    void dequeue(void) {
        // change head and delete node
        if (size < 1) return;
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        size--;
        if (!head) tail = nullptr; // queue became empty
    }
};

#endif // DATA_HPP
