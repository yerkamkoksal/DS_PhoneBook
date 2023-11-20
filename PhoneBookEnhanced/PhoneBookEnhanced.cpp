// YUSUF ERKAM KOKSAL 29512 CS300 HOMEWORK 5

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;

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
template<class T>
void binarySearch(vector<T>& persons, const string& query) {
    int low = 0, high = persons.size() - 1;
    bool found = false;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (persons[mid].fullName.substr(0, query.size()) < query) {
            low = mid + 1;
        } else if (persons[mid].fullName.substr(0, query.size()) > query) {
            high = mid - 1;
        } else {
            found = true;
            // check for multiple occurrences
            int temp = mid;
            while (temp >= 0 && persons[temp].fullName.substr(0, query.size()) == query) {
                cout << persons[temp].fullName << " " << persons[temp].phoneNumber << " " << persons[temp].city << "\n";
                temp--;
            }
            temp = mid + 1;
            while (temp < persons.size() && persons[temp].fullName.substr(0, query.size()) == query) {
                cout << persons[temp].fullName << " " << persons[temp].phoneNumber << " " << persons[temp].city << "\n";
                temp++;
            }
            break;
        }
    }
    if (!found) {
        cout << query << " does NOT exist in the dataset\n";
    }
}

// Sequential Search
template<class T>
void sequentialSearch(vector<T>& persons, const string& query) { // QUERYI INPUT ALIRKEN CAPITAL ALMAM GEREKLI!!!!!
    
    bool found = false;
    for(const auto& person : persons) {
        string fullNameUpper = ToUpper(person.fullName);
        
        if(fullNameUpper.find(query) != string::npos) {
            cout << person.fullName << " " << person.phoneNumber << " " << person.city << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << query << " does NOT exist in the dataset\n";
    }
}

// Heap Sort
template<class T>
void heapify(vector<T>& persons, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && persons[left].fullName > persons[largest].fullName)
        largest = left;
    if (right < n && persons[right].fullName > persons[largest].fullName)
        largest = right;
    if (largest != i) {
        swap(persons[i], persons[largest]);
        heapify(persons, n, largest);
    }
}

template<class T>
void heapSort(vector<T>& persons) {
    int n = persons.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(persons, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(persons[0], persons[i]);
        heapify(persons, i, 0);
    }
}

// Quick Sort
template<class T>
int partition(vector<T>& persons, int low, int high) {
    auto pivot = persons[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (persons[j].fullName < pivot.fullName) {
            i++;
            swap(persons[i], persons[j]);
        }
    }
    swap(persons[i + 1], persons[high]);
    return (i + 1);
}

template<class T>
void quickSort(vector<T>& persons, int low, int high) {
    if (low < high) {
        int pi = partition(persons, low, high);
        quickSort(persons, low, pi - 1);
        quickSort(persons, pi + 1, high);
    }
}

template<class T>
void merge(vector<T>& persons, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<T> L(n1), R(n2);
    for (i = 0; i < n1; i++)
        L[i] = persons[left + i];
    for (j = 0; j < n2; j++)
        R[j] = persons[mid + 1 + j];
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i].fullName <= R[j].fullName) {
            persons[k] = L[i];
            i++;
        } else {
            persons[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        persons[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        persons[k] = R[j];
        j++;
        k++;
    }
}

template<class T>
void mergeSort(vector<T>& persons, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(persons, left, mid);
        mergeSort(persons, mid + 1, right);
        merge(persons, left, mid, right);
    }
}

// Insertion Sort
template<class T>
void insertionSort(vector<T>& persons) {
    int i, j;
    T key;
    for (i = 1; i < persons.size(); i++) {
        key = persons[i];
        j = i - 1;

        while (j >= 0 && persons[j].fullName > key.fullName) {
            persons[j + 1] = persons[j];
            j = j - 1;
        }
        persons[j + 1] = key;
    }
}


