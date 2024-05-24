#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <chrono>

using namespace std;

struct Data {
    int year;
    string eventType;
    string region;
    int count;
};

int stringToInt(const string& str) {
    int result;
    stringstream ss(str);
    ss >> result;
    return result;
}

class FileReader {
public:
    vector<Data> readDataFromFile(const string& filename) {
        vector<Data> data;
        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "Failed to open the file." << endl;
            return data;
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string token;
            Data d;

            if (getline(iss, token, ','))
                d.year = stringToInt(token);
            if (getline(iss, token, ','))
                d.eventType = token;
            if (getline(iss, token, ','))
                d.region = token;
            if (getline(iss, token))
                d.count = stringToInt(token);

            data.push_back(d);
        }

        file.close();
        return data;
    }
};

vector<Data> findDeaths(const vector<Data>& data) {
    vector<Data> deathRecords;
    for (const auto& entry : data) {
        if (entry.eventType == "Deaths") {
            deathRecords.push_back(entry);
        }
    }
    return deathRecords;
}

void printRecords(const vector<Data>& data) {
    for (const auto& d : data) {
        cout << "Year: " << d.year << ", Event Type: " << d.eventType
            << ", Region: " << d.region << ", Count: " << d.count << endl;
    }
}

void countingSort(vector<Data>& A, vector<Data>& B, int k) {
    auto start = chrono::steady_clock::now();
    vector<int> C(k + 1, 0);

    for (int j = 0; j < A.size(); ++j)
        C[A[j].count]++;

    for (int i = 1; i <= k; ++i)
        C[i] += C[i - 1];

    for (int j = A.size() - 1; j >= 0; --j) {
        B[C[A[j].count] - 1] = A[j];
        C[A[j].count]--;
    }

    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << "CountingSort: " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
}

void merge(vector<Data>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Data> L(n1);
    vector<Data> R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].count <= R[j].count) {
            arr[k] = L[i];
            ++i;
        }
        else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(vector<Data>& arr, int left, int right) {
    if (left < right) {
        auto start = chrono::steady_clock::now();

        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);

        if (left == 0 && right == arr.size() - 1) { // Check if it's the outermost call
            auto end = chrono::steady_clock::now();
            auto diff = end - start;
    cout << "MergeSort: " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
        }
    }
}
int partition(vector<Data>& arr, int low, int high) {
    int pivot = arr[high].count;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].count < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<Data>& arr, int low, int high) {
    if (low < high) {
        auto start = chrono::steady_clock::now();

        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);

        if (low == 0 && high == arr.size() - 1) { // Check if it's the outermost call
            auto end = chrono::steady_clock::now();
            auto diff = end - start;
            cout << "QuickSort: " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
        }
    }
}

void heapify(vector<Data>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].count > arr[largest].count)
        largest = left;

    if (right < n && arr[right].count > arr[largest].count)
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<Data>& arr) {
    auto start = chrono::steady_clock::now();

    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }

    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << "HeapSort: " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
}

class BinarySearchRegionSearcher {
public:
    vector<Data> searchRegions(const vector<Data>& data, int b1, int b2) {
        vector<Data> sortedData = data;
        insertionSort(sortedData);

        auto leftIt = binarySearch(sortedData, b1);

        vector<Data> result;
        for (auto it = leftIt; it != sortedData.end() && it->count <= b2; ++it) {
            result.push_back(*it);
        }

        return result;
    }

private:
void insertionSort(vector<Data>& data) {
    auto start = chrono::steady_clock::now();

    for (size_t i = 1; i < data.size(); ++i) {
        Data key = data[i];
        int j = i - 1;

        while (j >= 0 && data[j].count > key.count) {
            data[j + 1] = data[j];
            j = j - 1;
        }
        data[j + 1] = key;
    }

    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << "InsertionSort: " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
}

    vector<Data>::iterator binarySearch(vector<Data>& data, int key) {
        int low = 0;
        int high = data.size() - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2;

            if (data[mid].count == key)
                return data.begin() + mid;
            else if (data[mid].count < key)
                low = mid + 1;
            else
                high = mid - 1;
        }

        return data.begin() + low;
    }
};

