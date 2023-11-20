// YUSUF ERKAM KOKSAL 29512 CS300 HOMEWORK 4

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <list>
#include <sstream>

using namespace std;

// Contact icin C, Node icin V Template'leri kullan!!!

struct Contact {
    string fullName;
    string phoneNumber;
    string city;
    
    Contact(string fname, string phoneNum, string cityName)
        :  fullName(fname), phoneNumber(phoneNum), city(cityName) {}
    Contact()
    : fullName(""), phoneNumber(""), city(""){}
};

string ToUpper(const string& str) {
    string result;
    for (auto c : str) {
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
        result += c;
    }
    return result;
}

class Node {
public:
    Contact data;
    Node* left;
    Node* right;

    Node(Contact newData) : data(newData), left(NULL), right(NULL) {}
};

template <class C, class N>
class BinarySearchTree {
private:
    N* root;
    
    N* deleteNode(N* node, string fullName) {
        if (node == NULL) {
            return node;
        }

        string nodeFullName = ToUpper(node->data.fullName);

        if (ToUpper(fullName) < nodeFullName) {
            node->left = deleteNode(node->left, fullName);
        } else if (ToUpper(fullName) > nodeFullName) {
            node->right = deleteNode(node->right, fullName);
        } else {
            if (node->left == NULL) {
                N* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == NULL) {
                N* temp = node->left;
                delete node;
                return temp;
            }

            node->data = searchNode(node->right, fullName);
            node->right = deleteNode(node->right, node->data.fullName);
        }

        return node;
    }

    N* insertNode(N* node, C data) {
        if (node == NULL) {
            return new N(data);
        }

        string fullName = ToUpper(data.fullName);
        string nodeFullName = ToUpper(node->data.fullName);

        if (fullName < nodeFullName) {
            node->left = insertNode(node->left, data);
        } else if (fullName > nodeFullName) {
            node->right = insertNode(node->right, data);
        }

        return node;
    }

    C searchNode(N* node, string fullName) const {
        if (node == NULL) {
            return C{};
        }
        fullName = ToUpper(fullName);
        string nodeFullName = ToUpper(node->data.fullName);

        if (fullName < nodeFullName) {
            return searchNode(node->left, fullName);
        } else if (fullName > nodeFullName) {
            return searchNode(node->right, fullName);
        } else {
            return node->data;
        }
    }

public:
    BinarySearchTree() : root(NULL) {}

    void insert(C data) {
        root = insertNode(root, data);
    }

    C find(string fullName) const {
        return searchNode(root, fullName);
    }
    
    void remove(string fullName) {
        root = deleteNode(root, fullName);
    }
};

const int TABLE_SIZE = 53;
const double LOAD_FACTOR_UPPER_THRESHOLD = 0.9;

template<class C, class N>
class HashTable {
private:
    list<C> *table;
    int itemCount;
    int tableSize;

    int hash(string fullName) const {
        int hashValue = 0;
        fullName = ToUpper(fullName);
        for (char c : fullName) {
            hashValue = (hashValue * 31 + c) % tableSize;
        }
        return hashValue;
    }

    void rehash() {
        cout<<"rehashing..."<<endl;
        int oldTableSize = tableSize;
        tableSize = tableSize * 2 + 1;

        list<C> *newTable = new list<C>[tableSize];

        for (int i = 0; i < oldTableSize; i++) {
            for (C &contact : table[i]) {
                int newIndex = hash(contact.fullName);
                newTable[newIndex].push_back(contact);
            }
        }

        delete[] table;
        table = newTable;
        double loadFactor = (double) itemCount / tableSize;
        cout<<"previous table size: "<<oldTableSize<<" ,new table size: "<<tableSize<< " current unique item count: "<< itemCount<< " current load factor: "<< loadFactor<<endl;
    }

public:
    HashTable() : itemCount(0), tableSize(TABLE_SIZE) {
        table = new list<C>[tableSize];
    }
    

    ~HashTable() {
        delete[] table;
    }

    int getItemCount(){
        return itemCount;
    }
    
