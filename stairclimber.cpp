/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Aryaman Srivastava
 * Date        : 9/27/23
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honors System
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector< vector<int> > get_ways(int num_stairs) {
    if (num_stairs <= 0) {
        return vector<vector<int>> {{}}; //base case, if the number of stairs is 0, return an empty vector
    } 
    vector<vector<int>> ways;
    for (int i = 1; i < 4; i++) { //iterate over all possible ways we can climb the stairs(1 step, 2 step, 3 step at a time)
        if (num_stairs >= i) { //check that we can climb the given number of stairs using the given number of steps at a time(we can't climb take 3 steps to climb 2 stairs for example)
            vector<vector<int>> result = get_ways(num_stairs - i); 
            //prepend i to all the solutions in result
            for (size_t j = 0; j < result.size(); j++) {
                result[j].insert(result[j].begin(), i); 
            }
            //insert result into ways(what we want to return)
            ways.insert(ways.end(), result.begin(), result.end()); 
        }
    }

    return ways;
}

void display_ways(const vector< vector<int> > &ways) {
    int highest_label = ways.size();
    int digits = 0;
    while (highest_label != 0) { //find out how many digits there are in the total number of ways we can climb the stairs
        highest_label/=10;
        digits++;
    }
    for (size_t i = 0; i < ways.size(); i++) { //iterate over all the ways we can climb the stairs
        cout << setw(digits) << i + 1 << ". " << "["; //print the label with the correct right-alignment
        for (size_t j = 0; j < ways[i].size(); j++) { //loop in each individual vector element and print each one
            if (j == ways[i].size() - 1) { //if we are at the end of the vector, don't print a ',' otherwise print a ','
                cout << ways[i][j];
            } else {
                cout << ways[i][j] << ", ";
            }
        }
        cout << "]" << endl; //print the final bracket for each solution
    }
}

int main(int argc, char * const argv[]) {
    if (argc != 2) { //if the user put incorrect number of arguments, print an error message
        cerr << "Usage: " << argv[0] << " <number of stairs>" << endl;
        return 1;
    }
    int num_stairs;
    istringstream iss;
    iss.str(argv[1]);
    if (!(iss >> num_stairs) || num_stairs <= 0) { //if the argument is invalid(non-positive integer amount of stairs, or not even an integer), print an error message
        cerr << "Error: Number of stairs must be a positive integer." << endl;
        return 1;
    }
    if (num_stairs == 1) { //print message stating how many ways there are to climb the given number of stairs 
        cout << get_ways(num_stairs).size() << " way to climb " << num_stairs << " stair." << endl;
    } else {
        cout << get_ways(num_stairs).size() << " ways to climb " << num_stairs << " stairs." << endl;
    }
    display_ways(get_ways(num_stairs)); //print all the ways there are to climb the given number of stairs
    return 0;
}
