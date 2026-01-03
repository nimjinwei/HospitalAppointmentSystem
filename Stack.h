#ifndef STACK_H
#define STACK_H

#include <iostream>

// To avoid conflict with LinkedList's Node, define a separate internal structure
template <typename T>
struct StackNode {
    T data;
    StackNode* next;
    StackNode(T val) : data(val), next(nullptr) {}
};

template <typename T>
class Stack {
private:
    StackNode<T>* topNode; // Top of stack pointer
    int size;

public:
    Stack() : topNode(nullptr), size(0) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Push: Add element to top of stack - O(1)
    void push(T value) {
        StackNode<T>* newNode = new StackNode<T>(value);
        newNode->next = topNode;
        topNode = newNode;
        size++;
    }

    // Pop: Remove element from top of stack - O(1)
    void pop() {
        if (isEmpty()) return;
        StackNode<T>* temp = topNode;
        topNode = topNode->next;
        delete temp;
        size--;
    }

    // Peek: View top element without removing - O(1)
    T top() const {
        if (isEmpty()) throw std::runtime_error("Stack is empty");
        return topNode->data;
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }

    int getSize() const {
        return size;
    }

    // Display all stack elements (used for system logs)
    void displayStack() const {
        StackNode<T>* current = topNode;
        while (current != nullptr) {
            std::cout << ">> " << current->data << std::endl;
            current = current->next;
        }
    }
};

#endif