#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include "CSVParser.h"

using namespace std;


// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold course information
struct Course {
	string courseNumber; // course number
	string courseTitle;
	string prereq;

};

struct Node {
	Course course;
	Node* left = nullptr;
	Node* right = nullptr;


};
class BinarySearchTree {

private:
	Node* root;
	void inOrder(Node* node);
	void addNode(Node* node, Course course);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	Course Search(string courseNumber);
	void Insert(Course course);
};

BinarySearchTree::BinarySearchTree() {
	/*Initialize the variables by setting the root equal to a nullptr
	*/
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	/*Recursively delete the tree. First we delete the left side of the tree
	* and then we move to delete the right side of the tree. We delete the root and then sset the root
	* to a nullptr so that the tree can be used again. We also want to make sure that the tree
	* is not already empty by putting in the tree check at the beginning.
	*/
	if (root == NULL) {
		cout << "Empty Tree";
		return;
	}
	else {
		delete(root->left);
		delete(root->right);

		delete(root);
	}
	root = nullptr;
}

void BinarySearchTree::InOrder() {
	//Call inorder to root to traverse the tree in order
	if (root == NULL) {
		cout << "No root established" << endl;
	}
	else {
		inOrder(root);
	}
}

Course BinarySearchTree::Search( string courseNumber) {
	// The current node is equal to the root
	Node* curr = root;
	Course course;
	/* While the current node is not null, if the courseNumber is the current course 
	* return the current course. If the current coursenumber is greater then the course we are searching for,
	* traverse down the left side of the tree. If the current node is less than the course we are
	* searching for, traverse down the right. Continue this until the match is found or the bottom is
	* reached (nullptr)
	*/
	

	while (curr != nullptr) {
		if (curr->course.courseNumber == courseNumber) {
			return curr->course;
		}
		if (courseNumber < curr->course.courseNumber) {
			curr = curr->left;
		}
		else if (courseNumber> curr->course.courseNumber) {
			curr = curr->right;
		}
	}
	return course;
}
void BinarySearchTree::inOrder(Node* node) {
	/* Traverse in order the tree. First put the tree in order starting from the root
	* going down the left side. Then return to thee root and print out the data. Finally,
	* now that we are back at the root, we traverse down the right side.
	*/
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseNumber << " : " << node->course.courseTitle << " | " << node->course.prereq;
		inOrder(node->right);
	}
}
void BinarySearchTree::Insert(Course course) {
	//if the tree is empty and the root is equal to a null pointer
	if (root == nullptr) {
		//The new inserted node is now the root
		root = new Node;
		root->course = course;
		//The left and right side of the new bid are then set to nullptr as there is no leaf node yet
		root->left = nullptr;
		root->right = nullptr;
	}
	//otherwise, add a node root and course to the tree
	else {
		addNode(root, course);
	}
}
void BinarySearchTree::addNode(Node* node, Course course) {
	/* If the node is larger than the course, we add the node to the left of the tree. If the left
	* node is a nullptr, the node becomes the left node. Otherwise, recurse down the left side.
	* Else, if the right node is a nullptr, the new node becomes the right node. Otherwise, recurse down
	* the right side to add the node.
	*/
	if (node->course.courseNumber.compare(course.courseNumber) > 0) {
		if (node->left == nullptr) {
			node->left = new Node;
		}
		else {
			addNode(node->left, course);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node;
		}
		else {
			addNode(node->right, course);
		}
	}

}


//============================================================================
// METHODS FOR TESTING
//============================================================================

//simple display course method
void displayCourse(Course course) {
	cout << course.courseNumber << ": " << course.courseTitle << " | " << course.prereq << endl;
	return;
}
//open and read csv file and then creating a data structure to hold the courses. 
void loadCourse(string csvPath, BinarySearchTree* bst) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	vector<string> header = file.getHeader();
	for (auto const& c : header) {
		cout << c << " | ";
	}
	cout << "" << endl;

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// data structure to hold the courses
			Course course;
			course.courseNumber = file[i][0];
			course.courseTitle = file[i][8];
			course.prereq = file[i][20];

			cout << "Course: " << course.courseNumber << ", Course Name: " << course.courseTitle << ", Prerequisite : " << course.prereq << endl;
			
			// push this course to the end
			bst->Insert(course);
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}


double strToDouble(string str, char ch) {
	str.erase(remove(str.begin(), str.end(), ch), str.end());
	return atof(str.c_str());
}

int main(int argc, char* argv[]) {
	// process command line arguments
	string csvPath, Key;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		Key = "00000";
		break;
	case 3:
		csvPath = argv[1];
		Key = argv[2];
		break;
	default:
		csvPath = "ABCU_Advising_Program_Input.csv";
		Key = "00000";
	}
	clock_t ticks;

	// Define a binary search tree to hold all courses along with course data structure and coursenum to search later on. 
	BinarySearchTree* bst{};
	Course course;
	string courseNum;

	int userInput = 0;
	while (userInput != 9) {
		cout << "Welcome to the course planner." << endl;
		cout << "1. Load Data Structure." << endl;
		cout << "2. Print Course List." << endl;
		cout << "3. Print Course." << endl;
		cout << "9. Exit" << endl;
		cout << "What would you like to do?";
		cin >> userInput;

		switch (userInput) {

		case 1:
			bst = new BinarySearchTree();

			// Initialize a timer variable before loading courses
			ticks = clock();

			// Complete the method call to load the courses
			loadCourse(csvPath, bst);

			cout << sizeof(bst)<< " courses read" << endl;

			// Calculate elapsed time and display result
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			break;

		case 2:
			cout << "Here is a sample schedule: " << endl;
			bst->InOrder();
			break;



		case 3:
			cout << "What course do you want to know about?" << endl;
			cin >> courseNum;
			bst->Search(courseNum);
			break;

		case 9:
			cout << " You are now exiting the program!" << endl;
			break;

		default:
			cout << userInput << " is not a valid option. " << endl;

		}

	}
	cout << "Goodbye!";
	return 0;
}