/*******************************************************************************
 * Name        : shortestpaths.cpp
 * Author      : Aryaman Srivastava
 * Version     : 1.0
 * Date        : 12/2/23
 * Description : Implementation of Floyd's algorithm
 * Pledge      : I pledge my honor that I have abided by the Stevens Honors System
 ******************************************************************************/
 #include <climits>
 #include <iostream>
 #include <fstream>
 #include <vector>
 #include <iomanip>
 #include <sstream>
 #include <cmath>
 #include <algorithm>
 using namespace std;

const long INF = pow(2,32);

/*
Returns the number of digits in the given number
*/
long len(long num) {
    if (num == 0) {
        return 1;
    } else {
        int digits = 0;
        while (num > 0) {
            digits++;
            num /= 10;
        }
        return digits;
    }
}

/*
Generates the matrix of intermediate vertices for the shortest path
*/
void generateIntermediateMatrix(long** intermediate, long** prevMatrix, long** newMatrix, int vertex, const int numVertices = 1) {
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (prevMatrix[i][j] != newMatrix[i][j]) {
                intermediate[i][j] = vertex;
                prevMatrix[i][j] = newMatrix[i][j];
            }
        }
    }
}

/*
Returns a string representing the shortest path from given vertex to the desired vertex
*/
string generatePath(long** matrix, int src, int dst, vector<string> vertices, bool alreadySeen) {
    if (matrix[src][dst] == INF && !alreadySeen) {
        string ans;
        if (src == dst) {
            ans += vertices[src];
        } else {
            ans += vertices[src];
            ans += " -> ";
            ans += vertices[dst];
        }
        return ans;
    } else if (matrix[src][dst] == INF) {
        return vertices[dst];
    } else {
        if (!alreadySeen) {
            string ans;
            ans += generatePath(matrix, src, matrix[src][dst], vertices, false);
            ans += " -> ";
            ans += generatePath(matrix, matrix[src][dst], dst, vertices, true);
            return ans;
        } else {
            string ans;
            ans += generatePath(matrix, src, matrix[src][dst], vertices, true);
            ans += " -> ";
            ans += generatePath(matrix, matrix[src][dst], dst, vertices, true);
            return ans;
        }
    }
}

/*
Floyd's Algorithm as seen in Levitin Textbook, which also generates the matrix of intermediate vertices at each iteration
*/
void floyd(long** matrix, long** intermediate, long** prevMatrix, const int num_vertices = 1) {
    for (int k = 0; k < num_vertices; k++) {
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
                generateIntermediateMatrix(intermediate, prevMatrix, matrix, k, num_vertices);
            }
        }
    }
}