    int getTableSize(){
        return tableSize;
    }
    
    void insert(C data) {
        double loadFactor = (double) itemCount / tableSize;
        if (loadFactor > LOAD_FACTOR_UPPER_THRESHOLD) {
            rehash();
        }
        
        int index = hash(ToUpper(data.fullName));
        table[index].push_back(data);
        itemCount++;
        loadFactor = (double) itemCount / tableSize;

    }

    C find(string fullName) const {
        int index = hash(ToUpper(fullName));

        for (const C &contact : table[index]) {
            if (contact.fullName == fullName) {
                return contact;
            }
        }

        return C{};
    }
    
    void erase(string fullName) {
        int index = hash(ToUpper(fullName));

        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->fullName == fullName) {
                table[index].erase(it);
                itemCount--;
                break;
            }
        }
    }
};



using namespace std;

template<class C, class N>
void loadPhoneBook(BinarySearchTree<C,N>& bst, HashTable<C,N>& hashTable) {
    string fileName;
    cout<<"Enter the file name: "<<endl;
    cin>>fileName;
    ifstream inFile(fileName);
    string line;

    while (getline(inFile, line)) {
        
        istringstream iss(line);
        string firstName, lastName, phone, city;
        iss >> firstName >> lastName >> phone >> city;
        string fullNameTemp = firstName + " " + lastName;
        
        C newContact(ToUpper(fullNameTemp), phone, city);
        bst.insert(newContact);
        hashTable.insert(newContact);
    }
    
    cout<<endl;
    double loadFactorHT= static_cast<double>(hashTable.getItemCount()) / hashTable.getTableSize();
    cout<<"After preprocessing, the contact count is "<<hashTable.getItemCount()<<". Current load ratio is "<< loadFactorHT <<endl;
    cout<<endl;
    inFile.close();
}

template<class C, class N>
void searchAndTime(string fullName, BinarySearchTree<C,N>& bst, HashTable<C,N>& hashTable) {
    const int k = 500;

    fullName = ToUpper(fullName);
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        bst.find(fullName);
    }
    auto bstTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() / k;

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        hashTable.find(fullName);
    }
    auto hashTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() / k;

    cout<<endl;
    cout << "BST Search Time: " << bstTime <<endl;
    cout << "HashTable Search Time: " << hashTime <<endl;
    cout << "Speedup: " << (float)(bstTime / hashTime) <<endl;
    cout<<endl;
}

void selectList(){
    cout<<endl;
    cout<<"Choose which action to perform from 1 to 4:"<<endl;
    cout<<"1 - Search a phonebook contact"<<endl;
    cout<<"2 - Adding a phonebook contact"<<endl;
    cout<<"3 - Deleting a phonebook contact"<<endl;
    cout<<"4 - Press 4 to exit"<<endl;
}

template<class C, class N>
void SearchContact(BinarySearchTree<C,N>& bst, HashTable<C,N>& hashTable){
    
    string fullName;
    cout << "Enter full name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, fullName);

    fullName = ToUpper(fullName);
    C bstContact = bst.find(fullName);
    C hashContact = hashTable.find(fullName);
    
    cout<<"Searching for an item in the phonebook (BST) . . ."<<endl;
    cout<<"Phonebook: Searching for : ("<<fullName<<")"<<endl;
    cout<<"===================================="<<endl;
    
    if (bstContact.fullName == ""){
        cout<<"Name not found!"<<endl;
        cout<<endl;
    }
    else if(bstContact.fullName != ""){
        
        cout << bstContact.fullName << " " << bstContact.phoneNumber << " " << bstContact.city <<endl;
        cout<<endl;
    }
    
    cout<<"Searching for an item in the phonebook (HashTable) . . ."<<endl;
    cout<<"Phonebook: Searching for : ("<<fullName<<")"<<endl;
    cout<<"===================================="<<endl;
    if (hashContact.fullName == ""){
        cout<<"Name not found!"<<endl;
        cout<<endl;

    }
    else if(hashContact.fullName != ""){
        
        cout << hashContact.fullName << " " << hashContact.phoneNumber << " " << hashContact.city <<endl;
        cout<<endl;
    }
    searchAndTime(fullName, bst, hashTable);
}

