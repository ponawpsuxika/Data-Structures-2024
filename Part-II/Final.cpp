#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>


using namespace std;
//HASHING*********************************************************************************
struct info {
    int date;
    string type;
    string place;
    int amount;
};

int stringToIntH(const string& str) {
    int result;
    stringstream ss(str);
    ss >> result;
    return result;
}

// Define the HashTable class
class HashTable {
private:
    int size;
    vector<list<info>> table;

    // Hash function to determine index
    int hash(const string& key) {
        size = key.size();

        int hashValue = 0;
        for (char c : key) {
            hashValue += c;
        }
        return hashValue % size;
    }


public:
    // Constructor
    HashTable(int size) : size(size), table(size) {}

    // Insertion method
    void insert(const info& data) {
        int index = hash(data.place);
        table[index].push_back(data);
    }

    // Search method
    info* searchHASH(string place, int date, string event) {
        int index = hash(place);
        for (auto& element : table[index]) {
            if (element.place == place && element.date == date && element.type == event) {
                return &element;
            }
        }
        return NULL;
    }
    // Deletion method
    void remove(const string& region) {
        int index = hash(region);
        for (auto it = table[index].begin(); it != table[index].end();) {
            if (it->place == region) {
                it = table[index].erase(it);
            }
            else {
                it++;
            }
        }
    }
    info* change_amount(string place, int date, string event) {
        int index = hash(place);
        int num;
        for (auto& element : table[index]) {
            if (element.place == place && element.date == date && element.type == event) {
                cout << "Please enter new number below:\n";
                cin >> num;
                element.amount = num;
                return &element;
            }
        }
        return NULL;
    }
};


void readFiletoHash(HashTable& hashTable, string filename) {
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Failed to open the file\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        string datestr, type, place;
        int amount;
        stringstream ss(line);
        getline(ss, datestr, ',');
        getline(ss, type, ',');
        getline(ss, place, ',');
        ss >> amount;
        int date = stringToIntH(datestr);
        info data = { date , type, place, amount };
        hashTable.insert(data);
    }
    file.close();
}

void searchMod(HashTable& hashTable) {
    string searchplace;
    int searchdate;
    cin.ignore();
    cout << "Enter the place you want to find: ";
    getline(cin, searchplace);

    cout << "Enter the date you want to find: ";
    cin >> searchdate;

    info* result = hashTable.searchHASH(searchplace, searchdate, "Births");

    if (result != NULL) {
        cout << "\n found: " << result->date << ", " << result->type << ", " << result->place << ", " << result->amount << endl;
    }
    else {
        cout << "\nPlace " << searchplace << " and date " << searchdate << " was not found or was all Deaths." << endl;
    }
}

//AVL TREE**********************************************************************************************
struct Node {
    int year;
    string name;
    string region;
    int population;
    Node* left;
    Node* right;
    int height;
};

//used for the readfromfile function
int stringToInt(const string& str) {
    int result;
    stringstream ss(str);
    ss >> result;
    return result;
}

