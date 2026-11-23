// Course: CAP455 - Object Oriented Programming using C++
// Phase: CA1 (Design & Create - Units I & II)
// Project: Theatre Rehearsal Cast and Prop Manager
// Student: Priyanshu Singh (Reg No: 12613305)
// Submitted To: Mr. Adarsh Kumar Singh

#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

// Enumeration for identifying whether the entry is a person or a prop
enum ResourceType {
    CAST_MEMBER = 1,
    STAGE_PROP = 2
};

// Enumeration for tracking current rehearsal readiness
enum ReadinessStatus {
    PENDING = 1,
    READY = 2,
    NEEDS_ATTENTION = 3
};

class RehearsalResource {
private:
    int resourceId;             // Unique Primary Key (1001 to 9999)
    int resourceType;           // 1 = Cast Member, 2 = Stage Prop
    char resourceName[50];      // Fixed-size array for safe memory storage
    char roleOrCategory[40];    // Actor role or Prop category
    int assignedScene;          // Scene number (1 to 25)
    int rehearsalMetric;        // Rehearsal hours (Cast) or Quantity (Prop)
    int readinessStatus;        // 1 = Pending, 2 = Ready, 3 = Issue

    // Static variable shared by all objects to count total created records
    static int totalResourcesCreated;

public:
    // Default constructor to set safe initial values
    RehearsalResource() {
        resourceId = 0;
        resourceType = CAST_MEMBER;
        strcpy(resourceName, "Not Set");
        strcpy(roleOrCategory, "None");
        assignedScene = 1;
        rehearsalMetric = 0;
        readinessStatus = PENDING;
    }

    // Using 'this' pointer to assign parameter values to class member variables
    void setResourceData(int resourceId, int resourceType, string nameStr, 
                         string roleStr, int assignedScene, int rehearsalMetric, 
                         int readinessStatus) {
        this->resourceId = resourceId;
        this->resourceType = resourceType;
        this->assignedScene = assignedScene;
        this->rehearsalMetric = rehearsalMetric;
        this->readinessStatus = readinessStatus;

        // Safely copy validated std::string into fixed-size char array
        strncpy(this->resourceName, nameStr.c_str(), 49);
        this->resourceName[49] = '\0';

        strncpy(this->roleOrCategory, roleStr.c_str(), 39);
        this->roleOrCategory[39] = '\0';
    }

    // Inline getter functions for fast access during pointer search
    inline int getResourceId() const {
        return resourceId;
    }

    inline int getResourceType() const {
        return resourceType;
    }

    inline int getAssignedScene() const {
        return assignedScene;
    }

    string getResourceName() const {
        return string(resourceName);
    }

    string getRoleOrCategory() const {
        return string(roleOrCategory);
    }

    // Static functions to manage and read the shared counter
    static void incrementResourceCount() {
        totalResourcesCreated++;
    }

    static int getTotalResourcesCreated() {
        return totalResourcesCreated;
    }

    // Display a single record as a formatted row in the table
    void displayRecordRow() const {
        string typeLabel = (resourceType == CAST_MEMBER) ? "Cast Member" : "Stage Prop";
        string statusLabel = "Pending";
        if (readinessStatus == READY) {
            statusLabel = "Ready";
        } else if (readinessStatus == NEEDS_ATTENTION) {
            statusLabel = "Issue";
        }

        cout << left << setw(8)  << resourceId
             << setw(14) << typeLabel
             << setw(22) << resourceName
             << setw(18) << roleOrCategory
             << setw(8)  << assignedScene
             << setw(10) << rehearsalMetric
             << setw(10) << statusLabel << endl;
    }

    // Friend function declaration so it can access private variables directly
    friend void printDetailedAuditCard(const RehearsalResource& res);
};

// Initialize static member outside the class
int RehearsalResource::totalResourcesCreated = 0;

