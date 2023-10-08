#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "linked_list.h"

// Functions
bool read_in_sorted_words(llist* words){
    std::string str;
    words->init();
    std::ifstream file;
    std::stringstream filename;
    filename << "Dictionaries/" << LETTERS << ".txt";
    file.open(filename.str());
    if (!file.is_open()) {
        std::cout << "Working directory:\n";
        system("echo %cd%");
        std::cout << "\nFilename: " << filename.str() << std::endl;
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
void setYear(llist* search_list) {
    std::cout << "How many letters are you playing with?  : ";
    std::cin >> LETTERS; std::cin.clear(); std::cin.ignore();
    if (!read_in_sorted_words(search_list)) {
        std::cout << "Reading dictionary file failed!\nExiting..." << std::endl;
        exit(1);
    }
}

int main() {
    llist search_list;
    std::string input;
    bool loop = true;
    search_list.init();
    setYear(&search_list);
    while(loop) {
        system("CLS");
        std::cout << "WORDLE SOLVER:\n";
        std::cout << "\nEnter a search string or one of the options below:\n";
        std::cout << "\tP : prints current searchlist (everything that meets criteria)\n";
        std::cout << "\tH : display help for using search strings\n";
        std::cout << "\tS : display summary of searchlist (number items)\n";
        std::cout << "\tN : change number of letters for this game\n";
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
                    std::cout << "\n\t__s_D   :  lowercase letters are correct but wrong place, uppercase are correct and in correct place\n";
                    std::cout << "\t.itna   :  a period before letters will exclude words containing those letters\n";
                    std::cout << "\t#posed  :  guess if a word meets current criteria (displays success/failure)\n";
                    std::cout << "\nPress enter to continue...";
                    std::getline(std::cin, input);
                    break;
                case 'S':
                    std::cout << "\nSize of searchable list  : " << search_list.size << " words\n";
                    std::cout << "\nPress enter to continue...";
                    std::getline(std::cin, input);
                    break;
                case 'N':
                    setYear(&search_list);
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
            std::string guess = "     ";
            for (int i = 0; i < LETTERS && i < input.length(); i++)
                guess.at(i) = input.at(i+1);   // remove first character (#) from string then search
            if (search_list.search(&guess) != nullptr)
                std::cout << "SUCCESS: " << input << " found in search list!\n";
            else
                std::cout << "FAILURE: " << input.substr(1,input.length()-1) << " not found in search list!\n";
            std::cout << "Press enter to continue...";
            std::getline(std::cin, input);
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