#include <iostream>
#include <string>
#include <fstream>

#define LETTERS 5
#include "linked_list.h"

// Functionality to add to linked list
void llist::exclude_letters(std::string input) {
    ll_node* ptr = this->head;
    bool goToNextWord;
    int letter = 1;
    while (ptr != nullptr) {   // this is horrifically slow, unsure how to speed it up but it works I guess...
        goToNextWord = false;
        for (letter = 1; letter < input.size() && !goToNextWord; letter++) {
            for (int i = 1; i < LETTERS && !goToNextWord; i++) {
                if (ptr->data[i] == input[letter]) {
                    ptr = this->remove(ptr);
                    goToNextWord = true;
                }
            }
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
        for (int i = 0; i < LETTERS && !has_c; i++) {
            if (ptr->data[i] == c) {
                has_c = true;
            }
        }
        ptr = ptr->next();
    }
    return;
}
void llist::include_correct(char c, int pos) {
    ll_node* ptr = this->head;
    while (ptr != nullptr) {
        if (ptr->data[pos] != c) {
            ptr = this->remove(ptr);
            continue;
        }
        ptr = ptr->next();
    }
}
void llist::exclude(char c, int pos) {
    ll_node* ptr = this->head;
    while (ptr != nullptr) {
        if (ptr->data[pos] == c) {
            ptr = this->remove(ptr);
            continue;
        }
        ptr = ptr->next();
    }
    return;
}
void llist::filter_results(std::string input) {
    ll_node* ptr = this->head;


    
    for (int i = 0; i < LETTERS; i++) {
        if (input[i] != '_') {
            while (ptr != nullptr) {
                if (input[i] != ptr->data[i]) {
                    ptr = this->remove(ptr);  // increments pointer after removing
                }
                else {
                    ptr = ptr->next();  // normal increment
                }
            }
            ptr = this->head;  // reset pointer to beginning of subset
        }
    }
}
llist* llist::first_filter_results(std::string input) const {
    auto* s = new llist;
    ll_node* ptr = this->head;
    for (int i = 0; i < 5; i++) {
        if (input[i] == '_' || input[i] < (char)65) {
            while (ptr != nullptr) {
                if (input[i] == ptr->data[i]) {
                    s->append(&ptr->data);
                }
                ptr = ptr->next();
            }
        }
    }
    return s;
}
void llist::filter(std::string input) {
    // loop to determine if any correct value & position (those narrow list faster than value only)
    for (int i = 0; i < LETTERS; i++) {
        if (input[i] == '_' || input[i] < (char)65)
            continue;
        else if (input[i] == toupper(input[i])) {
            this->include_correct(tolower(input[i]), i);   // use tolower because dictionary is all in lowercase
            input[i] = '_';
        }
    }
    for (int i = 0; i < LETTERS; i++) {
        if (input[i] == '_' || input[i] < (char)65)
            continue;
        else
            this->include(input[i], i);
    }
}


// Functions
bool read_in_sorted_words(llist* words){
    std::string str;
    words->init();
    std::ifstream file;
    file.open("sorted.txt");
    if (!file.is_open()) {
        std::cout << "Working directory:\n";
        system("echo %cd%");
        return false;
    }
    while (getline(file, str))  {
        words->append(&str);
    }
    return true;
}
bool read_in_unsorted_words(llist* words) {
    std::string str;
    words->init();
    std::ifstream file;
    file.open("words.txt");
    if (!file.is_open()) {
        return false;
    }
    while (getline(file, str))  {
        words->insert(&str);
    }
    return true;
}

int main() {
    llist search_list;
    std::string input;
    bool loop = true;
    search_list.init();
    if (!read_in_sorted_words(&search_list)) {
        std::cout << "Reading dictionary file failed!\nExiting..." << std::endl;
        return 1;
    }
    while(loop) {
        system("CLS");
        std::cout << "WORDLE SOLVER:\n";
        std::cout << "\nEnter a search string or one of the options below:\n";
        std::cout << "\tP : prints current searchlist (everything that meets criteria)\n";
        std::cout << "\tH : display help for using search strings\n";
        std::cout << "\tC : resets current search criteria\n";
        std::cout << "\tQ : quits this application\n";
        std::cout << "\n>> ";
        std::getline(std::cin, input);
        if(input.length() == 1) {
            switch (toupper(input[0])) {
                case 'Q':
                    loop = false;
                    break;
                case 'P':
                    system("CLS");
                    std::cout << "WORDLE SOLVER:\n\n";
                    search_list.print();
                    std::cout << "\nPress enter to continue...";
                    std::getline(std::cin, input);
                    break;
                case 'H':
                    std::cout << "\n\t__s_d  :  lowercase letters are correct but wrong placement, uppercase are correct and correct place\n";
                    std::cout << "\t.poe    :  a period before letters will exclude those letters\n";
                    std::cout << "\t#words  :  guess if a word meets current criteria\n";
                    std::cout << "\nPress enter to continue...";
                    std::getline(std::cin, input);
                    break;
                case 'C':
                    search_list.destroy();
                    if (!read_in_sorted_words(&search_list)) {
                        std::cout << "Reading dictionary file failed!\nExiting..." << std::endl;
                        std::cout << "\nPress enter to continue...";
                        std::getline(std::cin, input);
                        return 1;
                    }
                    break;
                default:
                    std::cout << "\nERROR: Invalid input given  :  " << input << "\n";
                    std::cout << "\nPress enter to continue...";
                    std::getline(std::cin, input);
            }
        }
        else if (input.length() == 5 && input[0] != '.') {
            search_list.filter(input);
        }
        else if (input.length() == 6 && input[0] == '#') {
            std::string guess;
            for (int i = 0; i < LETTERS && i < input.length(); i++)
                guess.at(i) = input.at(i+1);   // remove first character (#) from string then search
            if (search_list.search(&guess) != nullptr)
                std::cout << "SUCCESS: " << input << " found in search list!\n";
            else
                std::cout << "FAILURE: " << input << " not found in search list!\n";
        }
        else if (input[0] == '.') {
            search_list.exclude_letters(input);
        }
        else {
            std::cout << "\nERROR: Invalid input given  :  " << input << "\n";
            std::cout << "Press enter to continue...";
            std::getline(std::cin, input);
        }
    }

    // End of program
    search_list.destroy();
    return 0;
}