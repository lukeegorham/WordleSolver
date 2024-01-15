/***
 * Project :  Wordle Helper
 * Author  :  Luke Gorham
 * Date    :  21 May 2022
 */

#ifndef INC_WORDLE_LINKED_LIST_H
#define INC_WORDLE_LINKED_LIST_H

#include <string>
#include <iostream>
#include <iomanip>
#include "simple_stack.hpp"

#define ALPHABET_SIZE 26


/**
 * @brief Node for linked list 'wordle_ll'. Each node stores next and prev nodes
 * along with data (which is a std::string. Note that next (n) and prev (p)
 * members are private and must be set with the setnext and prev =) funcs.
 * They can be accessed using next and prev.
 */
struct ll_node {
    ll_node* next = nullptr;
    ll_node* prev = nullptr;
    std::string data;
};

void delete_nodes(ll_node* n) {
    if (n == nullptr)
        return;
    ll_node* n2 = n->next;
    while (n2 != nullptr) {
        free(n);
        n = n2;
        n2 = n2->next;
    }
    free(n);
}


/**
 * @brief Main linked list for storing words. Lists are double-linked
 * and track size. Each list has several helper functions (see declarations)
 * for more info.
 */
struct wordle_ll {
    ll_node* head = nullptr;
    ll_node* tail = nullptr;
    int size = 0;
    int WORD_SIZE = 5;