class InterpolationSearchRegionSearcher {
public:
    vector<Data> searchRegions(const vector<Data>& data, int b1, int b2) {
        vector<Data> sortedData = data;
        insertionSort(sortedData);

        auto leftIt = interpolationSearch(sortedData, b1);

        vector<Data> result;
        for (auto it = leftIt; it != sortedData.end() && it->count <= b2; ++it) {
            result.push_back(*it);
        }

        return result;
    }

private:
void insertionSort(vector<Data>& data) {
    auto start = chrono::steady_clock::now();

    for (size_t i = 1; i < data.size(); ++i) {
        Data key = data[i];
        int j = i - 1;

        while (j >= 0 && data[j].count > key.count) {
            data[j + 1] = data[j];
            j = j - 1;
        }
        data[j + 1] = key;
    }

    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << "InsertionSort: " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
}

    vector<Data>::iterator interpolationSearch(vector<Data>& data, int key) {
        int low = 0;
        int high = data.size() - 1;

        while (low <= high && key >= data[low].count && key <= data[high].count) {
            int pos = low + ((key - data[low].count) * (high - low)) / (data[high].count - data[low].count);

            if (data[pos].count == key) {
                return data.begin() + pos;
            }
            else if (data[pos].count < key) {
                low = pos + 1;
            }
            else {
                high = pos - 1;
            }
        }

        return data.begin() + low;
    }
};
//ASK4##################################################################################
void readDataforask4(const string& filename, vector<Data>& data) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        int year = stoi(token);
        getline(ss, token, ',');
        string eventType = token;
        getline(ss, token, ',');
        string region = token;
        getline(ss, token, ',');
        int count = stoi(token);

        if (eventType == "Births") {
            Data d = { year, eventType, region, count };
            data.push_back(d);
        }
    }

    file.close();
}