template<class C, class N>
void AddContact(BinarySearchTree<C,N>& bst, HashTable<C,N>& hashTable){
    string cName,cTel,cCity;
    cout<<"Enter the information of the contact to be added:"<<endl;
    cout<<"Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin,cName);
    cout<<endl;
    cout<<"Tel: ";
    cin>>cTel;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout<<endl;
    cout<<"City: ";
    cin>>cCity;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout<<endl;
    
    cout<<"Adding an item to the phonebook (BST) . . ."<<endl;
    cout<<"===================================="<<endl;
    auto startAddBST = std::chrono::high_resolution_clock::now();
    C newContact(ToUpper(cName),cTel,cCity);
    bst.insert(newContact);
    auto endAddBST = std::chrono::high_resolution_clock::now();
    auto durationAddBST = chrono::duration_cast<chrono::nanoseconds>(endAddBST - startAddBST);
    cout<<"Contact has been added successfully to the BST Tree"<<endl;
    cout<<endl;
    
    cout<<"Adding an item to the phonebook (HashTable) . . ."<<endl;
    cout<<"===================================="<<endl;
    auto startAddHT = std::chrono::high_resolution_clock::now();
    hashTable.insert(newContact);
    auto endAddHT = std::chrono::high_resolution_clock::now();
    auto durationAddHT = chrono::duration_cast<chrono::nanoseconds>(endAddHT - startAddHT);
    cout<<"Contact has been added successfully to the HashTable"<<endl;
    cout<<endl;
    
    cout<<"Adding a contact to the Binary Tree took " << durationAddBST.count() <<" nanoseconds. . ."<<endl;
    cout<<"Adding a contact to the HashTable took " << durationAddHT.count() <<" nanoseconds. . ."<<endl;
    cout<<endl;
    return;
}

template<class C, class N>
void deleteAndTime(string fullName, BinarySearchTree<C,N>& bst, HashTable<C,N>& hashTable) {
    auto start = chrono::high_resolution_clock::now();
    fullName = ToUpper(fullName);
    C bstContact = bst.find(fullName);
    C hashTableContact = hashTable.find(fullName);
    
    if (bstContact.fullName != ""){
        bst.remove(fullName);
    }
    else{
        cout<<"Name not found!"<<endl;
        cout<<endl;
    }
    auto bstTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count();

    start = chrono::high_resolution_clock::now();
    
    if (hashTableContact.fullName != ""){
        hashTable.erase(fullName);
    }
    else{
        cout<<"Name not found!"<<endl;
        cout<<endl;
    }
    auto hashTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count();
    cout<<endl;
    cout<<"Deleted successfully . . ."<<endl;
    cout<<endl;
    cout << "Deletion from the Binary Tree took " << bstTime << " ns\n";
    cout << "Deletion from the Hashtable took " << hashTime << " ns\n";
    cout<<endl;
}


template<class C, class N>
void DeleteContact(BinarySearchTree<C,N>& bst, HashTable<C,N>& hashTable) {
    string deleteName;
    cout << "Enter the full name of the contact to delete: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, deleteName);
    deleteName = ToUpper(deleteName);

    deleteAndTime(deleteName, bst, hashTable);
}

int main() {
    BinarySearchTree<Contact,Node> bst;
    HashTable<Contact,Node> hashTable;
    
    loadPhoneBook(bst, hashTable);
    
    int choice;
    selectList();
    cin >> choice;
    cout<<endl;
    
    while (choice != 0) {
        
        if (choice == 1) {
            SearchContact(bst, hashTable);
        }
        else if (choice == 2) {
            AddContact(bst, hashTable);
        }
        else if (choice == 3) {
            DeleteContact(bst, hashTable);
        }
        else if (choice == 4) {
            break;
        }
        selectList();
        cin.clear();
        cin >> choice;
        cout<<endl;
        
    }
    return 0;
}