template<class T>
void loadPhoneBook(vector<T>& persons) {

    string fileName;
    cout << "Please enter the contact file name:\n";
    cin >> fileName;

    // read from file
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string firstName, lastName, phone, city;
        iss >> firstName >> lastName >> phone >> city;
        string fullNameTemp = firstName + " " + lastName;
        
        T newContact(ToUpper(fullNameTemp), phone, city);
        persons.push_back(newContact);
    }
    file.close();
    return;
}

int main() {
    vector<Contact> phoneBook;
    loadPhoneBook(phoneBook);

    string query;
    cout << "Please enter the word to be queried:\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, query);
    cout<<endl;
    string queryUpper = ToUpper(query);
    
    vector<Contact> personsQuickSort = phoneBook;
    vector<Contact> personsInsertionSort = phoneBook;
    vector<Contact> personsMergeSort = phoneBook;
    vector<Contact> personsHeapSort = phoneBook;

    // measure sorting times
    cout << "Sorting the vector copies\n======================================\n";
    auto start = high_resolution_clock::now();
    quickSort(personsQuickSort, 0, personsQuickSort.size() - 1);
    auto stop = high_resolution_clock::now();
    auto durationQuickSort = duration_cast<nanoseconds>(stop - start);
    cout << "Quick Sort Time: " << durationQuickSort.count() << " Nanoseconds\n";

    start = high_resolution_clock::now();
    insertionSort(personsInsertionSort);
    stop = high_resolution_clock::now();
    auto durationInsertionSort = duration_cast<nanoseconds>(stop - start);
    cout << "Insertion Sort Time: " << durationInsertionSort.count() << " Nanoseconds\n";

    start = high_resolution_clock::now();
    mergeSort(personsMergeSort, 0, personsMergeSort.size() - 1);
    stop = high_resolution_clock::now();
    auto durationMergeSort = duration_cast<nanoseconds>(stop - start);
    cout << "Merge Sort Time: " << durationMergeSort.count() << " Nanoseconds\n";

    start = high_resolution_clock::now();
    heapSort(personsHeapSort);
    stop = high_resolution_clock::now();
    auto durationHeapSort = duration_cast<nanoseconds>(stop - start);
    cout << "Heap Sort Time: " << durationHeapSort.count() << " Nanoseconds\n";

    // measure search times
    cout<<endl;
    cout << "Searching for " << query << "\n======================================\n";
    start = high_resolution_clock::now();
    binarySearch(personsQuickSort, queryUpper);
    stop = high_resolution_clock::now();
    auto durationBinarySearch = duration_cast<nanoseconds>(stop - start);
    cout << "Binary Search Time: " << durationBinarySearch.count() << " Nanoseconds\n";
    cout<<endl;

    cout<<"Search results for Sequential Search: "<<endl;
    start = high_resolution_clock::now();
    sequentialSearch(phoneBook, queryUpper);
    stop = high_resolution_clock::now();
    auto durationSequentialSearch = duration_cast<nanoseconds>(stop - start);
    cout << "Sequential Search Time: " << durationSequentialSearch.count() << " Nanoseconds\n";

    // calculate and output speed-up ratios
    cout<<endl;
    cout << "SpeedUp between Search Algorithms\n======================================\n";
    cout << "(Sequential Search/ Binary Search) SpeedUp = " << (double)durationSequentialSearch.count() / durationBinarySearch.count() << "\n";
    cout<<endl;
    cout << "SpeedUps between Sorting Algorithms\n======================================\n";
    cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << (double)durationInsertionSort.count() / durationQuickSort.count() << "\n";
    cout << "(Merge Sort / Quick Sort) SpeedUp = " << (double)durationMergeSort.count() / durationQuickSort.count() << "\n";
    cout << "(Heap Sort / Quick Sort) SpeedUp = " << (double)durationHeapSort.count() / durationQuickSort.count() << "\n";
    cout<<endl;



    return 0;
}
