/***
 * Project :  Wordle Helper
 * Author  :  Luke Gorham
 * Date    :  21 May 2022
 */

#ifndef INC_5_LETTER_WORDLE_LINKED_LIST_H
#define INC_5_LETTER_WORDLE_LINKED_LIST_H

#include <string>
#include <iostream>
#include <iomanip>
#include "simple_stack.hpp"

//#define WORD_SIZE 5
int WORD_SIZE = 5;
#define ALPHABET_SIZE 26
#define MAX_SIZE 100000000


/**
 * @brief Node for linked list 'llist'. Each node stores next and prev nodes
 * along with data (which is a std::string. Note that next (n) and prev (p)
 * members are private and must be set with the setnext() and setprev() funcs.
 * They can be accessed using next() and prev().
 */
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

void delete_nodes(ll_node* n) {
    if (n == nullptr)
        return;
    ll_node* n2 = n->next();
    while (n2 != nullptr) {
        free(n);
        n = n2;
        n2 = n2->next();
    }
    free(n);
}

/**
 * @brief Main linked list for storing words. Lists are double-linked
 * and track size. Each list has several helper functions (see declarations)
 * for more info.
 */
class llist {
public:
    ll_node* head = nullptr;
    ll_node* tail = nullptr;
    int size = 0;

