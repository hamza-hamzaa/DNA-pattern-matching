//file has a .tpp extension because it is a template class implementation so for the linker 
//to combine files properly this must use .tpp
#include "Queue.h"
//all functions need template definition before them 
template<class T>
//create a queue and initialise both head and tail to be no pointers
Queue<T>::Queue():head(nullptr),tail(nullptr){}

template<class T>
//copies already existing queue through a deep copy by copying each elements individually
Queue<T>::Queue(const Queue<T>& copy): head(nullptr), tail(nullptr){
    QueueNode* temp = copy.head;
    while (temp!=nullptr) {
        enqueue(temp->data);
        temp=temp->next;
    }
}
template<class T> 
//deletes the queue thoroughly by itterating over each element on the queue and deleting it individually
Queue<T>::~Queue(){
    QueueNode* temp;
    while(head!=nullptr){
        temp=head;
        head=head->next;
        delete temp;
    }
    tail=nullptr;
}
template<class T>
//enqueues by first checking the case if the queue is empty and if so, it places it directly
// in head otherwise it itterates the linked list until the end, then appends it
void Queue<T>::enqueue(TrieNode<T>* d){
    if(head==nullptr){
        head=new QueueNode(d);
        tail=head;
        return;
    }
    tail->next=new QueueNode(d);
    tail=tail->next;
}
template<class T>
//dequeues by first checking if the queue is empty then it removes the first element
void Queue<T>::dequeue(){
    if(head==nullptr){return;}
    QueueNode* temp=head;
    head=head->next;
    delete temp;
    if(head==nullptr){
        tail=nullptr;
    }
}
template<class T>
//checks if head is empty because if so then there must be no elements in the queue and hence its empty
bool Queue<T>::empty(){
    return head==nullptr;
}
template<class T>
//returns value at head since this is the top of the queue
TrieNode<T>* Queue<T>:: top(){
    if(head==nullptr){return nullptr;}
    return head->data;
}

