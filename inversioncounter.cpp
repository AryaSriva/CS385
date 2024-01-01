/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Aryaman Srivastava
 * Version     : 1.0
 * Date        : 10/25/23
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honors System
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);
static long merge(int array[], int scratch[], int low, int mid, int high);

/**
 * Counts the number of inversions in an array in Theta(n^2) time using two nested loops.
 */
long count_inversions_slow(int array[], int length) {
    // TODO
    long numInversions = 0;
    for (int i = 0; i < length; i++) { //double for loop to count inversions
        for (int j = i + 1; j < length; j++) {
            if (array[i] > array[j]) { 
                numInversions++;
            }
        }
    }
    return numInversions;
}

/**
 * Counts the number of inversions in an array in Theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
    // TODO
    // Hint: Use mergesort!
    int* scratch = new int[length]; //initialize a new empty array to use in mergesort
    long answer = mergesort(array, scratch, 0, length - 1); //store the answer before deleting the new array
    delete [] scratch; 
    return answer;
}

static long mergesort(int array[], int scratch[], int low, int high) {
    // TODO
    if (low < high) {
        int mid = low + (high - low)/2;
        long a = mergesort(array, scratch, low, mid); //merge sort algorithm
        long b = mergesort(array, scratch, mid + 1, high);
        return a + b + merge(array, scratch, low, mid, high); //return the total number of inversions 
    } else {
        return 0; //base case
    }
}
static long merge(int array[], int scratch[], int low, int mid, int high) {
    int i1 = low;
    int i2 = mid + 1;
    int i = low;
    long numInversions = 0;
    while (i1 <= mid && i2 <= high) { //perform the merge algorithm
        if (array[i1] <= array[i2]) {
            scratch[i++] = array[i1++];
        } else {
            scratch[i++] = array[i2++];
            numInversions += (mid - i1 + 1); //increment the inversion counter 
        }
    }
    while (i1 <= mid) {
        scratch[i++] = array[i1++];
    }
    while (i2 <= high) {
        scratch[i++] = array[i2++];
        numInversions += (mid - i1 + 1); //increment the inversion counter
    }
    for (int i = low; i <= high; i++) { //copy the original array into the new(scratch) array
        array[i] = scratch[i];
    }
    return numInversions;
}

int main(int argc, char *argv[]) {
    // TODO: parse command-line argument
    if (argc > 2) { //check if the user passed more than 1 argument
        cerr << "Usage: " << argv[0] << " [slow]" << endl;
        return 1;
    } else if (argc == 2) {
        if (strcmp(argv[1], "slow") != 0) { //check if the user passed a valid argument
            cerr << "Error: Unrecognized option '" << argv[1] << "'." << endl;
            return 1;
        }
    }
    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }
    if (values.size() == 0) { //check if the user inputted any numbers
        cerr << "Error: Sequence of integers not received." << endl;
        return 1;
    }
    // TODO: produce output
    if (argc == 2) { //check if we need to use the fast or slow method and output accordingly
        cout << "Number of inversions (slow): " << count_inversions_slow(&values[0], values.size()) << endl;
    } else {
        cout << "Number of inversions (fast): " << count_inversions_fast(&values[0], values.size()) << endl;
    }

    return 0;
}
