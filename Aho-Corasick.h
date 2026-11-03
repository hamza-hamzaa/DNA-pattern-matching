#ifndef AHO-CORASICK_H
#define AHO-CORASICK_H

#include "Trie.h"
#include <string>
const int SIZE = 4;
template<class T> class AhoCorasick{
private:
    TrieNode<T>* root;                      //Initializes an empty trie node
                                            

public:

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

        for(int i = 0;i<4;i++){
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

void search(const std::string sequence){
    for(char j: sequence){

    }
}

~AhoCorasick();
};
#endif