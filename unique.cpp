/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Aryaman Srivastava
 * Date        : 9/27/23
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honors System
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
    for (const auto x: s) {
        if (!islower(x) || !isalpha(x)) { //check if the character is a lower case letter and its an alphabetic character(no number/symbol)
            return false;
        }
    }
    return true;
}

bool all_unique_letters(const string &s) {
    unsigned int vector = 0; //initialize bitset
    for (const auto x: s) {
        if ((vector >> (x - 'a') & 1) == 1) { //rightshift the bit till it becomes the LSB and mask all other bits, then and it with 1. 
            return false; //if the resulting number is 1, that means we found a duplicate
        } else {
            vector += 1 << (x - 'a'); //if not, flip the bit to 1 by adding 1 left shifted by the amount the character is away from 'a' 
        }
    }
    return true;
}

int main(int argc, char * const argv[]) {
    if (argc != 2) { //check that the user inputted the correct number of arguments, otherwise print an error message
        cerr << "Usage: " << argv[0] << " <string>" << endl;
        return 1;
    }
    string input = argv[1]; //take the argument and store it into a variable
    if(!is_all_lowercase(input)) { //check that the argument is valid(is all lowercase letters) and print an error message if otherwise
        cerr << "Error: String must contain only lowercase letters." << endl; 
        return 1;
    }
    if (all_unique_letters(input)) { //check if the given string contains all unique letters, and print the corresponding finding(is unique or not)
        cout << "All letters are unique." << endl;
    } else {
        cout << "Duplicate letters found." << endl;
    }
    return 0;
}
