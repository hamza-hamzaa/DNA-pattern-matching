#ifndef QUEUE_H
#define QUEUE_H

template<class T>
struct TrieNode;

//template so that the Trie can store any type of data
template<class T>
class Queue{
    private:
        // Queue will use a linked list implentation rather than a circular
        // queue in order to allow any string input size without losing previous info
        struct QueueNode{
            // The queue stores TrieNode pointer rather than strings because 
            // During Aho-Corasick's algorithm, we need to process trie nodes to construct
            // their failure links. Storing pointers allows  access without copying their 
            //data or retraversing the trie to find them again.
            // The queue owns only its linked-list nodes, not the TrieNode objects.
            TrieNode<T>* data;
            QueueNode* next;
            QueueNode(TrieNode<T>* d):data(d),next(nullptr){}
        };
        //head pointers allows functions like dequeue,top and empty 
        //while tail pointer allows equeue to have complexity O(1)
        QueueNode* head;
        QueueNode* tail;
    public:
        Queue();
        //both copy constructor and operator overloading to deep copy queues
        Queue(const Queue& copy);
        Queue<T>& operator=(const Queue& copy);
        //destructor that frees up memory 
        ~Queue();
        bool empty();
        void enqueue(TrieNode<T>* d);
        TrieNode<T>* top();
        void dequeue();

        
};
//in order for the file to compile properly it needs the .tpp file to be declared here
#include "Queue.tpp"
#endif