// Friend function definition: directly reads private attributes of RehearsalResource
void printDetailedAuditCard(const RehearsalResource& res) {
    cout << "\n--------------------------------------------------\n";
    cout << "       DETAILED RESOURCE AUDIT (FRIEND ACCESS)    \n";
    cout << "--------------------------------------------------\n";
    cout << "Resource ID (Key) : " << res.resourceId << "\n";
    cout << "Category Type     : " << (res.resourceType == CAST_MEMBER ? "Cast Member (Actor/Crew)" : "Physical Stage Prop") << "\n";
    cout << "Name / Title      : " << res.resourceName << "\n";
    cout << "Role / Prop Type  : " << res.roleOrCategory << "\n";
    cout << "Assigned Scene    : Scene " << res.assignedScene << "\n";

    if (res.resourceType == CAST_MEMBER) {
        cout << "Rehearsal Hours   : " << res.rehearsalMetric << " hrs\n";
    } else {
        cout << "Prop Quantity     : " << res.rehearsalMetric << " units\n";
    }

    cout << "Readiness Status  : ";
    if (res.readinessStatus == PENDING) cout << "Pending Rehearsal Check\n";
    else if (res.readinessStatus == READY) cout << "Ready for Stage\n";
    else cout << "Needs Attention / Repair\n";

    cout << "Memory Byte Size  : " << sizeof(res) << " bytes\n";
    cout << "--------------------------------------------------\n";
}

// Check duplicate ID using an Object Pointer (Unit II concept)
bool isDuplicateKey(const RehearsalResource* catalogPtr, int count, int newId) {
    const RehearsalResource* ptr = catalogPtr;
    for (int i = 0; i < count; i++) {
        if (ptr->getResourceId() == newId) {
            return true; // Duplicate found
        }
        ptr++; // Pointer arithmetic: move to next object in array
    }
    return false;
}

// Helper function using Call by Reference (&) to read and validate integer range
void getValidatedInt(const string& prompt, int& valueOut, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        if (cin >> valueOut && valueOut >= minVal && valueOut <= maxVal) {
            cin.ignore(1000, '\n'); // Clear newline from input buffer
            break;
        } else {
            cout << "   [Invalid Input] Please enter a number between " 
                 << minVal << " and " << maxVal << ".\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// Helper function using std::string modifiers to validate text length
string getValidatedString(const string& prompt, size_t minLen, size_t maxLen) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Trim leading spaces using std::string erase and find_first_not_of
        size_t firstChar = input.find_first_not_of(" \t");
        if (firstChar == string::npos) {
            cout << "   [Invalid Input] Text cannot be empty or only spaces.\n";
            continue;
        }
        input.erase(0, firstChar);

        if (input.length() >= minLen && input.length() <= maxLen) {
            return input;
        }
        cout << "   [Invalid Input] Length must be between " 
             << minLen << " and " << maxLen << " characters.\n";
    }
}

