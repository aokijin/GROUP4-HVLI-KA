#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

// ============================================================
//  CONSTANTS & VIOLATION TYPE LISTS  (Array / List)
// ============================================================

const vector<string> MAJOR_VIOLATIONS = {
    "Driving Without a License",
    "Driving Under the Influence (DUI)",
    "Reckless Driving",
    "Unregistered Vehicle",
    "Driving Without Helmet (Rider)",
    "Counterflowing"
};

const vector<string> MINOR_VIOLATIONS = {
    "Improper Helmet Use",
    "No Side Mirror",
    "Missing Documents (OR/CR)",
    "Expired Registration",
    "No Plate Number Display",
    "Overloading (Exceeding Passenger Limit)"
};

// Priority levels
const int PRIORITY_MAJOR = 1;   // higher priority (lower number = dequeued first)
const int PRIORITY_MINOR = 2;

// ============================================================
//  VIOLATION RECORD STRUCT
// ============================================================

struct ViolationRecord {
    int    id;
    string plateNumber;
    int    age;
    string violationType;   // "MAJOR" or "MINOR"
    string violation;
    int    priority;        // 1 = major, 2 = minor

    // For display
    string toString() const {
        ostringstream oss;
        oss << "| ID: "    << left << setw(4)  << id
            << "| Plate: " << left << setw(12) << plateNumber
            << "| Age: "   << left << setw(5)  << age
            << "| Type: "  << left << setw(7)  << violationType
            << "| Violation: " << violation;
        return oss.str();
    }
};

// ============================================================
//  PRIORITY QUEUE COMPARATOR  (lower number = higher priority)
// ============================================================

struct CompareViolation {
    bool operator()(const ViolationRecord& a, const ViolationRecord& b) {
        return a.priority > b.priority;   // max-heap inverted → min-heap on priority
    }
};

// ============================================================
//  GLOBAL DATA STRUCTURES
// ============================================================

// Stack – undo last entry
stack<ViolationRecord> undoStack;

// Queue – motorcycle queue at checkpoint (FIFO order of arrival)
queue<string> motorcycleQueue;   // stores plate numbers waiting for inspection

// Priority Queue – process violations by severity
priority_queue<ViolationRecord, vector<ViolationRecord>, CompareViolation> violationPQ;

// Hash Table – fast lookup by plate number
unordered_map<string, vector<ViolationRecord>> violationHashTable;

// Master list (for display / sequential tracking)
vector<ViolationRecord> allViolations;

int recordIdCounter = 1;

// ============================================================
//  UTILITY FUNCTIONS
// ============================================================

void printDivider(char ch = '=', int width = 70) {
    cout << string(width, ch) << "\n";
}

void printHeader() {
    printDivider();
    cout << "      HVLI-KA: High-Priority Violation Logging and\n";
    cout << "               Intelligent Knowledge of Infractions\n";
    cout << "      Batangas State University | CC 103 | Team 4\n";
    printDivider();
}

void pressEnterToContinue() {
    cout << "\nPress ENTER to continue...";
    cin.ignore();
    cin.get();
}

string toUpper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

// ============================================================
//  FEATURE 1: ADD VIOLATION
// ============================================================

void addViolation() {
    printDivider('-');
    cout << "  ADD VIOLATION RECORD\n";
    printDivider('-');

    ViolationRecord rec;
    rec.id = recordIdCounter++;

    // Step 1 – Major or Minor?
    int typeChoice = 0;
    cout << "\n  Select Violation Category:\n";
    cout << "    [1] MAJOR Violation\n";
    cout << "    [2] MINOR Violation\n";
    cout << "  Choice: ";
    cin >> typeChoice;
    cin.ignore();

    if (typeChoice == 1) {
        rec.violationType = "MAJOR";
        rec.priority      = PRIORITY_MAJOR;

        cout << "\n  MAJOR Violations:\n";
        for (int i = 0; i < (int)MAJOR_VIOLATIONS.size(); i++) {
            cout << "    [" << (i + 1) << "] " << MAJOR_VIOLATIONS[i] << "\n";
        }
        int vChoice = 0;
        cout << "  Select violation number: ";
        cin >> vChoice;
        cin.ignore();
        if (vChoice < 1 || vChoice > (int)MAJOR_VIOLATIONS.size()) {
            cout << "  [!] Invalid choice. Returning to menu.\n";
            recordIdCounter--;
            return;
        }
        rec.violation = MAJOR_VIOLATIONS[vChoice - 1];

    } else if (typeChoice == 2) {
        rec.violationType = "MINOR";
        rec.priority      = PRIORITY_MINOR;

        cout << "\n  MINOR Violations:\n";
        for (int i = 0; i < (int)MINOR_VIOLATIONS.size(); i++) {
            cout << "    [" << (i + 1) << "] " << MINOR_VIOLATIONS[i] << "\n";
        }
        int vChoice = 0;
        cout << "  Select violation number: ";
        cin >> vChoice;
        cin.ignore();
        if (vChoice < 1 || vChoice > (int)MINOR_VIOLATIONS.size()) {
            cout << "  [!] Invalid choice. Returning to menu.\n";
            recordIdCounter--;
            return;
        }
        rec.violation = MINOR_VIOLATIONS[vChoice - 1];

    } else {
        cout << "  [!] Invalid category. Returning to menu.\n";
        recordIdCounter--;
        return;
    }

    // Step 2 – Rider details
    cout << "\n  Enter Plate Number : ";
    getline(cin, rec.plateNumber);
    rec.plateNumber = toUpper(rec.plateNumber);

    cout << "  Enter Rider Age    : ";
    cin >> rec.age;
    cin.ignore();

    // Store in all data structures
    allViolations.push_back(rec);
    undoStack.push(rec);
    violationPQ.push(rec);
    violationHashTable[rec.plateNumber].push_back(rec);

    // Also enqueue plate to motorcycle queue if not already present
    // (simulate rider arriving at checkpoint)
    motorcycleQueue.push(rec.plateNumber);

    cout << "\n  [✓] Violation recorded successfully! (ID: " << rec.id << ")\n";
    printDivider('-');
    pressEnterToContinue();
}

// ============================================================
//  FEATURE 2: VIEW ALL VIOLATIONS (ordered list)
// ============================================================

void viewAllViolations() {
    printDivider('-');
    cout << "  ALL VIOLATION RECORDS\n";
    printDivider('-');

    if (allViolations.empty()) {
        cout << "  No violations recorded yet.\n";
    } else {
        cout << left
             << setw(6)  << "  ID"
             << setw(13) << "Plate"
             << setw(6)  << "Age"
             << setw(8)  << "Type"
             << "Violation\n";
        printDivider('-');
        for (const auto& rec : allViolations) {
            cout << "  "
                 << left << setw(6)  << rec.id
                 << left << setw(13) << rec.plateNumber
                 << left << setw(6)  << rec.age
                 << left << setw(8)  << rec.violationType
                 << rec.violation << "\n";
        }
    }
    printDivider('-');
    pressEnterToContinue();
}

// ============================================================
//  FEATURE 3: SEARCH BY PLATE NUMBER  (Hash Table)
// ============================================================

void searchByPlate() {
    printDivider('-');
    cout << "  SEARCH VIOLATION BY PLATE NUMBER\n";
    printDivider('-');

    string plate;
    cout << "  Enter Plate Number to search: ";
    cin.ignore();
    getline(cin, plate);
    plate = toUpper(plate);