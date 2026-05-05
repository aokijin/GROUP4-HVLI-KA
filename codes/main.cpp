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

     auto it = violationHashTable.find(plate);
    if (it == violationHashTable.end() || it->second.empty()) {
        cout << "  [!] No records found for plate: " << plate << "\n";
    } else {
        cout << "\n  Records found for plate [" << plate << "]:\n";
        printDivider('-');
        cout << left
             << setw(7)  << "ID"
             << setw(8)  << "Type"
             << setw(6)  << "Age"
             << "Violation\n";
        printDivider('-');
        for (const auto& rec : it->second) {
            cout << "  "
                 << left << setw(7) << rec.id
                 << left << setw(8) << rec.violationType
                 << left << setw(6) << rec.age
                 << rec.violation << "\n";
        }
    }
    printDivider('-');
    pressEnterToContinue();
}

// ============================================================
//  FEATURE 4: PROCESS NEXT HIGH-PRIORITY VIOLATION (Priority Queue)
// ============================================================

void processNextViolation() {
    printDivider('-');
    cout << "  PROCESS NEXT HIGH-PRIORITY VIOLATION\n";
    printDivider('-');

    if (violationPQ.empty()) {
        cout << "  [!] No violations in the priority queue.\n";
    } else {
        ViolationRecord top = violationPQ.top();
        violationPQ.pop();
        cout << "\n  Processing next violation (highest priority):\n\n";
        cout << "    Record ID  : " << top.id           << "\n";
        cout << "    Plate No.  : " << top.plateNumber  << "\n";
        cout << "    Age        : " << top.age           << "\n";
        cout << "    Type       : " << top.violationType << "\n";
        cout << "    Violation  : " << top.violation     << "\n";
        cout << "\n  [✓] Violation processed and removed from priority queue.\n";
    }
    printDivider('-');
    pressEnterToContinue();
}

// ============================================================
//  FEATURE 5: MOTORCYCLE CHECKPOINT QUEUE  (Queue / FIFO)
// ============================================================

void manageCheckpointQueue() {
    printDivider('-');
    cout << "  MOTORCYCLE CHECKPOINT QUEUE\n";
    printDivider('-');

    int choice = 0;
    cout << "\n  [1] View current queue\n";
    cout << "  [2] Admit next motorcycle for inspection\n";
    cout << "  [3] Manually add plate to queue\n";
    cout << "  Choice: ";
    cin >> choice;
    cin.ignore();

       if (choice == 1) {
        if (motorcycleQueue.empty()) {
            cout << "\n  [!] Checkpoint queue is empty.\n";
        } else {
            queue<string> temp = motorcycleQueue;
            int pos = 1;
            cout << "\n  Queue (front → back):\n";
            while (!temp.empty()) {
                cout << "    " << pos++ << ". " << temp.front() << "\n";
                temp.pop();
            }
        }
    } else if (choice == 2) {
        if (motorcycleQueue.empty()) {
            cout << "\n  [!] Queue is empty. No motorcycle to admit.\n";
        } else {
            string plate = motorcycleQueue.front();
            motorcycleQueue.pop();
            cout << "\n  [✓] Admitting motorcycle with plate: " << plate << "\n";
        }

    } else if (choice == 3) {
        string plate;
        cout << "\n  Enter plate number to enqueue: ";
        getline(cin, plate);
        motorcycleQueue.push(toUpper(plate));
        cout << "  [✓] Plate " << toUpper(plate) << " added to queue.\n";

    } else {
        cout << "\n  [!] Invalid choice.\n";
    }

    printDivider('-');
    pressEnterToContinue();
}

// ============================================================
//  FEATURE 6: UNDO LAST ENTRY  (Stack / LIFO)
// ============================================================

void undoLastEntry() {
    printDivider('-');
    cout << "  UNDO LAST VIOLATION ENTRY\n";
    printDivider('-');

    if (undoStack.empty()) {
        cout << "  [!] No entries to undo.\n";
    } else {
        ViolationRecord last = undoStack.top();
        undoStack.pop();

        // Remove from allViolations
        for (auto it = allViolations.begin(); it != allViolations.end(); ++it) {
            if (it->id == last.id) {
                allViolations.erase(it);
                break;
            }
        }

        // Remove from hash table
        auto& vec = violationHashTable[last.plateNumber];
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            if (it->id == last.id) {
                vec.erase(it);
                break;
            }
        }

        cout << "\n  [✓] Undone last entry:\n";
        cout << "    ID        : " << last.id           << "\n";
        cout << "    Plate     : " << last.plateNumber  << "\n";
        cout << "    Type      : " << last.violationType << "\n";
        cout << "    Violation : " << last.violation     << "\n";
        cout << "\n  Note: Entry also removed from hash table and violation list.\n";
        cout << "        Priority queue will no longer process this entry.\n";
    }
    printDivider('-');
    pressEnterToContinue();
}