    void init();
    void append(std::string*);
    void remove_head();
    void insert(std::string*);
    ll_node* remove(ll_node*);
    static ll_node* linsearch(std::string*, ll_node*);
    ll_node* search(std::string*) const;
    void position_exclude(std::string*);
    void destroy();
    void print() const;
    void printhistory() const;
    void filter_results(std::string);
    void filter(std::string);
    void include(std::string*, int);
    void include(char, int);
    void include_correct(char, int);
    void exclude(std::string*);
    void exclude(char, int);
    void exclude_letters(std::string);
    llist* first_filter_results(std::string) const;
    void rm_duplicates();
    void info();
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

void llist::position_exclude(std::string* s) {
    ll_node* ptr;
    std::string str_to_pass = "";
    bool b = false;
    for(int c=0; c < WORD_SIZE; c++) {
        ptr = this->head;
        b = false;
        while(ptr != nullptr) {
            if((*s)[c] == '*') {
                b = true;
                break;
            }
            if ((*s)[c] == ptr->data[c]) {
                ptr = this->remove(ptr);  // ptr gets incremented when removing from list
            }
            else
                ptr = ptr->next();
        }
        if(!b) {
            // Ensure (*s)[c] is included if not a '*'
            str_to_pass = (*s).substr(c,1);
            this->include(&str_to_pass, 1);
        }
    }
}

void llist::destroy() {
    delete_nodes(this->head);
}

void llist::print() const {
    ll_node* ptr = this->head;
    std::cout << "WORDS IN LIST [" << this->size << "]:\n";
    while(ptr != nullptr) {
        std::cout << " - " << ptr->data << std::endl;
        ptr = ptr->next();
    }
    std::cout << "Total Size: " << this->size << std::endl;
}

void llist::printhistory() const {
    ll_node* ptr = this->head;
    std::cout << "Filter History:\n";
    while(ptr != nullptr) {
        std::cout << "   " << ptr->data << std::endl;
        ptr = ptr->next();
    }
    std::cout << "End of History.\n";
}

void llist::filter(std::string input) {
    // loop to determine if any correct value & position (those narrow list faster than value only)
    for (int i = 0; i < WORD_SIZE; i++) {
        if (input[i] == '_' || input[i] < (char)65)
            continue;
        else if (input[i] == toupper(input[i])) {
            this->include_correct(tolower(input[i]), i);   // use tolower because dictionary is all in lowercase
            input[i] = '_';
        }
    }
    for (int i = 0; i < WORD_SIZE; i++) {
        if (input[i] == '_' || input[i] < (char)65)
            continue;
        else
            this->include(input[i], i);
    }
}

void llist::include_correct(char c, int pos) {
    ll_node* ptr = this->head;
    while (ptr != nullptr) {
        if (ptr->data[pos] != c) {
            ptr = this->remove(ptr);
            if (ptr == nullptr)
                break;
        }
        ptr = ptr->next();
    }
}

void llist::include(char c, int pos) {
    exclude(c, pos);   // cannot be in position, otherwise would be correct
    ll_node* ptr = this->head;
    bool has_c;
    while (ptr != nullptr) {
        has_c = false;
        for (int i = 0; i < WORD_SIZE && !has_c; i++) {
            if (ptr->data[i] == c) {
                has_c = true;
                break;
            }
        }
        if (!has_c) {
            ptr = this->remove(ptr);
            if (ptr == nullptr)
                break;
        }
        ptr = ptr->next();
    }
    return;
}

void llist::exclude(char c, int pos) {
    ll_node* ptr = this->head;
    while (ptr != nullptr) {
        if (ptr->data[pos] == c) {
            ptr = this->remove(ptr);
            if (ptr == nullptr)
                break;
        }
        ptr = ptr->next();
    }
    return;
}

void llist::filter_results(std::string input) {
    ll_node* ptr = this->head;
    for(int i=0; i<WORD_SIZE; i++) {
        if(input[i] != '_') {
            while(ptr != nullptr) {
                if(input[i] != ptr->data[i]) {
                    ptr = this->remove(ptr);  // increments pointer when removing
                }
                else{
                    ptr = ptr->next();
                }
            }
            ptr = this->head;  // reset pointer to beginning of subset
        }
    }
}

llist* llist::first_filter_results(std::string input) const {
    auto* s = new llist;
    ll_node* ptr = this->head;
    for(int i=0; i<WORD_SIZE; i++) {
        if(input[i] != '_') {
            while(ptr != nullptr) {
                if(input[i] == ptr->data[i]) {
                    s->append(&ptr->data);
                }
                ptr = ptr->next();
            }
        }
    }
    return s;
}

ll_node* llist::search(std::string* str) const {
    /**
     * Note: this function can be used for implementing different
     * kinds of search algorithms, but for most applications and
     * dictionaries with only several thousand words, a linear
     * search algorithm is sufficiently fast and poses no averse
     * wait times for the user.
     */
    return llist::linsearch(str, this->head);
}

ll_node* llist::linsearch(std::string* needle, ll_node* haystack) {
    if ((*needle).empty() || haystack == nullptr) {
        return nullptr;
    }
    // Traverse to get to desired location
    while(haystack->next() != nullptr && 0 < (*needle).compare(haystack->data)) {
        haystack = haystack->next();
    }
    if (*needle == haystack->data) {
        return haystack;
    }
    else {
        return nullptr;
    }
}

void llist::exclude(std::string* s) {
    ll_node* ptr = this->head;
    for(int c=1; c < WORD_SIZE+1; c++) {
        ptr = this->head;
        while(ptr != nullptr) {
            if ((*s)[c] == ptr->data[0] || (*s)[c] == ptr->data[1] ||
                (*s)[c] == ptr->data[2] || (*s)[c] == ptr->data[3] ||
                (*s)[c] == ptr->data[4]) {
                ptr = this->remove(ptr);  // ptr gets incremented when removing from list
            }
            else
                ptr = ptr->next();
        }
    }
}

void llist::exclude_letters(std::string input) {   // this is horrifically slow, unsure how to speed it up but it works I guess...
    ll_node* ptr = this->head;
    bool goToNextWord;
    int letter = 1;
nextCharInInput: 
    for (int i = 1; i < input.length(); i++) {   // for each letter in input
        ptr = this->head;
        while (ptr != nullptr) {                                    // loop through every word in search list
            for (int letter = 0; letter < WORD_SIZE; letter++) {    // for each letter in word
                if (ptr->data[letter] == input[i]) {                // if it matches the character from input
                    ptr = this->remove(ptr);                        // remove that word from search list
                    if (ptr == nullptr)
                        goto nextCharInInput;
                    break;
                }
            }
            ptr = ptr->next();
        }
    }
}

void llist::include(std::string* s, int length) {
    ll_node* ptr;
    for(int c=0; c < length; c++) {
        ptr = this->head;
        while(ptr != nullptr) {
            if ((*s)[c] == ptr->data[0] || (*s)[c] == ptr->data[1] ||
                (*s)[c] == ptr->data[2] || (*s)[c] == ptr->data[3] ||
                (*s)[c] == ptr->data[4]) {
                ptr = ptr->next();
            }
            else
                ptr = this->remove(ptr);  // ptr gets incremented when removing from list
        }
    }
}

void llist::rm_duplicates() {
    ll_node* ptr = this->head;

    // Make empty array of zeros
    int a[ALPHABET_SIZE];
    for(int i=0; i<ALPHABET_SIZE; i++) {
        a[i] = 0;
    }

    // Get Letter Occurrence: Increment each index of the array correlating to the character being compared
    loop: while(ptr != nullptr) {
        for(int j=0; j<WORD_SIZE; j++) {
            a[ptr->data[j] - 97]++;
            if (a[ptr->data[j]-97] > 1) {
                ptr = this->remove(ptr);
                for(int i=0; i<ALPHABET_SIZE; i++) {
                    a[i] = 0;
                }
                goto loop;
            }
        }
        ptr = ptr->next();
    }
}

void llist::info() {
    ll_node* ptr = this->head;

    // Make empty array of zeros
    int num_occur[ALPHABET_SIZE];
    for(int i=0; i<ALPHABET_SIZE; i++) {
        num_occur[i] = 0;
    }

    // Get Letter Occurrence: Increment each index of the array correlating to the character being compared
    while(ptr != nullptr) {
        for(int j=0; j<WORD_SIZE; j++) {
            num_occur[ptr->data[j] - 97]++;
        }
        ptr = ptr->next();
    }

    // Get order of most to least
    list maxes;         // stores indexes of num_occur in order of max occurrences (sorted list of maxes)
    list conflicts;     // stores indexes which contain duplicate occurrence values
    int value;
    int max = -1;
    int prevmax = INT_MAX;
    int conflict = ALPHABET_SIZE;
    int max_loc = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        max = -1;
        for (int j = 0; j < ALPHABET_SIZE; j++) {   // loop through num_occur finding maxes and conflicts
            if (num_occur[j] < prevmax && num_occur[j] > max) {
                conflicts.clear();  // clear conflicts
                max = num_occur[j]; // set max
                max_loc = j;        // set max location
                conflicts.push(j);  // add to conflicts
            }
            else if (num_occur[j] == max) {
                conflicts.push(j);
            }
        }
        maxes.push(conflicts.pop());
        while (conflicts.size != 0) {       // Only loops if more than 1 conflict found
            maxes.push(conflicts.pop());
            i--;
        }
        prevmax = max;
    }

    // Print Info
    std::cout << "NUMBER OF WORDS : " << this->size << std::endl;
    for(int k=0; k<ALPHABET_SIZE; k++) {
        int h_loc = maxes.pop();
        std::cout << "  " << char(k+97) << " : " << std::setw(10) << std::left << num_occur[k] 
                << char(h_loc+97) <<  " : " << num_occur[h_loc] << std::endl;
    }
}

#endif //INC_5_LETTER_WORDLE_LINKED_LIST_H
