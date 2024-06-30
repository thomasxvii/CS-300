//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Michael Thomas
// Version     : 1.0
// Description : 7-2 Project Two ABCU
//============================================================================

#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>
#include <Windows.h>

using namespace std;

const int GLOBAL_SLEEP_TIME = 10000; //Default time it takes for system to sleep

struct Course { //Defines course instances 
	string courseId;
	string courseName;
	vector<string> preList;
};

class BinarySearchTree {

private:
	//Node structure to hold courses
	struct Node {
		Course course;
		Node* right;
		Node* left;

		//Default constructor
		Node() {
			left = nullptr;
			right = nullptr;
		}

		//Course initialization
		Node(Course aCourse) {
			course = aCourse;
			left = nullptr;
			right = nullptr;
		}
	};

	Node* root;
	void inOrder(Node* node);
	int size = 0;

public:
	BinarySearchTree();
	void InOrder();
	void Insert(Course aCourse);
	void Remove(string courseId);
	Course Search(string courseId);
	int Size();
};

//Default Constructor
BinarySearchTree::BinarySearchTree() {
	this->root = nullptr;
}

//Tree traversal in numerical order
void BinarySearchTree::InOrder() {
	inOrder(root);
}

//Course insertion
void BinarySearchTree::Insert(Course aCourse) {

	Node* currNode = root;

	if (root == NULL) {
		root = new Node(aCourse);
	}
	else {
		while (currNode != NULL) {
			if (aCourse.courseId < currNode->course.courseId) {
				if (currNode->left == nullptr) {
					currNode->left = new Node(aCourse);
					currNode = NULL;
				}
				else {
					currNode = currNode->left;
				}
			}
			else {
				if (currNode->right == nullptr) {
					currNode->right = new Node(aCourse);
					currNode = NULL;
				}
				else {
					currNode = currNode->right;
				}
			}
		}
	}
	size++;
}

//Course Removal
void BinarySearchTree::Remove(string courseId) {
	Node* par = NULL;
	Node* curr = root;

	while (curr != NULL) {
		if (curr->course.courseId == courseId) {
			if (curr->left == NULL && curr->right == NULL) {
				if (par == NULL) {
					root = nullptr;
				}
				else if (par->left == curr) {
					par->left = NULL;
				}
				else {
					par->right = NULL;
				}
			}
			else if (curr->right == NULL) {
				if (par == NULL) {
					root = curr->left;
				}
				else if (par->left == curr) {
					par->left = curr->left;
				}
				else {
					par->right = curr->left;
				}
			}
			else if (curr->left == NULL) {
				if (par == NULL) {
					root = curr->right;
				}
				else if (par->left == curr) {
					par->left = curr->right;
				}
				else {
					par->right = curr->right;
				}
			}
			else {
				Node* suc = curr->right;

				while (suc->left != NULL) {
					suc = suc->left;
				}

				//Creating data node for the successor
				Node succData = Node(suc->course);
				Remove(suc->course.courseId);
				curr->course = succData.course;
			}
			return;
		}
		else if (curr->course.courseId < courseId) {
			par = curr;
			curr = curr->right;
		}
		else {
			par = curr;
			curr = curr->left;
		}
	}
	cout << "\nValue not found" << endl;
	return;
}

//Course Search
Course BinarySearchTree::Search(string courseId) {
	Course aCourse;
	Node* currNode = root;

	while (currNode != NULL) {
		if (currNode->course.courseId == courseId) {
			return currNode->course;
		}
		else if (courseId < currNode->course.courseId) {
			currNode = currNode->left;
		}
		else {
			currNode = currNode->right;
		}
	}

	//Not found, will display "value not found"
	return aCourse;
}

//Course Ordered List
void BinarySearchTree::inOrder(Node* node) {
	if (node == NULL) {
		return;
	}
	inOrder(node->left);

	//Display the node
	cout << node->course.courseId << ", " << node->course.courseName << endl;

	inOrder(node->right);
}

//Size of BinarySearchTree
int BinarySearchTree::Size() {
	return size;
}

//Delimiter for file stream
vector<string> Split(string lineFeed) {
	char delim = ',';

	lineFeed += delim; //Delimiter will be included at the end, ensures final word is read 
	vector<string> lineTokens;
	string temp = "";
	for (int i = 0; i < lineFeed.length(); i++) {
		if (lineFeed[i] == delim) {
			lineTokens.push_back(temp); //Stores words in file as token vectors
			temp = "";
			i++;
		}
		temp += lineFeed[i];
	}
	return lineTokens;
}

