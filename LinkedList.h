#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedList() {
        clear();
    }

    void clear() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        size = 0;
    }

    void append(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    // Remove a specific node (used by algorithms)
    void removeNode(Node<T>* node) {
        if (!node) return;

        if (node == head) {
            head = node->next;
            if (head) head->prev = nullptr;
        } else {
            node->prev->next = node->next;
        }

        if (node == tail) {
            tail = node->prev;
            if (tail) tail->next = nullptr;
        } else {
            if (node->next) node->next->prev = node->prev;
        }

        delete node;
        size--;
    }

    // For traversal and algorithm access
    Node<T>* getHead() const { return head; }
    Node<T>* getTail() const { return tail; }
    
    // For algorithms to update list structure (used by sorting algorithms)
    void setHead(Node<T>* newHead) { head = newHead; }
    void setTail(Node<T>* newTail) { tail = newTail; }
    
    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }
};

#endif