//
// Created by luke on 4/14/22.
//

#ifndef INC_5_LETTER_WORDLE_LINKED_LIST_H
#define INC_5_LETTER_WORDLE_LINKED_LIST_H

#include <string>
#include <iostream>


class ll_node {
private:
    ll_node* n = nullptr;
    ll_node* p = nullptr;

public:
    std::string data;
    ll_node* next();
    void setnext(ll_node*);
    ll_node* prev();
    void setprev(ll_node*);
    void delete_nodes();
};
ll_node* ll_node::prev() {
    return this->p;
}
void ll_node::setprev(ll_node* prev) {
    this->p = prev;
}
ll_node* ll_node::next() {
    return this->n;
}
void ll_node::setnext(ll_node* next) {
    this->n = next;
}
void ll_node::delete_nodes() {  //recursive delete
    if (this->n != nullptr) {
        this->n->delete_nodes();
        delete(this->n);
    }
}


class llist {
private:
    void remove_head();
public:
    ll_node* head = nullptr;
    ll_node* tail = nullptr;
    int size = 0;

    void init();
    void append(std::string*);
    void insert(std::string*);
    ll_node* remove(ll_node*);
    static ll_node* linsearch(std::string*, ll_node*);
    ll_node* search(std::string*) const;
    void destroy();
    void print() const;
    void exclude_letters(std::string input);
    void include(char c, int pos);
    void include_correct(char c, int pos);
    void exclude(char c, int pos);
    void filter_results(std::string);
    llist* first_filter_results(std::string) const;
    void filter(std::string input);
};
void llist::init() {
    this->head = nullptr;
    this->tail = nullptr;
    this->size = 0;
}
void llist::append(std::string* s) {
    auto* n = new ll_node;
    if(this->head == nullptr) {
        this->head = n;
        this->tail = n;
        this->head->data = *s;
    }
    else {
        this->tail->setnext(n);
        n->setprev(this->tail);
        this->tail = n;
    }
    n->data = *s;
    this->size++;
}
void llist::remove_head() {
    ll_node* ptr = this->head;
    if (this->head == nullptr)
        return;
    this->head = this->head->next();
    if (this->head != nullptr)
        this->head->setprev(nullptr);
    else
        this->tail = nullptr;
    delete(ptr);
    this->size--;
}
void llist::insert(std::string* str) {
    ll_node* ptr = this->head;
    if (this->head == nullptr) {
        append(str);
        return;
    }
    else {
        auto* newnode = new ll_node;
        if (*str < ptr->data){  // prepend if necessary
            newnode->data = *str;
            this->head = newnode;
            newnode->setnext(ptr);
            ptr->setprev(newnode);
            return;
        }
        // get to proper location
        while(ptr->data < *str && ptr->next() != nullptr) {
            ptr = ptr->next();
        }
        if(ptr->data == *str) {  // disregard if already exists
            delete(newnode);
            return;
        }
        else {      // insert after ptr
            newnode->data = *str;
            newnode->setnext(ptr->next());
            newnode->setprev(ptr);
            if(ptr->next() == nullptr) {
                this->tail = newnode;
            }
            else {
                ptr->next()->setprev(newnode);
            }
            ptr->setnext(newnode);
        }
    }
}
ll_node* llist::remove(ll_node* d) {
    ll_node* retval;
    if(d == this->head) {  // case for deleting head
        remove_head();
        return this->head;
    }
    d->prev()->setnext(d->next());
    if (d->next() != nullptr) {  // case for deleting tail
        d->next()->setprev(d->prev());
        retval = d->next();
        delete(d);
        this->size--;
        return retval;
    }
    else {  // case for deleting in middle of list
        this->tail = d->prev();
        delete(d);
        this->size--;
        return nullptr;
    }
}
void llist::destroy() {
    if(this->head != nullptr) {
        //this->head->delete_nodes();   // want to make this work, but causing stack overflow, guess I'm calling too many recurses
        while (this->tail->prev() != nullptr) {
            remove(this->tail);
        }
        llist::remove_head();
    }
    this->size = 0;
}
void llist::print() const {
    ll_node* ptr = this->head;
    std::cout << "Items in list\n";
    while(ptr != nullptr) {
        std::cout << "- " << ptr->data << std::endl;
        ptr = ptr->next();
    }
    std::cout << "TOTAL: " << this->size << " words\n";
}
ll_node* llist::search(std::string* str) const {
    return llist::linsearch(str, this->head);
}
ll_node* llist::linsearch(std::string* needle, ll_node* haystack) {
    if ((*needle).empty() || haystack == nullptr) {
        return nullptr;
    }
    // Traverse to get to desired location
    while(haystack->next() != nullptr && 0 < (*needle).compare(haystack->data)) {
//        int cmp = (*needle).compare(haystack->data);
        haystack = haystack->next();
    }
    if (*needle == haystack->data) {
        return haystack;
    }
    else {
        return nullptr;
    }
}

#endif //INC_5_LETTER_WORDLE_LINKED_LIST_H