//constuctor
Node* newNode(int year, string name, string region, int population) {
    Node* node = new Node;  // Use new instead of malloc
    node->year = year;
    node->name = name;
    node->region = region;
    node->population = population;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

Node* rightRotate(Node* y) {
    if (y == nullptr) {
        // Handle null pointer case
        return nullptr;
    }

    Node* x = y->left;
    if (x == nullptr) {
        // Handle null pointer case
        return nullptr;
    }

    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    // Update heights
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}


Node* leftRotate(Node* x) {
    if (x == nullptr) {
        // Handle null pointer case
        return nullptr;
    }

    Node* y = x->right;
    if (y == nullptr) {
        // Handle null pointer case
        return nullptr;
    }

    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    // Update heights
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}



int getBalance(Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insertNode(Node* node, int year, string name, string region, int population) {
    if (node == NULL) {
        return newNode(year, name, region, population);
    }
    // Compare region first
    if (region < node->region)
        node->left = insertNode(node->left, year, name, region, population);
    else if (region > node->region)
        node->right = insertNode(node->right, year, name, region, population);
    else { //Region is the same, compare name type
        if (name < node->name)
            node->left = insertNode(node->left, year, name, region, population);
        else if (name > node->name)
            node->right = insertNode(node->right, year, name, region, population);
        else { //Region and name type are the same, compare year
            if (year < node->year)
                node->left = insertNode(node->left, year, name, region, population);
            else
                node->right = insertNode(node->right, year, name, region, population);
        }
    }

    
    node->height = 1 + max(height(node->left), height(node->right));

    
    int balance = getBalance(node);

   
    if (balance > 1 && year < node->left->year)
        return rightRotate(node);

    
    if (balance < -1 && year > node->right->year)
        return leftRotate(node);

    
    if (balance > 1 && year > node->left->year) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    
    if (balance < -1 && year < node->right->year) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}
Node* insertNodeButBirths(Node* node, int year, string name, string region, int population) {
    // If the node is a "Death" node, ignore it
    if (name == "Deaths") {
        return node;
    }

    if (node == NULL) {
        return newNode(year, name, region, population);
    }

    // Compare populations
    if (population < node->population) {
        node->left = insertNodeButBirths(node->left, year, name, region, population);
    }
    else if (population > node->population) {
        node->right = insertNodeButBirths(node->right, year, name, region, population);
    }
    else { // If populations are the same
        // Compare regions
        if (region < node->region) {
            node->left = insertNodeButBirths(node->left, year, name, region, population);
        }
        else if (region > node->region) {
            node->right = insertNodeButBirths(node->right, year, name, region, population);
        }
    }

    // Update height of the current node
    node->height = 1 + max(height(node->left), height(node->right));

    // Get the balance factor of this node
    int balance = getBalance(node);

    // Perform rotations if necessary
    if (balance > 1 && population < node->left->population)
        return rightRotate(node);

    if (balance < -1 && population > node->right->population)
        return leftRotate(node);

    if (balance > 1 && population > node->left->population) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && population < node->right->population) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // If no rotations needed, return the unchanged node
    return node;
}





void inOrder(Node* root, string& currentRegion) {
    if (root != NULL) {
        inOrder(root->left, currentRegion);
        if (root->region != currentRegion) {
            currentRegion = root->region;
            cout << "REGION : " << currentRegion << endl;
        }
        cout << root->year << ", " << root->name << ", " << root->region << ", " << root->population << endl;
        inOrder(root->right, currentRegion);
    }
}

void readFileToAVLTree(Node **root, string filename,int select) {
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Failed to open the file\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        string yearstr, name, region;
        int population;
        stringstream ss(line);
        getline(ss, yearstr, ',');
        getline(ss, name, ',');
        getline(ss, region, ',');
        ss >> population;
        int year = stringToInt(yearstr);
        if (select == 1) {
            *root = insertNodeButBirths(*root, year, name, region, population);
        }
        else {
            *root = insertNode(*root, year, name, region, population);
        }
    }

    file.close();
}

Node* search(Node* root, string region, int year, string event) {
    if (root == NULL) {
        return NULL;
    }

    Node* leftResult = search(root->left, region, year, event);
    if (leftResult != NULL)
        return leftResult;

    // Check if region, year, and event match
    if (root->region == region && root->year == year && root->name == event)
        return root;

    // If not matched, continue searching in the right subtree
    return search(root->right, region, year, event);
}






void search(Node* root) {
    string searchRegion, searchEvent;
    int searchYear;

    cout << "Enter the region you want to find: ";
    getline(cin, searchRegion);
    cout << "Enter the year you want to find: ";
    cin >> searchYear;

    Node* foundNode = search(root, searchRegion, searchYear, "Births");


    if (foundNode != NULL) {
        cout << "\nNode found: " << foundNode->year << ", " << foundNode->name << ", " << foundNode->region << ", " << foundNode->population << endl;
    }
    else {
        cout << "\nNode with region " << searchRegion << " and year " << searchYear << " for Births not found." << endl;
    }
}




Node* deleteNode(Node* root, string deleteRegion) {
    if (root == NULL) {
        return root;
    }

    if (deleteRegion < root->region)
        root->left = deleteNode(root->left, deleteRegion);
    else if (deleteRegion > root->region)
        root->right = deleteNode(root->right, deleteRegion);
    else {

        root->left = deleteNode(root->left, deleteRegion);
        root->right = deleteNode(root->right, deleteRegion);
        delete root;
        return NULL;
    }

    
    root->height = 1 + max(height(root->left), height(root->right));

   
    int balance = getBalance(root);

    //If this node becomes unbalanced, then there are 4 cases

   
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

 
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);


    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node* findMaxPopulation(Node* root) {
    if (root == NULL)
        return NULL;

    while (root->right != NULL)
        root = root->right;

    return root;
}

