#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// creating a course struct to hold course objects including their attributes
struct Course {
    string courseNum;
    string courseName;
    vector<string> prereqs;
    Course(){}
    Course(string num, string name, vector<string> preq) {
        courseNum = num;
        courseName = name;
        prereqs = preq;
    }
};

// creating node object to hold courses in the BST
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

// creating binary search tree class to order and store course/node objects
class BinarySearchTree{
    
private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    // adding a private helper funtion to recursively delete nodes in the deconstructor method
    void deconstructTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNum);
};

// creating default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// creating helper function for destructor
void BinarySearchTree::deconstructTree(Node* node) {
    if (node != nullptr) {
        deconstructTree(node->left);
        deconstructTree(node->right);
        delete node;
    }
}

// creating destructor
BinarySearchTree::~BinarySearchTree() {
    deconstructTree(root);
}

// creating public InOrder method to print out courses in order
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// creating insert method to take course object and insert into proper bst location
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

// creating a search method to find and return course object from bst
Course BinarySearchTree::Search(string courseNum) {
    Node* currNode = root;
    while (currNode != nullptr) {
        if (courseNum == currNode->course.courseNum) {
            return currNode->course;
        }
        else if (courseNum < currNode->course.courseNum) {
            currNode = currNode->left;
        }
        else {
            currNode = currNode->right;
        }
    }
    Course course;
    return course;
}

// creating addNode method to place course object into proper location in bst using recursive calls
void BinarySearchTree::addNode(Node* node, Course course) {
    if (node->course.courseNum > course.courseNum) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

// creating private inOrder method to handle traversing bst recursively
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNum << ", "
            << node->course.courseName << '\n'
            << "Prerequisites: ";
        if (!node->course.prereqs.empty()) {
            int vectorSize = node->course.prereqs.size();
                for (int i = 0; i < vectorSize; ++i) {
                    cout << node->course.prereqs[i] << " ";
                }
            cout << endl;
        }
        inOrder(node->right);
    }
}

// creating load courses function to handle parsing file and creating and storing course objects
void loadCourses(string fileName, BinarySearchTree& bst) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: file could not be opened" << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        
        stringstream ss(line);
        string courseNum;
        string courseName;
        string prereq;
        vector<string> prereqs;

        getline(ss, courseNum, ',');
        getline(ss, courseName, ',');
        while (getline(ss, prereq, ',')) {
            prereqs.push_back(prereq);
        }
        Course course(courseNum, courseName, prereqs);
        bst.Insert(course);
    }
    cout << "Courses have been loaded" << endl;
}

// function to display course information neatly
void DisplayCourse(Course course) {
    cout << course.courseNum << ", " << course.courseName << '\n'
        << "Prerequisites: ";
    if (!course.prereqs.empty()) {
        int vectorSize = course.prereqs.size();
        for (int i = 0; i < vectorSize; ++i) {
            cout << course.prereqs[i] << " ";
        }
        cout << endl;
    }
    return;
}

int main()
{
    BinarySearchTree bst;
    Course course;
    string fileName;
    int choice = 0;

    while (true) {
        cout << "Welcome to the course planner." << '\n'
            << "Please type in the CSV file name." << endl;
        getline(cin >> ws, fileName);

        if (cin.fail()) {
            cout << "Invalid input" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        else {
            cout << "You entered " << fileName << endl;
            break;
        }
    }

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input." << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }

        if (choice != 1 && choice != 2 && choice != 3 && choice != 9) {
            cout << choice << " is an invalid option." << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }

        switch (choice) {

        case 1:
            loadCourses(fileName, bst);
            break;

        case 2:
            cout << "Here are all courses in order." << endl;
            bst.InOrder();
            break;

        case 3:
            cout << "Please enter the course number you want to search for." << endl;
            string courseNum;
            cin >> courseNum;

            course = bst.Search(courseNum);

            if (!course.courseNum.empty()) {
                DisplayCourse(course);
            }
            else {
                cout << courseNum << " not found." << endl;
            }

            break;
        }
    }

    cout << "Thank you for using the course planner." << endl;

    return 0;
}


