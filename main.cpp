#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "wordle_ll.h"


int main() {
    wordle_ll search_list;
    search_list.init();
    std::string input;
    bool loop = true;

    while(loop) {
        system("CLS");
        std::cout << "WORDLE SOLVER:\n";
        std::cout << "\nEnter a search string or one of the options below:\n";
        std::cout << "\tP : prints current searchlist (everything that meets criteria)\n";
        std::cout << "\tS : display summary of searchlist (number items)\n";
        std::cout << "\tD : perform detailed analytics on the current searchlist\n";
        std::cout << "\tN : change number of letters for this game\n";
        std::cout << "\tC : resets current search criteria\n";
        std::cout << "\tQ : quits this application\n";
        std::cout << "\tH : display help for using search strings\n";
        std::cout << "\tOr you can enter a search string directly:\n";
        std::cout << "\n>> ";
        std::getline(std::cin, input);
        if(input.length() == 1) {
            switch (toupper(input[0])) {
                case 'P':
                    system("CLS");
                    std::cout << "WORDLE SOLVER:\n\n";
                    search_list.print();
                    std::cout << "\nPress enter to continue...";
                    std::getline(std::cin, input);
                    break;
                
                case 'S':
                    search_list.info();
                    break;
                case 'D':
                    search_list.analyze();
                    break;
                case 'N':
                    search_list.setYear();
                    break;
                case 'C':
                    search_list.destroy();
                    if (!search_list.read_in_sorted_words()) {
                        std::cout << "Reading dictionary file failed!\nExiting..." << std::endl;
                        std::cout << "\nPress enter to continue...";
                        std::getline(std::cin, input);
                        return 1;
                    }
                    break;
                case 'Q':
                    loop = false;
                    break;
                case 'H':
                    std::cout << "\n\t__s_D   :  lowercase letters are correct but wrong place, uppercase are correct and in correct place\n";
                    std::cout << "\t.itna   :  a period before letters will exclude words containing those letters\n";
                    std::cout << "\t#posed  :  guess if a word meets current criteria (displays success/failure)\n";
                    std::cout << "\nPress enter to continue...";
                    std::getline(std::cin, input);
                    break;
                default:
                    std::cout << "\nERROR: Invalid input given  :  " << input << "\n";
                    std::cout << "\nPress enter to continue...";
                    std::getline(std::cin, input);
            }
        }
        else if (input.length() == 5 && input[0] != '.') {
            search_list.parse(input);
        }
        else if (input.length() == 6 && input[0] == '#') {
            std::string guess = "     ";
            for (int i = 0; i < search_list.WORD_SIZE && i < input.length(); i++)
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