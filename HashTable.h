#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "LinkedList.h"
#include <string>
#include <functional>
using namespace std;

// Hash Table for fast O(1) lookups by ID
template <typename T>
class HashTable {
private:
    static const int TABLE_SIZE = 1009; // Prime number for better distribution
    LinkedList<T*>* table[TABLE_SIZE];   // store pointers to objects owned elsewhere
    int elementCount;
    
    // Hash function: converts ID string to table index
    int hashFunction(const std::string& key) const {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % TABLE_SIZE;
    }
    
    // Function pointer to extract ID from object (by reference)
    std::string (*getID)(const T&);

public:
    // Constructor: pass a function to extract ID from your object
    HashTable(std::string (*idExtractor)(const T&)) 
        : elementCount(0), getID(idExtractor) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }
    
    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i]) delete table[i];
        }
    }
    
    // Insert pointer to an object owned elsewhere
    void insert(T* value) {
        std::string id = getID(*value);
        int index = hashFunction(id);
        
        if (!table[index]) {
            table[index] = new LinkedList<T*>();
        }
        
        // Check if ID already exists (avoid duplicates)
        Node<T*>* curr = table[index]->getHead();
        while (curr) {
            if (getID(*curr->data) == id) {
                return; // Already exists
            }
            curr = curr->next;
        }
        
        table[index]->append(value);
        elementCount++;
    }
    
    // Search: returns pointer stored in table (same pointer stored by insert)
    T* search(const std::string& id) {
        int index = hashFunction(id);
        
        if (!table[index]) return nullptr;
        
        Node<T*>* curr = table[index]->getHead();
        while (curr) {
            if (getID(*curr->data) == id) {
                return curr->data;
            }
            curr = curr->next;
        }
        return nullptr;
    }
    
    // Remove: does not delete the object, only removes pointer from index
    bool remove(const std::string& id) {
        int index = hashFunction(id);
        
        if (!table[index]) return false;
        
        Node<T*>* curr = table[index]->getHead();
        while (curr) {
            if (getID(*curr->data) == id) {
                table[index]->removeNode(curr);
                elementCount--;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
    
    // Update by copying newValue into the existing object referenced by the pointer
    bool update(const std::string& id, const T& newValue) {
        T* found = search(id);
        if (found) {
            *found = newValue;
            return true;
        }
        return false;
    }
    
    int getSize() const { return elementCount; }
    
    bool exists(const std::string& id) {
        return search(id) != nullptr;
    }
    
    // Get all elements (for display/iteration) - appends copies to outList
    void getAllElements(LinkedList<T>& outList) const {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i]) {
                Node<T*>* curr = table[i]->getHead();
                while (curr) {
                    outList.append(*curr->data);
                    curr = curr->next;
                }
            }
        }
    }
};

#endif