#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include "Trie.h"
#include <string>
#include <iostream>
template<class T> class AhoCorasick{
private:
    TrieNode<T>* root;                      //Initializes the root pointer of the trie


public:

AhoCorasick(const AhoCorasick&) = delete;               //Stops C++ from auto copying which would make the
AhoCorasick& operator=(const AhoCorasick&) = delete;    //destructor delete the root twice, causing a crash

AhoCorasick();                                          //Makes sure every instance has their own trie
void createTrie(std::string pattern[], int k);         //Builds the trie from an array of k patterns
void failAndOutLinkage();                                         //Builds the failure and output links of the trie to enable the text search
int search(const std::string& sequence);               //Function to search our trie for desired patterns
~AhoCorasick();                                                         //Deleting the root causes a cascade effect of children deleting their children
};

//AhoCorasick is a template, so its function bodies must be visible wherever AhoCorasick<T> is used; they live in Aho-Corasick.tpp and are included here
#include "Aho-Corasick.tpp"
#endif