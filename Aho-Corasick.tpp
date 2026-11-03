#include "Trie.h"
#include <string>
using namespace std;

const int MAX_STATES = 256;             //The maximum number of states in the algorithm
const int MAX_LETTERS = 4;              //The four letters for DNA suffix searching are A, C, G, T

int output[MAX_STATES];                 //The output function uses a bitmask so that when the machine automaton 
                                        //enters a state, it will be 1 if the word appears in the correct index

int failure[MAX_STATES];                //The fallback state for failures during search are kept here

template<class T> struct TrieNode* DB;  //The data-structure we will be inserting and searching in using the Aho-Corasick algorithm

int createAutomaton(string words[], int s){

}