//Function for loading courses
void loadCourses(string csvPath, BinarySearchTree* courseList) {

	//Data structure creation and addition to the course collection
	ifstream inFS; //In stream to read input file
	string line; //Feed for each line
	vector<string> stringTokens;

	inFS.open(csvPath); //Opens the file to read

	if (!inFS.is_open()) { //Handles errors in file opening
		cout << "Could not open files. Please check inputs." << endl;
		return;
	}

	while (!inFS.eof()) {
		Course aCourse; //Create a new structure for each line in file

		getline(inFS, line);
		stringTokens = Split(line); //Splits each line into tokens using the comma delimiter

		if (stringTokens.size() < 2) { //Skips lines that are misformatted 
			cout << "\nError. Skipping line." << endl;

		}
		else {
			aCourse.courseId = stringTokens.at(0);
			aCourse.courseName = stringTokens.at(1);

			for (unsigned int i = 2; i < stringTokens.size(); i++) {
				aCourse.preList.push_back(stringTokens.at(i));
			}

			//Push this course to end of list
			courseList->Insert(aCourse);
		}
	}
	inFS.close(); //Closes the file
}

//Course Display
void displayCourse(Course aCourse) {
	cout << aCourse.courseId << ", " << aCourse.courseName << endl;
	cout << "Prerequisites: ";

	if (aCourse.preList.empty()) { //Empty lists represent no course prerequisites
		cout << "none" << endl;
	}
	else {
		for (unsigned int i = 0; i < aCourse.preList.size(); i++) {
			cout << aCourse.preList.at(i);

			if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1) {
				cout << ", "; //Includes a comma for multiple elements
			}
		}
	}
	cout << endl;
	cout << "Loading . . ." << endl;
}

//Uppercase conversion
void convertCase(string& toConvert) {
	for (unsigned int i = 0; i < toConvert.length(); i++) {
		if (isalpha(toConvert[i])) {
			toConvert[i] = toupper(toConvert[i]);
		}
	}
}

//The one and only main function
int main(int argc, char* argv[]) {

	cout << "Welcome to the course planner." << endl;

	//Processes command line arguments
	string csvPath, aCourseKey;

	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		aCourseKey = argv[2];
		break;
	default:
		csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
	}

	//Table definition that holds all the courses
	BinarySearchTree* courseList = new BinarySearchTree();

	Course course;
	bool goodInput;
	int choice = 0;

	while (choice != 9) {
		cout << "\nMenu:" << endl;
		cout << " 1. Load Courses" << endl;
		cout << " 2. Display All Courses" << endl;
		cout << " 3. Find Course" << endl;
		cout << " 4. Remove Course" << endl;
		cout << " 9. Exit\n" << endl;
		cout << "What would you like to do? ";

		aCourseKey = ""; //Clears the string
		string anyKey = " "; //Clears the string
		choice = 0; //Clears the choice

		try {
			cin >> choice;

			if ((choice > 0 && choice < 5) || (choice == 9)) { //Limits user input to appropriate values
				goodInput = true;
			}
			else { //Throws errors for catch
				goodInput = false;
				throw 1;
			}

			switch (choice) {

			case 1:

				//Calls the method to load courses from file
				loadCourses(csvPath, courseList);
				cout << courseList->Size() << " courses read" << endl;
				cout << "Loading, preparing menu. . ." << endl;
				Sleep(GLOBAL_SLEEP_TIME);
				break;

			case 2:

				cout << "Here is a sample schedule:" << endl << "\n";
				courseList->InOrder();
				cout << "\nEnter \'y\' to continue..." << endl;
				cin >> anyKey;
				break;

			case 3:

				cout << "\nWhat course do you want to know about? ";
				cin >> aCourseKey;
				convertCase(aCourseKey); //Converts user input to proper format
				course = courseList->Search(aCourseKey);

				if (!course.courseId.empty()) {
					displayCourse(course);
				}
				else {
					cout << "\nCourse ID " << aCourseKey << " not found." << endl;
				}

				Sleep(GLOBAL_SLEEP_TIME);
				break;

			case 4:

				cout << "\nWhat course do you want to delete? ";
				cin >> aCourseKey;
				cout << "Loading, please be patient . . ." << endl;
				convertCase(aCourseKey); //Converts user input to proper format
				courseList->Remove(aCourseKey);
				Sleep(GLOBAL_SLEEP_TIME);
				break;

			case 9:

				exit;
				break;

			default:

				throw 2;
			}
		}
		catch (int err) {
			std::cout << choice << " is not a valid option." << endl;
			cout << "Reloading menu for new user input . . ." << endl;
			Sleep(GLOBAL_SLEEP_TIME);
		}
		//Clears cin operator in case of extra user input
		cin.clear();
		cin.ignore();

		
	}

	cout << "Thank you for using the course planner!" << endl;

	Sleep(GLOBAL_SLEEP_TIME);

	return 0;
}