/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Aryaman Srivastava
 * Date        : 10/14/23
 * Description : Solves the water jug puzzle for given inputs
 * Pledge      : I pledge my honor that I have abided by the Stevens Honors System
 ******************************************************************************/
 #include <iostream>
 #include <sstream>
 #include <iomanip>
 #include <ostream>
 #include <queue>
 #include <stack>
 #include <string>
 using namespace std;
struct State { 
    int a, b, c;
    string directions;
    State *parent;
    
    State(int _a, int _b, int _c, string _directions, State* _parent) : //constructor for a State, which initializes all the member variables with given arguments
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{_parent} { }
    
    State(int _a, int _b, int _c, string _directions) : //constructor for a State, which initializes all the member variables with given arguments, and initializes parent to a nullptr
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }
    // String representation of state in tuple form.
    string to_string() { 
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};
int checkArguments(int argc, char * const argv[], int& capA, int& capB, int& capC, int& goalA, int& goalB, int& goalC) {
    if (argc != 7) {
        cerr << "Usage: " << argv[0] << " <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl; //check if we have the correct number of arguments, if not throw an error
        return -1;
    }
    istringstream iss;
    string arr [6] = {"jug A", "jug B", "jug C","jug A", "jug B", "jug C"}; 
    for (int i = 1; i < argc; i++) { //Loop through all the arguments, and check that each one is valid(they are all an integer >= 0) and for the argument for capC, that it is not zero
        iss.str(argv[i]);
        int tester;
        if (!(iss >> tester) || (tester < 0)) {
            if (i < 4) { //output is different for capcities and goals, so check before throwing an error
                cerr << "Error: Invalid capacity '" << argv[i] << "' for " << arr[i - 1] << "." << endl;
                return -1;
            } else {
                cerr << "Error: Invalid goal '" << argv[i] << "' for " << arr[i - 1] << "." << endl;
                return -1;
            }
        } else if (tester == 0 && i == 3) { //special case for if the argument for capC is 0
            cerr << "Error: Invalid capacity '" << argv[i] << "' for " << arr[i-1] << "." << endl;
            return -1;
        }
        iss.clear();
    }
    //put all the arguments into their respective variables
    iss.str(argv[1]);
    iss >> capA;
    iss.clear();
    iss.str(argv[2]);
    iss >> capB;
    iss.clear();
    iss.str(argv[3]);
    iss >> capC;
    iss.clear();
    iss.str(argv[4]);
    iss >> goalA;
    iss.clear();
    iss.str(argv[5]);
    iss >> goalB;
    iss.clear();
    iss.str(argv[6]);
    iss >> goalC;
    iss.clear();
    if (goalA > capA) { //check that goalA <= capA
        cerr << "Error: Goal cannot exceed capacity of jug A." << endl;
        return -1;
    } else if (goalB > capB) { //check that goalB <= capB
        cerr << "Error: Goal cannot exceed capacity of jug B." << endl; 
        return -1;
    } else if (goalC > capC) { //check that goalC <= capC
        cerr << "Error: Goal cannot exceed capacity of jug C." << endl;
        return -1;
    }
    if (goalA + goalB + goalC != capC) { //check that total gallons in goal state is equal to capcity of C
        cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
        return -1;
    }
    return 0;
}
void waterjug(int capA, int capB, int capC, int goalA, int goalB, int goalC) {
    queue<State*> states; //initialize a queue for the BFS algorithm
    bool **arr = new bool*[capA + 1];
    for (int i = 0; i < capA + 1; i++) { //initialize a 2D array of booleans for keeping track of which states we've visited
        arr[i] = new bool[capB + 1];
        fill(arr[i], arr[i] + capB + 1, false);
    }
    State* goal = nullptr; //initialize a state pointer to store the goal state once we reach it.
    State* temp = new State(0,0,capC, "Initial state."); //the initial state
    vector<State*> everyState; //initialize a vector of states that we will use to store all the states so that we can delete everything later on
    states.push(temp); //push the initial state to the queue and vector 
    everyState.push_back(temp);

    while (!states.empty()) {
       State* current = states.front(); //store the front of the queue into a pointer variable
       states.pop();
       int a = current->a;
       int b = current->b;
       int c = current->c;
       int x = 0; //variable to store how many gallons we pour
        if (a == goalA && b == goalB && c == goalC) { //check that we are not already at the goal state
            goal = current;
            break;
        }
        if (!arr[a][b]) {   //check that we haven't already visited the current state
            arr[a][b] = true; //mark the state as being visited
            //perform the 6 operations for the BFS algorithm for waterjug in order from the current state, storing new states into the queue and vector
            if (a != capA && c != 0) {
                if (a + c <= capA) {
                    x = c;
                    a += c;
                    c = 0;
                } else {
                    c = (c + a) - capA;
                    x = capA - a;
                    a = capA;
                }
                if (x > 1) {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallons from C to A.", current);
                    states.push(t);
                    everyState.push_back(t);
                } else {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallon from C to A.", current);
                    states.push(t);
                    everyState.push_back(t);
                }
                a = current->a;
                b = current->b;
                c = current->c;
            }
            if (a != capA && b != 0) {
                if (a + b <= capA) {
                    x = b;
                    a += b;
                    b = 0;
                } else {
                    b = (b + a) - capA;
                    x = capA - a;
                    a = capA;
                }
                if (x > 1) {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallons from B to A.", current);
                    states.push(t);
                    everyState.push_back(t);
                } else {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallon from B to A.", current);
                    states.push(t);
                    everyState.push_back(t);
                }
                a = current->a;
                b = current->b;
                c = current->c;
            }
            if (b != capB && c != 0) {
                if (b + c <= capB) {
                    x = c;
                    b += c;
                    c = 0;
                } else {
                    c = (b + c) - capB;
                    x = capB - b;
                    b = capB;
                }
                if (x > 1) {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallons from C to B.", current);
                    states.push(t);
                    everyState.push_back(t);
                } else {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallon from C to B.", current);
                    states.push(t);
                    everyState.push_back(t);
                }
                a = current->a;
                b = current->b;
                c = current->c;
            } 
            if (b != capB && a != 0) {
                if (b + a <= capB) {
                    x = a;
                    b += a;
                    a = 0;
                } else {
                    a = (a + b) - capB;
                    x = capB - b;
                    b = capB;
                }
                if (x > 1) {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallons from A to B.", current);
                    states.push(t);
                    everyState.push_back(t);
                } else {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallon from A to B.", current);
                    states.push(t);
                    everyState.push_back(t);
                }
                a = current->a;
                b = current->b;
                c = current->c;
            }
            if (c != capC && b != 0) {
                 if (b + c <= capC) {
                    x = b;
                    c += b;
                    b = 0;
                } else {
                    b = (b + c) - capC;
                    x = capC - c;
                    c = capC;
                }
                if (x > 1) {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallons from B to C.", current);
                    states.push(t);
                    everyState.push_back(t);
                } else {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallon from B to C.", current);
                    states.push(t);
                    everyState.push_back(t);
                    
                }
                a = current->a;
                b = current->b;
                c = current->c;
            }
            if (c != capC && a != 0) {
                if (a + c <= capC) {
                    x = a;
                    c += a;
                    a = 0;
                } else {
                    a = (a + c) - capC;
                    x = capC - c;
                    c = capC;
                }
                if (x > 1) {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallons from A to C.", current);
                    states.push(t);
                    everyState.push_back(t);
                } else {
                    State* t = new State(a, b, c, "Pour " + to_string(x) + " gallon from A to C.", current);
                    states.push(t);
                    everyState.push_back(t);
                }
                a = current->a;
                b = current->b;
                c = current->c;
            }
        } 
    }
    if (goal == nullptr) { //check that the goal is not null, if it is that means we went through BFS and couldn't arrive at the goal state, so there is no solution
            cout << "No solution." << endl;
        } else {
            stack<State*> statesStack;
            while (goal != nullptr) { //push all the states that lead to the goal state into a stack
                statesStack.push(goal);
                goal = goal->parent;
            } 
            while (!statesStack.empty()) {
                cout << statesStack.top()->directions << " " << statesStack.top()->to_string() << endl; //print all the states, starting with the initial state
                statesStack.pop();
            }
        }
    for (int i = 0; i < capA + 1; i++) { //delete every array element(which are also arrays) in the 2D array
        delete [] arr[i];
    } 
    for (State* state: everyState) { //delete every state contained in the vector(every state we visited in the BFS algorithm)
        delete state;
    }
    delete [] arr; //delete the array
    return;
}   

int main(int argc, char * const argv[]) {
    //initialize variables to hold the arguments
    int capA;
    int capB;
    int capC;
    int goalA;
    int goalB;
    int goalC;
    if (checkArguments(argc, argv, capA, capB, capC, goalA, goalB, goalC) == -1) { //check that the arguments are valid
        return -1;
    } else {
        waterjug(capA, capB, capC, goalA, goalB, goalC); //run the algorithm
    }
    return 0;
 }