    void setYear();
    bool read_in_unsorted_words();
    bool read_in_sorted_words();
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
    void parse(std::string);
    void analyze();
    void include(std::string*, int);
    void include(char, int);
    void include_correct(char, int);
    void exclude(std::string*);
    void exclude(char, int);
    void exclude_letters(std::string);
    wordle_ll* first_filter_results(std::string) const;
    void rm_duplicates();
    void info();
};
void wordle_ll::setYear() {
    std::cout << "How many letters are you playing with?  : ";
    std::cin >> this->WORD_SIZE; std::cin.clear(); std::cin.ignore();
    if (!read_in_sorted_words()) {
        std::cout << "Reading dictionary file failed!\nExiting..." << std::endl;
        exit(1);
    }
}
bool wordle_ll::read_in_unsorted_words() {
    std::string str;
    std::ifstream file;
    file.open("words.txt");
    if (!file.is_open()) {
        return false;
    }
    while (getline(file, str)) {
        insert(&str);
    }
    return true;
}
bool wordle_ll::read_in_sorted_words() {
    std::string str;
    //words->init();
    std::ifstream file;
    std::stringstream filename;
    filename << "Dictionaries/" << WORD_SIZE << ".txt";
    file.open(filename.str());
    if (!file.is_open()) {
        std::cout << "Working directory:\n";
        system("echo %cd%");
        std::cout << "\nFilename: " << filename.str() << std::endl;
        return false;
    }
    while (getline(file, str)) {
        //words->append(&str);
        append(&str);
    }
    return true;
}
void wordle_ll::init() {
    this->head = nullptr;
    this->tail = nullptr;
    this->size = 0;

    this->setYear();
}
void wordle_ll::append(std::string* s) {
    auto* n = new ll_node;
    if(this->head == nullptr) {
        this->head = n;
        this->tail = n;
        this->head->data = *s;
    }
    else {
        this->tail->next =n;
        n->prev =this->tail;
        this->tail = n;
    }
    n->data = *s;
    this->size++;
}
void wordle_ll::remove_head() {
    ll_node* ptr = this->head;
    if (this->head == nullptr)
        return;
    this->head = this->head->next;
    if (this->head != nullptr)
        this->head->prev =nullptr;
    else
        this->tail = nullptr;
    delete(ptr);
    this->size--;
}
void wordle_ll::insert(std::string* str) {
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
            newnode->next =ptr;
            ptr->prev =newnode;
            return;
        }
        // get to proper location
        while(ptr->data < *str && ptr->next != nullptr) {
            ptr = ptr->next;
        }
        if(ptr->data == *str) {  // disregard if already exists
            delete(newnode);
            return;
        }
        else {      // insert after ptr
            newnode->data = *str;
            newnode->next =ptr->next;
            newnode->prev =ptr;
            if(ptr->next == nullptr) {
                this->tail = newnode;
            }
            else {
                ptr->next->prev =newnode;
            }
            ptr->next =newnode;
        }
    }
}
ll_node* wordle_ll::remove(ll_node* d) {
    ll_node* retval;
    if(d == this->head) {  // case for deleting head
        remove_head();
        return this->head;
    }
    d->prev->next =d->next;
    if (d->next != nullptr) {  // case for deleting tail
        d->next->prev = d->prev;
        retval = d->next;
        delete(d);
        this->size--;
        return retval;
    }
    else {  // case for deleting in middle of list
        this->tail = d->prev;
        delete(d);
        this->size--;
        return nullptr;
    }
}
void wordle_ll::position_exclude(std::string* s) {
    ll_node* ptr;
    std::string str_to_pass = "";
    bool b = false;
    for(int c=0; c < this->WORD_SIZE; c++) {
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
                ptr = ptr->next;
        }
        if(!b) {
            // Ensure (*s)[c] is included if not a '*'
            str_to_pass = (*s).substr(c,1);
            this->include(&str_to_pass, 1);
        }
    }
}
void wordle_ll::destroy() {
    delete_nodes(this->head);
}
void wordle_ll::print() const {
    ll_node* ptr = this->head;
    std::cout << "WORDS IN LIST [" << this->size << "]:\n";
    while(ptr != nullptr) {
        std::cout << " - " << ptr->data << std::endl;
        ptr = ptr->next;
    }
    std::cout << "Total Size: " << this->size << std::endl;
}
void wordle_ll::printhistory() const {
    ll_node* ptr = this->head;
    std::cout << "Filter History:\n";
    while(ptr != nullptr) {
        std::cout << "   " << ptr->data << std::endl;
        ptr = ptr->next;
    }
    std::cout << "End of History.\n";
}
void wordle_ll::parse(std::string input) {
    char c;
    // loop to determine if any correct value & position (those narrow list faster than value only)
    for (int i = 0; i < this->WORD_SIZE; i++) {
        c = input[i];
        if (c == '_') {
            continue;
        }
        else if (c > 96 && c < 123) {
            this->include(input[i], i);
        }
        else if (c > 64 && c < 91) {
            this->include_correct(input[i]+32, i);   // use tolower because dictionary is all in lowercase
        }
    }
}
void wordle_ll::analyze() {
    // Initialize Variables
    ll_node* ptr;
    int num_in_word;
    int with_without_total[3][ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {   // will store number of words WITH each letter
        with_without_total[0][i] = 0;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {   // will store number of words WITHOUT each letter
        with_without_total[1][i] = 0;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {   // will store the TOTAL number of each letter
        with_without_total[2][i] = 0;
    }

    // Loop through, counting words with and without the letter
    for (int i = 0; i < ALPHABET_SIZE; i++) {               // loops through each letter of the alphabet
        ptr = this->head;
        num_in_word = 0;
        while (ptr != nullptr) {                            // loops through each word in the dictionary
            for (int j = 0; j < this->WORD_SIZE; j++) {     // loops through each letter in each word
                if (ptr->data[j] == i + 97) {
                    num_in_word += 1;
                }
            }
            if (num_in_word != 0) {
                with_without_total[2][i] += num_in_word;
                with_without_total[0][i] += 1;
            }
            else {
                with_without_total[1][i] += 1;
            }
            ptr = ptr->next;
        }
    }

    // Display letters, how many words with, without, and total number of occurrences
    std::cout << "\t" << "LETTER:" << std::right << std::setw(6) << "WITH" << std::setw(6) << "W/O" << std::setw(6) << "TOTAL\n";
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        std::cout << std::left << "\t" << std::setw(6) << (char)(i + 65) << ":" << std::right
            << std::setw(6) << with_without_total[0][i] << "   "
            << std::setw(6) << with_without_total[1][i] << "   "
            << std::setw(6) << with_without_total[2][i] << "\n";
    }

    // Freeze Displaying Info
    std::string input;
    std::cout << "\nPress enter to continue...";
    std::getline(std::cin, input);
}
void wordle_ll::include_correct(char c, int pos) {
    ll_node* ptr = this->head;
    while (ptr != nullptr) {
        if (ptr->data[pos] != c) {
            ptr = this->remove(ptr);
            if (ptr == nullptr)
                break;
            continue;
        }
        ptr = ptr->next;
    }
}
void wordle_ll::include(char c, int pos) {
    exclude(c, pos);   // cannot be in position, otherwise would be correct
    ll_node* ptr = this->head;
    bool has_c;
    include_loop: while (ptr != nullptr) {
        has_c = false;
        for (int i = 0; i < this->WORD_SIZE && !has_c; i++) {
            if (ptr->data[i] == c) {
                has_c = true;
                break;
            }
            continue;
        }
        if (!has_c) {
            ptr = this->remove(ptr);
            if (ptr == nullptr)
                break;
            goto include_loop;
        }
        ptr = ptr->next;
    }
    return;
}
void wordle_ll::exclude(char c, int pos) {
    ll_node* ptr = this->head;
    while (ptr != nullptr) {
        if (ptr->data[pos] == c) {
            ptr = this->remove(ptr);
            if (ptr == nullptr)
                break;
            continue;
        }
        ptr = ptr->next;
    }
    return;
}
void wordle_ll::filter_results(std::string input) {
    ll_node* ptr = this->head;
    for(int i=0; i<this->WORD_SIZE; i++) {
        if(input[i] != '_') {
            while(ptr != nullptr) {
                if(input[i] != ptr->data[i]) {
                    ptr = this->remove(ptr);  // increments pointer when removing
                }
                else{
                    ptr = ptr->next;
                }
            }
            ptr = this->head;  // reset pointer to beginning of subset
        }
    }
}
wordle_ll* wordle_ll::first_filter_results(std::string input) const {
    auto* s = new wordle_ll;
    ll_node* ptr = this->head;
    for(int i=0; i<this->WORD_SIZE; i++) {
        if(input[i] != '_') {
            while(ptr != nullptr) {
                if(input[i] == ptr->data[i]) {
                    s->append(&ptr->data);
                }
                ptr = ptr->next;
            }
        }
    }
    return s;
}
ll_node* wordle_ll::search(std::string* str) const {
    /**
     * Note: this function can be used for implementing different
     * kinds of search algorithms, but for most applications and
     * dictionaries with only several thousand words, a linear
     * search algorithm is sufficiently fast and poses no averse
     * wait times for the user.
     */
    return wordle_ll::linsearch(str, this->head);
}
ll_node* wordle_ll::linsearch(std::string* needle, ll_node* haystack) {
    if ((*needle).empty() || haystack == nullptr) {
        return nullptr;
    }
    // Traverse to get to desired location
    while(haystack->next != nullptr && 0 < (*needle).compare(haystack->data)) {
        haystack = haystack->next;
    }
    if (*needle == haystack->data) {
        return haystack;
    }
    else {
        return nullptr;
    }
}
void wordle_ll::exclude(std::string* s) {
    ll_node* ptr = this->head;
    for(int c=1; c < this->WORD_SIZE+1; c++) {
        ptr = this->head;
        while(ptr != nullptr) {
            if ((*s)[c] == ptr->data[0] || (*s)[c] == ptr->data[1] ||
                (*s)[c] == ptr->data[2] || (*s)[c] == ptr->data[3] ||
                (*s)[c] == ptr->data[4]) {
                ptr = this->remove(ptr);  // ptr gets incremented when removing from list
            }
            else
                ptr = ptr->next;
        }
    }
}
void wordle_ll::exclude_letters(std::string s) {
    ll_node* ptr;
    bool removed = false;
    for (int c = 1; c < s.length(); c++) {
        ptr = this->head;
        while (ptr != nullptr) {
            for (int l = 0; l < this->WORD_SIZE; l++) {
                if (s[c] == ptr->data[l]) {
                    ptr = this->remove(ptr);
                    removed = true;
                    break;
                }
            }
            if (removed) {
                removed = false;
            }
            else {
                ptr = ptr->next;
            }
        }
    }
}
void wordle_ll::include(std::string* s, int length) {
    ll_node* ptr;
    for(int c=0; c < length; c++) {
        ptr = this->head;
        while(ptr != nullptr) {
            if ((*s)[c] == ptr->data[0] || (*s)[c] == ptr->data[1] ||
                (*s)[c] == ptr->data[2] || (*s)[c] == ptr->data[3] ||
                (*s)[c] == ptr->data[4]) {
                ptr = ptr->next;
            }
            else
                ptr = this->remove(ptr);  // ptr gets incremented when removing from list
        }
    }
}
void wordle_ll::rm_duplicates() {
    ll_node* ptr = this->head;

    // Make empty array of zeros
    int a[ALPHABET_SIZE];
    for(int i=0; i<ALPHABET_SIZE; i++) {
        a[i] = 0;
    }

    // Get Letter Occurrence: Increment each index of the array correlating to the character being compared
    loop: while(ptr != nullptr) {
        for(int j=0; j<this->WORD_SIZE; j++) {
            a[ptr->data[j] - 97]++;
            if (a[ptr->data[j]-97] > 1) {
                ptr = this->remove(ptr);
                for(int i=0; i<ALPHABET_SIZE; i++) {
                    a[i] = 0;
                }
                goto loop;
            }
        }
        ptr = ptr->next;
    }
}
void wordle_ll::info() {
    ll_node* ptr = this->head;

    // Make empty array of zeros
    int num_occur[ALPHABET_SIZE];
    for(int i=0; i<ALPHABET_SIZE; i++) {
        num_occur[i] = 0;
    }

    // Get Letter Occurrence: Increment each index of the array correlating to the character being compared
    while(ptr != nullptr) {
        for(int j=0; j<this->WORD_SIZE; j++) {
            num_occur[ptr->data[j] - 97]++;
        }
        ptr = ptr->next;
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

    // Freeze Displaying Info
    std::string input;
    std::cout << "\nPress enter to continue...";
    std::getline(std::cin, input);
}

#endif //INC_WORDLE_LINKED_LIST_H
