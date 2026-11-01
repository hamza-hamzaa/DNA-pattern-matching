//file has a .tpp extension because it is a template class implementation so for the linker 
//to combine files properly this must use .tpp
#include "Queue.h"
//all functions need template definition before them 
template<class T>
Queue<T>::Queue():head(nullptr),tail(nullptr){}

template<class T>
Queue<T>::Queue(const Queue<T>& copy): head(nullptr), tail(nullptr){
    QueueNode* temp = copy.head;
    while (temp!=nullptr) {
        enqueue(temp->data);
        temp=temp->next;
    }
}
template<class T>
Queue<T>& Queue<T>::operator=(const Queue& copy){
    if (this == &copy) {
        return *this;
    }

    while(head!=nullptr){
        dequeue();
    }
    QueueNode* temp = copy.head;
    while (temp!=nullptr) {     
        enqueue(temp->data);    
        temp=temp->next;        
    }    
    return *this;                        
}

template<class T> 
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
bool Queue<T>::empty(){
    return head==nullptr;
}
template<class T>
TrieNode<T>* Queue<T>:: top(){
    if(head==nullptr){return nullptr;}
    return head->data;
}

