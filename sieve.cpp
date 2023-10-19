/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Aryaman Srivastava
 * Date        : 9/12/23
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honors System
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    sieve();
}

void PrimesSieve::display_primes() const {
    // TODO: write code to display the primes in the format specified in the
    // requirements document.
    const int max_prime_width = num_digits(max_prime_),
       primes_per_row = 80 / (max_prime_width + 1); //find the width of the biggest prime number and how many primes we can fit in each row
    cout << "\nNumber of primes found: " << num_primes_ << endl;
    cout << "Primes up to " << limit_ << ":" << endl;
    int num_primes = 0;
    if (num_primes_ <= primes_per_row) { //case if the number of primes can fit on 1 row
        for (int i = 2; i <= limit_; i++) {
            if (is_prime_[i]) {
                num_primes++;
                if (num_primes == num_primes_) { //if we've reached the last prime, just end the line
                    cout << i << endl;
                } else {
                    cout << i << " "; //otherwise, print each prime with a space in between
                }
            }
        }
    } else {
        num_primes = 1; //case if the number of primes will take up multiple rows
        cout << setw(max_prime_width) << "2";
        for (int i = 3; i <= limit_; i++) {
            if (is_prime_[i]) {
                if (num_primes == primes_per_row) { //if we've reached the max number of primes that can be printed on one line, then end line and go to the next line
                    cout << endl;
                    num_primes = 0;
                } else {
                    cout << " "; //otherwise just print a space
                }
                cout << setw(max_prime_width) << i; //print each prime with the appropriate amount of whitespace padding to right-align the prime
                num_primes++;
            }
        }
    }
    
}

void PrimesSieve::sieve() {
    // TODO: write sieve algorithm
    // All instance variables must be initialized by the end of this method.
    for (int i = 2; i <= limit_; i++) { //first set all the elements to true
        is_prime_[i] = true;
    }
    for (int i = 2; i <= sqrt(limit_); i++) { //perform the sieve algorithm, which will make only the prime elements true
        if (is_prime_[i]) {
            for (int j = pow(i, 2); j <= limit_; j += i) {
                is_prime_[j] = false;
            }
        }
    }
    //Initialize the instance variables
    num_primes_ = 0;
    max_prime_ = 0;
    for (int i = limit_; i >= 2; i--) { //loop to find the number of primes in the array and the maximum prime
        if (is_prime_[i]) {
            num_primes_++;
            if (i > max_prime_) {
                max_prime_ = i;
            }
        }
    }
}

int PrimesSieve::num_digits(int num) {
    // TODO: write code to determine how many digits are in an integer
    // Hint: No strings are needed. Keep dividing by 10.
    int digits = 0; 
    while (num != 0) { //keep dividing the number by 10 until it reaches 0 and increment digits each time a division is performed
        num /= 10;
        digits++;
    }
    return digits;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

    // TODO: write code that uses your class to produce the desired output.
    PrimesSieve primes(limit); //create a PrimesSieve object with the given limit
    primes.display_primes(); //call the method to output all the primes
    return 0;
}