Node* findMinPopulation(Node* root) {
    if (root == NULL)
        return NULL;

    while (root->left != NULL)
        root = root->left;

    return root;
}



void modifyPopulation(Node* root) {
    string region;
    int year, newPopulation;

    cout << "Enter the region you want to modify population for:\n ";
    getline(cin, region);

    cout << "Enter the year: ";
    cin >> year;

    cout << "Enter the new population: ";
    cin >> newPopulation;

    
    if (year < 0 || newPopulation < 0) {
        cout << "Invalid input! Year and population must be non-negative." << endl;
        return;
    }

    Node* nodeToUpdate = search(root ,region, year,"Births");

   
    if (nodeToUpdate != NULL && nodeToUpdate->name == "Births") {
        nodeToUpdate->population = newPopulation;
        cout << "Population for region " << region << " in year " << year << " updated to " << newPopulation << endl;
    }
    else {
        cout << "Node with region " << region << " and year " << year << " not found or there were only deaths" << endl;
    }
}



int main() {
    string region_to_delete;
    string region = " ";
    Node* root = NULL;
    int num,question,most_least;
    int choice;
    HashTable hashTable(100);
    readFiletoHash(hashTable, "data.txt");
   do{ 
       cout << "\nWhat would you like to do ? \n"
            "1. Organise by Births\n"
            "2. Organise by region\n"
            "3.Use hashing to search, insert or delete\n"
            "4. Exit\n";
    cin >> question;
    switch (question)
    {
    case(1):
        readFileToAVLTree(&root, "data.txt", question);
        do {
            cout << "\nWhat would you like to do ? \n"
                "1. Show most Births\n"
                "2. Show least Births\n"
                "3.Show AVL tree in order\n"
                "4. Exit\n";
            cin >> most_least;
            Node* maxNode = findMaxPopulation(root);
            Node* minNode = findMinPopulation(root);
            switch (most_least)
            {
            case(1):
                if (maxNode != NULL)
                    cout << "Node with maximum population: " << maxNode->year << ", " << maxNode->name << ", " << maxNode->region << ", " << maxNode->population;
                else
                    cout << "AVL tree is empty.";
                break;
            case(2):
                if (minNode != NULL)
                    cout << "Node with minimum population: " << minNode->year << ", " << minNode->name << ", " << minNode->region << ", " << minNode->population;
                else
                    cout << "AVL tree is empty.";
                break;
            case(3):
                cout << "In Order Traversal of the created AVL Tree is:\n";
                inOrder(root, region);
                break;
            case(4):
                break;
            default:
                cout << "Invalid input\n";
                break;
            }
        } while (most_least != 4);
        break;
    case(2):
        readFileToAVLTree(&root, "data.txt", question);
        do {

            cout << "\nWhat would you like to do?\n"
                "1. Show AVL tree in order\n"
                "2. Search for birth population given region and year\n"
                "3. Modify birth population given region and year\n"
                "4. Delete a region\n"
                "5. Exit\n";
            cin >> num;

            switch (num) {
            case 1:
                cout << "In Order Traversal of the created AVL Tree is:\n";
                inOrder(root, region);
                break;
            case 2:
                cin.ignore();
                search(root);
                break;
            case 3:
                cin.ignore();
                modifyPopulation(root);
                break;
            case 4:
                cin.ignore();
                cout << "Give region to delete:\n ";
                getline(cin, region_to_delete);
                root = deleteNode(root, region_to_delete);
                break;
            case 5:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid option. Please try again.\n";
            }
        } while (num != 5);
        break;
    case(3):
        do {
            cout << "\nMenu:\n";
            cout << "1. Search for an event\n";
            cout << "2. Remove an entry\n";
            cout << "3. Change amount for an event\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                searchMod(hashTable);
                break;
            case 2: {
                string removed;
                cout << "Enter the place to remove: ";
                 cin.ignore();
                 getline(cin, removed);
                cout << removed;
                hashTable.remove(removed);
                cout << "\nEntry removed.\n";
                break;
            }
            case 3: {
                string place;
                int date;
                cout << "Enter the place: ";
                cin.ignore();
                getline(cin,place);
                cout << "Enter the date: ";
                cin >> date;
                hashTable.change_amount(place, date, "Births");
                break;
            }
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 4);
        break;
    case(4):
        cout << "Exiting programm...";
        break;
    default:
        cout << "Invalid input\n";
        break;
    }
}while (question != 4);
    

    return 0;
}

