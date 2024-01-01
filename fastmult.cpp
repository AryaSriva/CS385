/*******************************************************************************
 * Name        : fastmult.cpp
 * Author      : Aryaman Srivastava
 * Date        : 11/9/23
 * Description : Karatsuba Algorithm
 * Pledge      : I pledge my honor that I have abided by the Stevens Honors System
 ******************************************************************************/
#include <sstream> 
#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;

string add(const string& a, const string& b) {
    //addition helper 
    string result;
    size_t maxLength = max(a.size(), b.size()); //make a and b the same length
    string intA(maxLength - a.size(), '0');
    string intB(maxLength - b.size(), '0');
    intA += a;
    intB += b;
    int carryin = 0;
    for (int i = intA.size() - 1; i >= 0; i--) {
        int a_digit = intA[i] - '0';
        int b_digit = intB[i] - '0';
        int sum = carryin + a_digit + b_digit;
        result = to_string(sum % 10) + result; //prepend the summation and calculate the carry for each digit addition
        carryin = sum/10;
    }
    if (carryin > 0) { //check if we have a carry after performing the addition 
        result = to_string(carryin) + result;
    }
    return result;
}
string subtract(const string& a, const string& b) {
    //subtraction helper
    string result;
    size_t maxLength = max(a.size(), b.size()); //make a and b the same length
    string intA(maxLength - a.size(), '0');
    string intB(maxLength - b.size(), '0');
    intA += a;
    intB += b;
    int carry = 0;
    for (int i = intA.size() - 1; i >= 0; i--) {
        int a_digit = intA[i] - '0';
        int b_digit = intB[i] - '0';
        int difference = a_digit - b_digit - carry; //calculate the difference 
        if (difference < 0) { //check if we need to carry/borrow from the next digit
            difference += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result = to_string(difference) + result; //prepend the difference for each digit subtraction
    }
    return result;
}
string multiply(const string& a, const string& b) {
    //multiplication helper(performs one digit multiplication)
    int intA = a[0] - '0';
    int intB = b[0] - '0';
    return to_string(intA*intB);
}

string karatsuba(const string& a, const string& b) {
    if (a.size() == 1 && b.size() == 1) { //base case: one digit multiplication
        return multiply(a, b);
    } else {
        //make a and b the same length of power 2 by padding 0s 
        size_t length = max(a.size(), b.size()); 
        size_t pow2Length = 1;
        while (pow2Length < length) {
            pow2Length*=2;
        }
        string tempA(pow2Length - a.size(), '0');
        string tempB(pow2Length - b.size(), '0'); 
        tempA += a;
        tempB += b;
        //karatsuba algorithm described in lecture
        string a1 = tempA.substr(0, tempA.size()/2);
        string a0 = tempA.substr(tempA.size()/2); 
        string b1 = tempB.substr(0, tempB.size()/2);
        string b0 = tempB.substr(tempB.size()/2);
        string c0 = karatsuba(a0, b0); 
        string c2 = karatsuba(a1, b1);
        string c1 = karatsuba(add(a1, a0), add(b1, b0));
        string digits(tempA.size(), '0');
        string digits2(tempA.size()/2, '0');
        string intermediate = subtract(subtract(c1, c2), c0);
        return add(add(c2 + digits, intermediate + digits2), c0);
    }
}

int main(int argc, char * const argv[]) {
    string a;
    string b;
    istringstream iss;
    iss.str(argv[1]);
    iss >> a;
    iss.clear();
    iss.str(argv[2]);
    iss >> b; //call karatsuba algorithm with command line args
    string solution = karatsuba(a, b);
    string outstr;
    size_t i = 0;
    while (solution[i] == '0' && i < solution.size() - 1) { //remove leading zeroes from the solution 
        i++;
    }
    outstr += solution.substr(i); //printout result
    cout << outstr << endl;
    return 0;
}