// ============================================================
//  FEATURE 7: GENERATE REPORT
// ============================================================

void generateReport() {
    printDivider('-');
    cout << "  VIOLATION REPORT\n";
    printDivider('-');

    int majorCount = 0, minorCount = 0;
    for (const auto& rec : allViolations) {
        if (rec.violationType == "MAJOR") majorCount++;
        else                             minorCount++;
    }

    cout << "\n  Total Violations Recorded : " << allViolations.size() << "\n";
    cout << "  Major Violations          : " << majorCount            << "\n";
    cout << "  Minor Violations          : " << minorCount            << "\n";
    cout << "  Pending in Priority Queue : " << violationPQ.size()    << "\n";
    cout << "  In Checkpoint Queue       : " << motorcycleQueue.size()<< "\n";
    cout << "  Plates on Record (HT)     : " << violationHashTable.size() << "\n";

    if (!allViolations.empty()) {
        cout << "\n  --- Violation Breakdown ---\n";

        // Count each specific violation
        unordered_map<string, int> freq;
        for (const auto& rec : allViolations) freq[rec.violation]++;

        // Sort by frequency descending
        vector<pair<string, int>> sorted(freq.begin(), freq.end());
        sort(sorted.begin(), sorted.end(),
             [](const pair<string,int>& a, const pair<string,int>& b) {
                 return a.second > b.second;
             });

        for (const auto& p : sorted) {
            cout << "    " << left << setw(40) << p.first << " : " << p.second << "\n";
        }
    }

    printDivider('-');
    pressEnterToContinue();
}

// ============================================================
//  FEATURE 8: VIEW VIOLATION TYPE LISTS  (Array / List)
// ============================================================

void viewViolationTypes() {
    printDivider('-');
    cout << "  VIOLATION TYPE REFERENCE LIST\n";
    printDivider('-');

    cout << "\n  MAJOR VIOLATIONS (Priority Level 1):\n";
    for (int i = 0; i < (int)MAJOR_VIOLATIONS.size(); i++) {
        cout << "    " << (i + 1) << ". " << MAJOR_VIOLATIONS[i] << "\n";
    }

    cout << "\n  MINOR VIOLATIONS (Priority Level 2):\n";
    for (int i = 0; i < (int)MINOR_VIOLATIONS.size(); i++) {
        cout << "    " << (i + 1) << ". " << MINOR_VIOLATIONS[i] << "\n";
    }

    printDivider('-');
    pressEnterToContinue();
}

// ============================================================
//  MAIN MENU
// ============================================================

void showMainMenu() {
    printDivider();
    cout << "      HVLI-KA  -  Main Menu\n";
    printDivider();
    cout << "  [1] Add Violation Record\n";
    cout << "  [2] View All Violation Records\n";
    cout << "  [3] Search Record by Plate Number\n";
    cout << "  [4] Process Next High-Priority Violation\n";
    cout << "  [5] Manage Checkpoint Motorcycle Queue\n";
    cout << "  [6] Undo Last Violation Entry\n";
    cout << "  [7] Generate Violation Report\n";
    cout << "  [8] View Violation Type Lists\n";
    cout << "  [0] Exit\n";
    printDivider();
    cout << "  Enter choice: ";
}

// ============================================================
//  MAIN
// ============================================================

int main() {
    printHeader();
    cout << "\n  Welcome, Officer! System ready.\n";
    pressEnterToContinue();

    int choice = -1;
    while (choice != 0) {
        showMainMenu();
        cin >> choice;
        cin.ignore();

          switch (choice) {
            case 1: addViolation();          break;
            case 2: viewAllViolations();     break;
            case 3: searchByPlate();         break;
            case 4: processNextViolation();  break;
            case 5: manageCheckpointQueue(); break;
            case 6: undoLastEntry();         break;
            case 7: generateReport();        break;
            case 8: viewViolationTypes();    break;
            case 0:
                printDivider();
                cout << "  Thank you for using HVLI-KA. Stay safe on the road!\n";
                printDivider();
                break;
            default:
                cout << "\n  [!] Invalid choice. Please try again.\n";
                pressEnterToContinue();
                break;
        }
    }

    return 0;
}