int main() {
    const int MAX_CAPACITY = 100;
    RehearsalResource catalog[MAX_CAPACITY]; // Array of Objects
    int currentCount = 0;

    // 2D Multidimensional Array: 5 Stage Zones (Scenes 1-5) x 3 Rehearsal Slots (Morning, Afternoon, Evening)
    int stageSlotMatrix[5][3] = {0};

    int choice = 0;
    while (choice != 5) {
        cout << "\n==========================================================\n";
        cout << "     THEATRE REHEARSAL CAST AND PROP MANAGER (CA1)        \n";
        cout << "==========================================================\n";
        cout << "Total Records in Memory : " << currentCount << " / " << MAX_CAPACITY << "\n";
        cout << "Static Session Counter  : " << RehearsalResource::getTotalResourcesCreated() << "\n";
        cout << "----------------------------------------------------------\n";
        cout << "1. Create New Cast / Prop Record\n";
        cout << "2. Display All Records (Pointer Traversal)\n";
        cout << "3. Inspect Record by ID (Friend Function Audit)\n";
        cout << "4. View 2D Stage Rehearsal Slot Matrix\n";
        cout << "5. Exit Application\n";
        cout << "----------------------------------------------------------\n";

        getValidatedInt("Enter your choice (1-5): ", choice, 1, 5);

        if (choice == 1) {
            // CREATE OPERATION
            if (currentCount >= MAX_CAPACITY) {
                cout << "\n[Error] Memory catalog is full (Max 100 records).\n";
                continue;
            }

            cout << "\n--- Create New Rehearsal Resource ---\n";
            int id, type, scene, metric, status, slot;

            // Validate Unique Key
            while (true) {
                getValidatedInt("Enter Unique Resource ID (1001-9999): ", id, 1001, 9999);
                if (isDuplicateKey(catalog, currentCount, id)) {
                    cout << "   [Duplicate Error] Resource ID " << id 
                         << " already exists! Please enter a unique ID.\n";
                } else {
                    break;
                }
            }

            getValidatedInt("Select Type (1 = Cast Member, 2 = Stage Prop): ", type, 1, 2);
            
            string namePrompt = (type == CAST_MEMBER) ? "Enter Actor / Crew Full Name: " : "Enter Stage Prop Name: ";
            string nameStr = getValidatedString(namePrompt, 2, 49);

            string rolePrompt = (type == CAST_MEMBER) ? "Enter Character Role (e.g. Hamlet): " : "Enter Prop Category (e.g. Lighting): ";
            string roleStr = getValidatedString(rolePrompt, 2, 39);

            getValidatedInt("Enter Assigned Scene Number (1-25): ", scene, 1, 25);
            
            string metricPrompt = (type == CAST_MEMBER) ? "Enter Rehearsal Hours (1-200): " : "Enter Prop Quantity (1-200): ";
            getValidatedInt(metricPrompt, metric, 1, 200);

            getValidatedInt("Select Status (1 = Pending, 2 = Ready, 3 = Issue): ", status, 1, 3);
            getValidatedInt("Select Rehearsal Slot (1 = Morning, 2 = Afternoon, 3 = Evening): ", slot, 1, 3);

            // Save data into the array of objects
            catalog[currentCount].setResourceData(id, type, nameStr, roleStr, scene, metric, status);
            currentCount++;
            RehearsalResource::incrementResourceCount();

            // Update 2D matrix for Scenes 1 to 5 (mapped via scene % 5)
            int sceneRow = (scene - 1) % 5;
            stageSlotMatrix[sceneRow][slot - 1]++;

            cout << "\n[Success] Record saved successfully with Unique ID: " << id << "\n";
        }
        else if (choice == 2) {
            // READ ALL OPERATION USING OBJECT POINTER
            if (currentCount == 0) {
                cout << "\n[Notice] No records found in memory. Please create a record first.\n";
                continue;
            }

            cout << "\n------------------------------------------------------------------------------------------\n";
            cout << left << setw(8)  << "ID"
                 << setw(14) << "Type"
                 << setw(22) << "Name / Title"
                 << setw(18) << "Role / Category"
                 << setw(8)  << "Scene"
                 << setw(10) << "Hrs/Qty"
                 << setw(10) << "Status" << endl;
            cout << "------------------------------------------------------------------------------------------\n";

            // Traverse array using a pointer to objects
            RehearsalResource* ptr = catalog;
            for (int i = 0; i < currentCount; i++) {
                ptr->displayRecordRow();
                ptr++;
            }
            cout << "------------------------------------------------------------------------------------------\n";
        }
        else if (choice == 3) {
            // INSPECT SINGLE RECORD USING FRIEND FUNCTION
            if (currentCount == 0) {
                cout << "\n[Notice] No records available to inspect.\n";
                continue;
            }

            int searchId;
            getValidatedInt("Enter Resource ID to inspect (1001-9999): ", searchId, 1001, 9999);
            
            bool found = false;
            RehearsalResource* ptr = catalog;
            for (int i = 0; i < currentCount; i++) {
                if (ptr->getResourceId() == searchId) {
                    printDetailedAuditCard(*ptr); // Call friend function
                    found = true;
                    break;
                }
                ptr++;
            }

            if (!found) {
                cout << "\n[Not Found] No record exists with Resource ID " << searchId << ".\n";
            }
        }
        else if (choice == 4) {
            // DISPLAY 2D MULTIDIMENSIONAL ARRAY
            cout << "\n--- 2D Stage Rehearsal Slot Matrix (Resource Count per Slot) ---\n";
            cout << left << setw(15) << "Stage Group"
                 << setw(15) << "Slot 1 (Morn)"
                 << setw(15) << "Slot 2 (Aft)"
                 << setw(15) << "Slot 3 (Eve)" << endl;
            cout << "------------------------------------------------------------\n";

            for (int r = 0; r < 5; r++) {
                cout << left << setw(15) << ("Stage Zone " + to_string(r + 1))
                     << setw(15) << stageSlotMatrix[r][0]
                     << setw(15) << stageSlotMatrix[r][1]
                     << setw(15) << stageSlotMatrix[r][2] << endl;
            }
            cout << "------------------------------------------------------------\n";
        }
        else if (choice == 5) {
            cout << "\nExiting Theatre Rehearsal Manager. Goodbye!\n";
        }
    }

    return 0;
}