int access(int x, vector<Data>& S, int start, int end,int flag) {
    int left = start;
    int right = end;

    if (x < S[left].count || x > S[right].count) {
        cout << x << " was not found." << endl;
        return -1;
    }

    int size = right - left + 1;
    int next = left + ((size * (x - S[left].count)) / (S[right].count - S[left].count));
    //removed +1 was causing problems;

    while (left <= right) {
        size = right - left + 1;

        if (size <= 3) {
            for (int j = left; j <= right; j++) {
                if (S[j].count == x) {
                    cout << x << " was found in position " << j << endl;
                    return j;
                }
            }
            cout << x << " was not found." << endl;
            return -1;
        }

        if (x == S[next].count) {
            if (flag = 0) {
                cout << x << " was found in position " << next << endl;
                return next;
            }
            else {
                return next;
            }
        }

        int i = 0;
        int jump = sqrt(size);

        if (x > S[next].count) {
            while (next + i * jump <= right && x > S[next + i * jump].count) {
                i++;
            }
            left = next + (i - 1) * jump + 1;
            right = min(next + i * jump, end);
        }
        else {
            while (next - i * jump >= left && x < S[next - i * jump].count) {
                i++;
            }
            right = next - (i - 1) * jump - 1;
            left = max(next - i * jump, start);
        }

        if (left <= right) {
            next = left + ((right - left) * (x - S[left].count)) / (S[right].count - S[left].count);
        }
        else {
            break;
        }
    }

    cout << x << " was not found." << endl;
    return -1;
}
void printVectorInRange(const vector<Data>& S, int start, int end) {
    cout << "\nRegions with birth counts in the range [" << start << ", " << end << "]\n";
    for (int i = start; i <= end; ++i) {
         cout << "Region: " << S[i].region << ", Year: " << S[i].year << ", Count: "
            << S[i].count <<  endl;
    }
}
void printVector(const vector<Data>& vec) {
    cout << "Vector contents:";
    for (int i = 0; i < vec.size(); ++i) {
        cout << " " << vec[i].count;
    }
    cout << endl;
}
int main() {
    FileReader reader;
    vector<Data> data = reader.readDataFromFile("data.txt");
    vector<Data> data2;
    readDataforask4("data.txt", data2);
    int choice, startIndex, endIndex,elementIndex;
    int start, end;
    int element;

    do {

         cout << "Choose:\n"
            "1.Exercise1\n"
            "2.Exercise2\n"
            "3.Exercise3\n"
            "4.Exercise4\n"
            "5.EXIT\n";
         cin >> choice;

        switch (choice) {
        case 1: {
            int sortingChoice;
            cout << "Choose sorting algorithm: 1 for MergeSort, 2 for QuickSort: ";
            cin >> sortingChoice;

            vector<Data> data_births;
            for (const auto& elements : data) {
                if (elements.eventType == "Births") {
                    data_births.push_back(elements);
                }
            }

            int n = data_births.size();

            if (sortingChoice == 1) {
                mergeSort(data_births, 0, n - 1);
                cout << "\nSorted by MergeSort:\n";
            }
            else if (sortingChoice == 2) {
                quickSort(data_births, 0, n - 1);
                 cout << "\nSorted by QuickSort:\n";
            }
            else {
                 cout << "Invalid choice. Exiting." <<  endl;
                return 1;
            }
            printRecords(data_births);
            break;
        }
        case 2: {
            int sortingChoice;
             cout << "Choose sorting algorithm: 1 for Heap Sort, 2 for Counting sort: ";
             cin >> sortingChoice;

             vector<Data> data_deaths;
            for (const auto& elements : data) {
                if (elements.eventType == "Deaths") {
                    data_deaths.push_back(elements);
                }
            }

            if (sortingChoice == 1) {
                heapSort(data_deaths);
                 cout << "\nSorted by Heap:\n";
            }
            else if (sortingChoice == 2) {

                int maxCount = 0;
                for (const auto& d : data_deaths) {
                    if (d.count > maxCount) maxCount = d.count;
                }


                vector<Data> sortedDeaths(data_deaths.size());


                countingSort(data_deaths, sortedDeaths, maxCount);


                data_deaths = sortedDeaths;

                 cout << "\nSorted by Counting:\n";
            }
            else {
                 cout << "Invalid choice. Exiting." <<  endl;
                return 1;
            }
            printRecords(data_deaths);
            break;
        }
        case 3: {
            int searchChoice;
             cout << "Choose searching algorithm: 1 for Binary Search, 2 for Interpolation Search: ";
             cin >> searchChoice;

            int b1, b2;
             cout << "Enter the range of birth counts (b1 and b2): ";
             cin >> b1 >> b2;

            switch (searchChoice) {
            case 1: {
                BinarySearchRegionSearcher binarySearcher;
                 vector<Data> regionsBinary = binarySearcher.searchRegions(data, b1, b2);

                 cout << "\nRegions with birth counts in the range [" << b1 << ", " << b2
                    << "] using Binary Search:\n";
                for (const auto& region : regionsBinary) {
                     cout << "Region: " << region.region << ", Year: " << region.year << ", Count: "
                        << region.count <<  endl;
                }
                break;
            }
            case 2: {
                InterpolationSearchRegionSearcher interpolationSearcher;
                 vector<Data> regionsInterpolation = interpolationSearcher.searchRegions(data, b1, b2);

                 cout << "\nRegions with birth counts in the range [" << b1 << ", " << b2
                    << "] using Interpolation Search:\n";
                for (const auto& region : regionsInterpolation) {
                     cout << "Region: " << region.region << ", Year: " << region.year << ", Count: "
                        << region.count <<  endl;
                }
                break;
            }
            default:
                 cout << "Invalid choice. Exiting." <<  endl;
                return 1;
            }
            break;
        }
        case 4:
            while (true) {
                quickSort(data2, 0, data2.size() - 1);
                printVector(data2);
                cout << "Please give the start and end of the space you want to search in (values must be part of the vector):" << endl;
                cin >> start >> end;

                startIndex = access(start, data2, 0, data2.size() - 1,0);
                endIndex = access(end,data2, 0, data2.size() - 1,0);

                if (startIndex != -1 && endIndex != -1) {
                    printVectorInRange(data2, startIndex, endIndex);
                    break;
                }
                else {
                    cout << "Invalid start or end. Please try again." << endl;
                }
            }

            cout << "Number to search: ";
            cin >> element;

            elementIndex = access(element, data2, 0, data2.size() - 1,1);
            startIndex = access(start, data2, 0, data2.size() - 1,1);
            endIndex = access(end,data2, 0, data2.size() - 1,1);
            if (elementIndex < startIndex || elementIndex > endIndex) {
                cout << element << " was not found." << endl;
            }
            else {
               access(element, data2, startIndex, endIndex,0);
            }
            break;
        case 5:
            cout << "Exiting...";
        default:
             cout << "Invalid choice. Exiting." <<  endl;
            return 1;
        }
    }while ( choice != 5);
    return 0;
}
