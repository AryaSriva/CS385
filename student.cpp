/*******************************************************************************
* Filename: student.cpp
* Author : Aryaman Srivastava
* Version : 1.0
* Date : 9/15/23
* Description: Implements the Student Class and subsequent methods to create a vector of given students
* Pledge : I pledge my honor that I have abided by the Stevens Honors System
******************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std; 

class Student {
    public:
        /**
        * Constructor for the Student class, uses a list initializer
        */
        Student(string first, string last, float gpa, int id) : first_{first}, last_{last}, gpa_{gpa}, id_{id} { }
        /*
        * Returns the student's full name
        */
        string full_name() const {
            return first_ + " " + last_;
        }
        /*
        *Returns the student's id
        */
        int id() const {
            return id_;
        }
        /*
        *Returns the student's gpa
        */
        float gpa() const {
            return gpa_;
        }
        /*
        * prints out the student's information in the following format <first name> <last name> <GPA> <ID>
        */
        void print_info() const {
            cout << full_name() << ", " << "GPA: " << fixed << setprecision(2) << gpa() << ", " << "ID: " << id() << endl;
        }

    private:
        /*
        * Student class attributes: a first name, last name, gpa, and ID number
        */
        string first_;
        string last_;
        float gpa_;
        int id_; 

};
/**
* Takes a vector of Student objects, and returns a new vector
* with all Students whose GPA is < 1.0.
*/
vector<Student> find_failing_students(const vector<Student> &students) {
    vector<Student> failing_students;
    // Iterates through the students vector, appending each student whose gpa is
    // less than 1.0 to the failing_students vector.
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].gpa() < 1.0) {
            failing_students.push_back(students[i]);
        }
    }
    return failing_students;
}
/**
* Takes a vector of Student objects and prints them to the screen.
*/
void print_students(const vector<Student> &students) {
// Iterates through the students vector, calling print_info() for each student.
    for (size_t i = 0; i < students.size(); i++) {
        students[i].print_info();
    }
}
/**
* Allows the user to enter information for multiple students, then
* find those students whose GPA is below 1.0 and prints them to the
* screen. */
int main() {
    string first_name, last_name;
    float gpa;
    int id;
    char repeat;
    vector<Student> students;
    do {
        cout << "Enter student's first name: ";
        cin >> first_name;
        cout << "Enter student's last name: ";
        cin >> last_name;
        gpa = -1;
        while (gpa < 0 || gpa > 4) {
            cout << "Enter student's GPA (0.0-4.0): ";
            cin >> gpa;
        }
        cout << "Enter student's ID: ";
        cin >> id;
        students.push_back(Student(first_name, last_name, gpa, id));
        cout << "Add another student to database (Y/N)? ";
        cin >> repeat;
    } while (repeat == 'Y' || repeat == 'y');
    cout << endl << "All students:" << endl;
    print_students(students);
    cout << endl << "Failing students:";
    vector<Student> failing_students = find_failing_students(students); //create a vector to hold all the failing students
    if (failing_students.size() == 0) { //if the size of this vector is 0 then we have no failing students
        cout << " None" << endl;
    } else { //otherwise print the students that have failed
        cout << endl;
        print_students(failing_students);
    }
    return 0;
}