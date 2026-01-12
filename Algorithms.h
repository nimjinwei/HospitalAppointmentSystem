#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "LinkedList.h"
#include <string>

// In-place insertion sort by relinking linked list nodes (no data copy).
// Algorithm: Insertion Sort
// Time Complexity: O(n²)
// Space Complexity: O(1)
// 'comp' is a comparison function pointer that returns true when
// the first argument should come after the second (i.e. needs swapping).
template <typename T>
void manualSort(LinkedList<T>& list, bool (*comp)(const T&, const T&)) {
    typedef Node<T> ListNode;
    
    ListNode* head = list.getHead();
    if (!head || !head->next) return; // Empty or single element list
    
    ListNode* newHead = nullptr;
    ListNode* newTail = nullptr;
    
    ListNode* curr = head;
    while (curr) {
        ListNode* next = curr->next;
        // Detach current node from original list
        curr->prev = curr->next = nullptr;
        
        if (!newHead) {
            // First node in sorted list
            newHead = newTail = curr;
        } else {
            // Find insertion point: insert before first node where comp returns true
            ListNode* pos = newHead;
            bool inserted = false;
            
            while (pos) {
                if (comp(pos->data, curr->data)) {
                    // Insert curr before pos
                    curr->next = pos;
                    curr->prev = pos->prev;
                    pos->prev = curr;
                    
                    if (curr->prev) {
                        curr->prev->next = curr;
                    } else {
                        newHead = curr; // curr becomes new head
                    }
                    inserted = true;
                    break;
                }
                if (!pos->next) break; // Reached end
                pos = pos->next;
            }
            
            if (!inserted) {
                // Append at the end (curr is larger than all existing nodes)
                pos->next = curr;
                curr->prev = pos;
                curr->next = nullptr;
                newTail = curr;
            }
        }
        
        curr = next;
    }
    
    // Update list structure
    list.setHead(newHead);
    list.setTail(newTail ? newTail : newHead);
}

// ================= SEARCHING ALGORITHMS =================

// Algorithm 1: Hash based search


// Algorithm 2: Linear Search (for partial matches and multiple results)
// Algorithm: Linear Search
// Time Complexity: O(n)
// Space Complexity: O(1)
// Use case: When all elements must be checked (partial matches, multiple results)
// Returns pointer to first found element, or nullptr if not found
// Calls processFunc for each matching element
// Returns count of matches found
template <typename T>
int linearSearchAll(LinkedList<T>& list, bool (*matchFunc)(const T&), void (*processFunc)(const T&)) {
    typedef Node<T> ListNode;
    ListNode* curr = list.getHead();
    int count = 0;
    
    while (curr) {
        if (matchFunc(curr->data)) {
            processFunc(curr->data);
            count++;
        }
        curr = curr->next;
    }
    return count;
}

// Helper: split a string by a delimiter into a LinkedList<string>.
inline void splitString(const std::string& str, char delimiter, LinkedList<std::string>& outList) {
    std::string token;
    for (char c : str) {
        if (c == delimiter) {
            if (!token.empty()) {
                outList.append(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) {
        outList.append(token);
    }
}

#endif