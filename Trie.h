#ifndef TRIE_H
#define TRIE_H
#include "Queue.h"
#include <string>
const int SIZE = 4;

template<class T> struct TrieNode {
    public:
    TrieNode* children[SIZE];                           //Pointer array for children of each node, indices from 0-3 for DNA letters A, T, C, G
    TrieNode* failure;                                  //Failure link for Aho-Corasick
    TrieNode* output;                                   //Nearest node on the failure link chain that has an output

    int seqID;                                          //Used to tag the last node of a completed pattern in the sequence
    int patternLength;                                   //Used for finding the indices of a pattern in Aho-Corasick search function

    TrieNode();                                         //Constructor
    int validateCharToIndex(const char ch);             //Used to validate the read character in the sequence
                                                        //and help calculate the state of the child
    void insert(TrieNode* root, const std::string& sequence, int tag);  //Function to insert a new sequence into the trie
    ~TrieNode();                                                //Only deletes the children because output and failure only point at nodes in the tree
};

//TrieNode is a template, so its function bodies must be visible wherever TrieNode<T> is used; they live in Trie.tpp and are included here
#include "Trie.tpp"
#endif
