#ifndef TRIE_H
#define TRIE_H
#include "Queue.h"
#include <string>
const int SIZE = 4;

template<class T> struct TrieNode {
    public:
    TrieNode* children[SIZE];                           //Pointer array for children of each node, indices from 0-3 for DNA letters A, T, C, G
    TrieNode* failure;                                  //Failure link for Aho-Corasick
    TrieNode* output;                                   //Output link during failure link traversal

    int seqID;                                          //Used to tag the last node of a completed pattern in the sequence

    TrieNode(){                                         //Initializes the root
        seqID = -1;                                     //There is no sequence to tag yet
        for(int i = 0;i<SIZE;i++)                       //Indexes every child node, failure and output link with nullptr for initilization
            {children[i] = nullptr;
        }
        failure = nullptr;
        output = nullptr;
    }
    int validateCharToIndex(const char ch){             //Used to validate the read character in the sequence 
                                                        //and help calculate the state of the child
        switch(ch){
        case 'A': return 0; 
        case 'T': return 1;
        case 'C': return 2;
        case 'G': return 3;
        default: return -1;
        }

    }
    void insert(TrieNode* root, const std::string& sequence, int tag){  //Function to insert a new sequence into the trie
        //start at root node
        if(sequence == "") return;                              //Edge-case where sequence is empty
        TrieNode* current = root;
                                      
        for(char letter: sequence){                             //Iterate through the letter in the sequence
        
            int num = current->validateCharToIndex(letter);     //Validating the characters in the sequence before inserting them into the trie
            if(num == -1) return;                               //If there is an unknown or typo in the sequence, we stop writing to the trie

            if(current->children[num] == nullptr){              //Create a new child node if one doesnt exist 
                                                                //If node already exists, continue
                TrieNode* newChild = new TrieNode();

                current->children[num] = newChild;              //Keeping the reference of the new child node to navigate to it
            }

            current = current->children[num];                   //Move to the new child node

        }
        current->seqID = tag;                                   //Tags the last node of the sequence with the index
    }
    
    ~TrieNode();
};

#endif