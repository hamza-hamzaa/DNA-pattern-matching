#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include "Trie.h"
#include <string>
#include <iostream>
template<class T> class AhoCorasick{
private:
    TrieNode<T>* root;                      //Initializes the root pointer of the tries
                                            

public:

AhoCorasick(const AhoCorasick&) = delete;               //Stops C++ from auto copying which would make the 
AhoCorasick& operator=(const AhoCorasick&) = delete;    //destructor delete the root twice, causing a crash

AhoCorasick(){
    root = new TrieNode<T>;                       //Makes sure every instance has their own trie
}
void createTrie(std::string pattern[], int k){         //Builds the trie from an array of k patterns
    for(int i = 0;i<k;i++){
        root->insert(root, pattern[i], i);
    }

    this->failAndOutLinkage();
}

void failAndOutLinkage(){                                         //Builds the failure and output links of the trie to enable the text search
    Queue<T> q;                                                   //Initialize a queue for BFS

    for(int i = 0;i<SIZE;i++){
        if(root->children[i] != nullptr){                            //Makes all the root's childrens' failure links connect to root
            root->children[i]->failure = root;
            q.enqueue(root->children[i]);                            //Queues all the first level nodes, ensuring Breadth First Search
        }
        
    }

    while(!q.empty()){
        TrieNode<T>* curNode = q.top(); q.dequeue();                 //Takes the next node in the level

        for(int i = 0;i<SIZE;i++){
            if(curNode->children[i] != nullptr){                     //We skip empty nodes, and all previous nodes are already linked

                TrieNode<T>* curChild = curNode->children[i];        //Grabs the value of the child node
                TrieNode<T>*curFail = curNode->failure;              //Starts from parents failure link to build childs as an extension off it

                while(curFail != nullptr && curFail->children[i] == nullptr)    //Traverses failure links until a child of the same letter is found

                curFail = curFail->failure;

                if(curFail == nullptr)                                    //No ending of the sequence can be extended with this letter, so return to root
                    curChild->failure = root;
                else
                    curChild->failure = curFail->children[i];
                
                if(curChild->failure->seqID != -1)                              //Catches smaller patterns embedded in larger patterns
                    curChild->output = curChild->failure;
                else
                curChild->output = curChild->failure->output;                   //Copies the failure links output node

                q.enqueue(curChild);                                            //Maintains Breadth First Search order
            }
        }
    }
}

void search(const std::string& sequence){               //Function to search our trie for desired patterns

    TrieNode<T>* current = root;                        //Initialize traversal node

    for(int j = 0; j < (int)sequence.length();j++){     //Loops over every character in the string

        int index = current->validateCharToIndex(sequence[j]);          //Checks if the read character is a valid DNA letter
        if(index == -1)
        {current = root; continue;}                                     //If character does not continue any of the patterns or is an invalid character like 'N', reset to root
        while(current != root && current->children[index] == nullptr)   //When at any node with no child for this letter, there are no 
        current = current->failure;                                     //more possible extensions so it triggers a failure link
        if(current->children[index] != nullptr)                         //Continues down the trie if the following character starts to match a desired pattern
            current = current->children[index];
        if(current->seqID != -1)                                        //If the node is a desired pattern output, output which pattern it is, and where it starts and ends
            std::cout << "From " << j-current->patternLength + 1 << " to " << j << " " << 
            sequence.substr(j-current->patternLength + 1, current->patternLength) << std::endl;

        TrieNode<T>* tempo = current->output;                           //Temporary node to output all the embedded patterns found in current node

        while(tempo != nullptr){                                        //Same action but for the embedded patterns, output which pattern it is, and where it starts and ends
            std::cout << "From " << j-tempo->patternLength + 1 << " to " << j << " " << 
            sequence.substr(j-tempo->patternLength + 1, tempo->patternLength) << std::endl;
            tempo = tempo->output;                                      //Loops through all embedded patterns before moving to the next character
        } 

    }
}

~AhoCorasick(){                                                         //Deleting the root causes a cascade effect of children deleting their children
    delete root;
}
};
#endif