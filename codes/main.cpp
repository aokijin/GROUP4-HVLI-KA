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
