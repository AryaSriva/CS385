/*******************************************************************************
* Filename: sqrt.cpp
* Author : Aryaman Srivastava
* Version : 1.0
* Date : 9/13/23
* Description: Computes the square root of a given double using Newton's method
* Pledge : I pledge my honor that I have abided by the Stevens Honors System
******************************************************************************/
#include <iomanip>
#include <iostream>
#include <sstream>
#include <limits>
#include <math.h>
using namespace std;

double sqrt(double num, double epsilon) {
    if (num < 0) {
        return numeric_limits<double>::quiet_NaN(); //if the number is less than 0, then its square root does not exist
    } else if (num == 0 || num == 1) {
        return num; //if the number is 0 or 1, then its square root will be the number itself
    } else {
        double last_guess = num; 
        double next_guess = (last_guess + 1)/2;
        while (abs(last_guess - next_guess) > epsilon) { //execute Newton's method until absolute difference between last_guess and next_guess is less than or equal to epsilon
            last_guess = next_guess;
            next_guess = (last_guess + num/last_guess)/2;
        }
        return next_guess;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) { //check if user has given correct number of arguments
        cerr << "Usage: " << argv[0] << " <value> [epsilon]" << endl;
        return 1;
    }
    double n;
    istringstream iss;
    iss.str(argv[1]); //check if the value given is a valid double
    if(!(iss >> n)) {
        cerr << "Error: Value argument must be a double." << endl;
        return 1;
    }
    iss.clear();
    if (argc == 3) {
        double m;
        iss.str(argv[2]);
        if (!(iss >> m) || m <= 0) { //check if the epsilon value given is a valid positive double
            cerr << "Error: Epsilon argument must be a positive double." << endl;
            return 1;
        }
        cout << fixed << setprecision(8) << sqrt(n, m) << endl; //output the result
    } else {
        cout << fixed << setprecision(8) << sqrt(n, pow(10,-7)) << endl; //output the result 
    }
    return 0;
}