/**
* Displays the matrix on the screen formatted as a table. Taken from assignment specification. 
*/
void display_table(long** const matrix, const string &label, const bool use_letters = false, const int num_vertices = 1) {
    cout << label << endl;
    long max_val = 0;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            long cell = matrix[i][j];
            if (cell < INF && cell > max_val) {
                max_val = matrix[i][j];
            }
        }
    }
    int max_cell_width = use_letters ? len(max_val) :
        len(max(static_cast<long>(num_vertices), max_val));
    cout << ' ';
    for (int j = 0; j < num_vertices; j++) {
        cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
    }
    cout << endl;
    for (int i = 0; i < num_vertices; i++) {
        cout << static_cast<char>(i + 'A');
        for (int j = 0; j < num_vertices; j++) {
            cout << " " << setw(max_cell_width);
            if (matrix[i][j] == INF) {
                cout << "-";
            } else if (use_letters) {
                cout << static_cast<char>(matrix[i][j] + 'A');
            } else {
                cout << matrix[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

/*
Deletes all the subarrays of the given 2D array, then delete the actual array
*/
void deleteArray(long** matrix, int length) {
 for (int i = 0; i < length; i++) {
        delete [] matrix[i];
    }
    delete [] matrix;
}


int main(int argc, const char *argv[]) {
    if (argc != 2) { //check file is given 
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    ifstream input_file(argv[1]);
    if (!input_file) { //check if file is in directory
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }
    input_file.exceptions(ifstream::badbit);
    string line;
    int numVertices;
    long** distanceMatrix;
    vector<string> allPossibleVertices = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S","T","U","V","W","X","Y","Z"};
    vector<string> vertices;
    try {
        int lineNumber = 1;
        while (getline(input_file, line)) {
            if (lineNumber == 1) {
                //check if number of vertices is a valid integer between 1 and 26
                for (size_t i = 0; i < line.size(); i++) {
                    if (!isdigit(line[i])) { 
                        cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
                        return 1;
                    }
                } 
                if (stoi(line) < 1 || stoi(line) > 26) {
                    cerr << "Error: Invalid number of vertices '" << stoi(line) << "' on line 1." << endl;
                    return 1;
                } else {
                    //if it is, initialize the vector containing the valid vertices and the distance matrix
                    numVertices = stoi(line);
                    for (int i = 0; i < numVertices; i++) {
                        vertices.push_back(allPossibleVertices[i]);
                    }
                    distanceMatrix = new long*[numVertices];
                    for (int i = 0; i < numVertices; i++) {
                        distanceMatrix[i] = new long[numVertices];
                        for (int j = 0; j < numVertices; j++) {
                            if (i == j) {
                                distanceMatrix[i][j] = 0; 
                            } else {
                                distanceMatrix[i][j] = INF;
                            }
                        }
                    }
                    lineNumber++;
                }
            } else {
                int length = 0;
                stringstream ss(line);
                string word;
                string vertex1;
                string vertex2;
                int edgeWeight;
                while (ss >> word) { //read each word in each line and check if it is valid
                    if (length == 2) {
                        //check if the edge weight is a valid positive integer
                        for (size_t i = 0; i < word.size(); i++) {
                            if (!isdigit(word[i])) {
                                cerr << "Error: Invalid edge weight '" << word << "' on line " << lineNumber << "." << endl;
                                deleteArray(distanceMatrix, numVertices);
                                return 1;
                            }
                        } 
                        if (stoi(word) <= 0) {
                            cerr << "Error: Invalid edge weight '" << word << "' on line " << lineNumber << "." << endl;
                            deleteArray(distanceMatrix, numVertices);
                            return 1;
                        } else {
                            //if it is, set our edgeweight and break out of the loop
                            edgeWeight = stoi(word);
                            length++;
                            break;
                        }
                    } else if (find(vertices.begin(), vertices.end(), word) == vertices.end()) {
                        //check if the two vertices are valid
                        if (length == 1) {
                            cerr << "Error: Ending vertex '" << word << "' on line " << lineNumber << " is not among valid values " << vertices[0] << "-" << vertices[vertices.size() - 1] << "." << endl;
                            deleteArray(distanceMatrix, numVertices);
                            return 1;
                        } else {
                            cerr << "Error: Starting vertex '" << word << "' on line " << lineNumber << " is not among valid values " << vertices[0] << "-" << vertices[vertices.size() - 1] << "." << endl;
                            deleteArray(distanceMatrix, numVertices);
                            return 1;
                        }
                    } else {
                        //if they are, set them accordingly
                        if (length == 0) {
                            vertex1 = word;
                        } else if (length == 1) {
                            vertex2 = word;
                        }
                    } 
                    length++;
                }
                if (length != 3) {
                    //check that each line is valid: 2 vertices and an edgeweight
                    cerr << "Error: Invalid edge data '" << line << "' on line " << lineNumber << "." << endl;
                    deleteArray(distanceMatrix, numVertices);
                    return 1;
                } else {
                    //if it is, set the corresponding matrix element to the correct edgeweight in our distance matrix
                    auto it = find(vertices.begin(), vertices.end(), vertex1);
                    auto it2 = find(vertices.begin(), vertices.end(), vertex2);
                    int index1 = it - vertices.begin();
                    int index2 = it2 - vertices.begin();
                    distanceMatrix[index1][index2] = edgeWeight;
                }
                lineNumber++;
            }
        }
    } catch(const ifstream::failure &f) {
        cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
        return 1;
    }
    //initialize a temporary matrix to track the previous matrix at each step of the algorithm and a matrix to keep track of the intermediates
    long** tempMatrix = new long*[numVertices];
    long** intermediateMatrix = new long*[numVertices];
    for (int i = 0; i < numVertices; i++) {
        intermediateMatrix[i] = new long[numVertices];
        tempMatrix[i] = new long[numVertices];
        for (int j = 0; j < numVertices; j++) {
            intermediateMatrix[i][j] = INF;
            tempMatrix[i][j] = distanceMatrix[i][j];
        }
    }
    //display the matrices
    display_table(distanceMatrix, "Distance matrix:",false, numVertices);
    floyd(distanceMatrix, intermediateMatrix, tempMatrix, numVertices);
    display_table(distanceMatrix, "Path lengths:", false, numVertices);
    display_table(intermediateMatrix, "Intermediate vertices:", true, numVertices);
    //display all the paths
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            cout << vertices[i] << " -> " << vertices[j] << ", distance: ";
            if (distanceMatrix[i][j] == INF) {
                cout << "infinity, path: none" << endl;
            } else {
                cout << distanceMatrix[i][j] << ", path: " << generatePath(intermediateMatrix, i, j, vertices, false) << endl;
            }
        }
    }
    //delete the arrays
    deleteArray(distanceMatrix, numVertices);
    deleteArray(tempMatrix, numVertices);
    deleteArray(intermediateMatrix, numVertices);
    return 0;
 }
