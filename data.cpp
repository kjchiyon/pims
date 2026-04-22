// data.cpp by CHEONG KAI JIN
/*
#include <iostream>
#include "data.hpp"

//
template <typename T>
LinkedList<T>::LinkedList() : // init list
    head(nullptr),
    size(0)
{}

//
template <typename T>
LinkedList<T>::~LinkedList() {
    // free all node data
    Node<T>* current_node = head;
    while (current_node != nullptr) {
        Node<T>* next_node = current_node->next;
        delete current_node;
        current_node = next_node;
    }
    head = nullptr;
}

//
template <typename T>
int LinkedList<T>::get_size(void) {return size;}

//
template <typename T>
Node<T>* LinkedList<T>::get_node(int param_index) {
    // check index
    if (param_index < 0 || param_index >= size) {
        std::cout << "LinkedList::get_node index out of range";
        return nullptr;
    }
    // traverse to index
    Node<T>* node = head;
    for (int i = 0; i < param_index; ++i) {
        node = node->next;
    }
    return node;
}

//
template <typename T>
void LinkedList<T>::insert_node(int param_index, T param_data) {
    // check index
    if (param_index < 0 || param_index > size) {
        std::cout << "LinkedList::insert_node index out of range";
        return;
    }
    // insert node
    Node<T>* new_node = new Node(param_data);
    if (param_index == 0) { // insert front
        new_node->next = head;
        head = new_node;
    } else { // insert somewhere else
        // traverse to index
        Node<T>* prev_node = head;
        for (int i = 0; i < param_index - 1; ++i) {
            prev_node = prev_node->next;
        }
        // connect new node
        new_node->next = prev_node->next;
        prev_node->next = new_node;
    }
    // increase size var
    ++size;
}

//
template <typename T>
void LinkedList<T>::delete_node(int param_index) {
    // check index
    if (param_index < 0 || param_index >= size) {
        std::cout << "LinkedList::remove_node index out of range";
        return;
    }
    // delete node
    Node<T>* node;
    if (param_index == 0) { // remove front
        node = head;
        head = head->next;
    } else { // remove somewhere else
        // traverse to index
        Node<T>* prev_node = head;
        for (int i = 0; i < param_index - 1; ++i) {
            prev_node = prev_node->next;
        }
        // reconnect nodes
        node = prev_node->next;
        prev_node->next = node->next;
    }
    delete node; // free memory
    // decrease size var
    size--;
}

// ----------- Queue ------------

// Queue constructor
template <typename T>
Queue<T>::Queue()
{}

// Queue destructor
template <typename T>
Queue<T>::~Queue()
{}

//
template <typename T>
int Queue<T>::get_size(void) {
    return queue_list.get_size();
}

//
template <typename T>
T* Queue<T>::get_front(void) {
    return &queue_list.get_node(0).data;
}

//
template <typename T>
T* Queue<T>::get_back(void) {
    return &queue_list.get_node(queue_list.get_size()).data;
}

//
template <typename T>
T* Queue<T>::get_index(int param_index) {
    // check index
    if (param_index < 0 || param_index >= queue_list.get_size()) {
        std::cout << "LinkedList::remove_node index out of range";
        return nullptr;
    }
    // get data
    return &queue_list.get_node(param_index).data;
}

//
template <typename T>
void Queue<T>::enqueue(T data) {
    int index = queue_list.get_size();
    queue_list.insert_node(index, data);
}

//
template <typename T>
void Queue<T>::dequeue(void) {
    // check list has item
    if (queue_list.get_size() < 1) return;
    // dequeue
    queue_list.remove_node(0);
}
*/
