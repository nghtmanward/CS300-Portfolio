/*
    Shawn Ward
    CS 300: Analysis and Design
    Project Two - Advising Assistance Program
    Professor Chao Ling
    19 February 2026

    Description:
    This program loads course data from a CSV file into a Binary Search Tree (BST),
    prints all courses in alphanumeric order, and allows the user to look up
    individual courses along with their prerequisite information.
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Course structure to store course information
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Node structure for the BST
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree for storing and retrieving courses
class BinarySearchTree {
private:
    Node* root;

    // Recursive helper for inserting a course
    void addNode(Node* node, const Course& course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        }
    }

    // Recursive in-order traversal
    void inOrder(Node* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.courseNumber << ": " << node->course.title << endl;
        inOrder(node->right);
    }

    // Recursively delete nodes
    void destroyTree(Node* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroyTree(root);
    }

    // Insert a course into the BST
    void Insert(const Course& course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    // Search for a course by course number
    Course* Search(const string& courseNumber) const {
        Node* current = root;
        while (current != nullptr) {
            if (courseNumber == current->course.courseNumber) {
                return &current->course;
            } else if (courseNumber < current->course.courseNumber) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }

    // Print all courses in sorted order
    void PrintAllCourses() const {
        if (root == nullptr) {
            cout << "No courses loaded." << endl;
            return;
        }
        inOrder(root);
    }
};

// Load courses from a CSV file and insert them into the BST
void loadCourses(const string& filename, BinarySearchTree& bst) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        Course course;

        // Course number
        getline(ss, token, ',');
        if (token.empty()) continue;
        course.courseNumber = token;

        // Course title
        getline(ss, token, ',');
        if (token.empty()) continue;
        course.title = token;

        // Prerequisites
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                course.prerequisites.push_back(token);
            }
        }

        bst.Insert(course);
    }

    file.close();
    cout << "Courses loaded successfully from '" << filename << "'." << endl;
}

// Print a single course and its prerequisite titles
void printCourse(const Course* course, const BinarySearchTree& bst) {
    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->courseNumber << ": " << course->title << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            string prereqNum = course->prerequisites[i];
            Course* prereqCourse = bst.Search(prereqNum);

            if (prereqCourse != nullptr) {
                cout << prereqCourse->courseNumber << " (" << prereqCourse->title << ")";
            } else {
                cout << prereqNum << " (not found)";
            }

            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// Display the menu
void printMenu() {
    cout << "\nMenu:" << endl;
    cout << "  1. Load Data Structure" << endl;
    cout << "  2. Print Course List" << endl;
    cout << "  3. Print Course" << endl;
    cout << "  9. Exit" << endl;
    cout << "Enter choice: ";
}

int main() {
    BinarySearchTree bst;
    bool dataLoaded = false;
    int choice = 0;

    while (choice != 9) {
        printMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            string filename;
            cout << "Enter the course data file name: ";
            getline(cin, filename);

            loadCourses(filename, bst);
            dataLoaded = true;
        }
        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Please load the data structure first (option 1)." << endl;
            } else {
                cout << "\nHere is a sample schedule:\n" << endl;
                bst.PrintAllCourses();
            }
        }
        else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Please load the data structure first (option 1)." << endl;
            } else {
                string courseNumber;
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);

                cout << endl;
                Course* course = bst.Search(courseNumber);
                printCourse(course, bst);
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }
        else {
            cout << "Invalid option. Please select 1, 2, 3, or 9." << endl;
        }
    }

    return 0;
}