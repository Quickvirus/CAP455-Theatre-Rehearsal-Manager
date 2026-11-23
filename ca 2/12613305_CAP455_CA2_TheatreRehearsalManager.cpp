// Course: CAP455 - Object Oriented Programming using C++
// Phase: CA2 (Persist & Search - Unit III)
// Project: Theatre Rehearsal Cast and Prop Manager
// Student: Priyanshu Singh (Reg No: 12613305)

#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <fstream> // Required for file operations (Unit III)

using namespace std;

enum ResourceType {
    CAST_MEMBER = 1,
    STAGE_PROP = 2
};

enum ReadinessStatus {
    PENDING = 1,
    READY = 2,
    NEEDS_ATTENTION = 3
};

class RehearsalResource {
private:
    int resourceId;             
    int resourceType;           
    char resourceName[50];      // Crucial: Fixed size char array ensures sizeof(RehearsalResource) is constant
    char roleOrCategory[40];    // so seekg() and seekp() calculate exact byte offsets correctly.
    int assignedScene;          
    int rehearsalMetric;        
    int readinessStatus;        
    bool isDeleted;             // Soft delete flag for future CA3 deletion

    static int totalResourcesCreated;

public:
    // 1. DEFAULT CONSTRUCTOR (Unit III)
    RehearsalResource() {
        resourceId = 0;
        resourceType = CAST_MEMBER;
        strcpy(resourceName, "Not Set");
        strcpy(roleOrCategory, "None");
        assignedScene = 1;
        rehearsalMetric = 0;
        readinessStatus = PENDING;
        isDeleted = false;
        totalResourcesCreated++;
    }

    // 2. PARAMETERIZED CONSTRUCTOR (Unit III)
    // Uses initializer list for primitive types, then handles strings inside body
    RehearsalResource(int id, int type, const string& nameStr, const string& roleStr, 
                      int scene, int metric, int status) 
        : resourceId(id), resourceType(type), assignedScene(scene), 
          rehearsalMetric(metric), readinessStatus(status), isDeleted(false) 
    {
        strncpy(resourceName, nameStr.c_str(), 49);
        resourceName[49] = '\0';

        strncpy(roleOrCategory, roleStr.c_str(), 39);
        roleOrCategory[39] = '\0';
        
        totalResourcesCreated++;
    }

    // 3. COPY CONSTRUCTOR (Unit III)
    // Creates a new object as a safe clone of an existing one
    RehearsalResource(const RehearsalResource& other) {
        this->resourceId = other.resourceId;
        this->resourceType = other.resourceType;
        this->assignedScene = other.assignedScene;
        this->rehearsalMetric = other.rehearsalMetric;
        this->readinessStatus = other.readinessStatus;
        this->isDeleted = other.isDeleted;
        strcpy(this->resourceName, other.resourceName);
        strcpy(this->roleOrCategory, other.roleOrCategory);
        totalResourcesCreated++;
    }

    // 4. DESTRUCTOR (Unit III)
    // Cleans up. Here it simply decrements the shared counter when objects go out of scope.
    ~RehearsalResource() {
        totalResourcesCreated--;
    }

    inline int getResourceId() const { return resourceId; }
    inline int getResourceType() const { return resourceType; }
    string getResourceName() const { return string(resourceName); }
    string getRoleOrCategory() const { return string(roleOrCategory); }
    inline bool getIsDeleted() const { return isDeleted; }

    static int getTotalResourcesCreated() {
        return totalResourcesCreated;
    }

    void displayRecordRow() const {
        if (isDeleted) return; // Skip showing deleted records

        string typeLabel = (resourceType == CAST_MEMBER) ? "Cast Member" : "Stage Prop";
        string statusLabel = "Pending";
        if (readinessStatus == READY) statusLabel = "Ready";
        else if (readinessStatus == NEEDS_ATTENTION) statusLabel = "Issue";

        cout << left << setw(8)  << resourceId
             << setw(14) << typeLabel
             << setw(22) << resourceName
             << setw(18) << roleOrCategory
             << setw(8)  << assignedScene
             << setw(10) << rehearsalMetric
             << setw(10) << statusLabel << endl;
    }

    friend void printDetailedAuditCard(const RehearsalResource& res);
};

int RehearsalResource::totalResourcesCreated = 0;

void printDetailedAuditCard(const RehearsalResource& res) {
    if (res.isDeleted) return;
    cout << "\n--------------------------------------------------\n";
    cout << "       DETAILED RESOURCE AUDIT (FRIEND ACCESS)    \n";
    cout << "--------------------------------------------------\n";
    cout << "Resource ID (Key) : " << res.resourceId << "\n";
    cout << "Category Type     : " << (res.resourceType == CAST_MEMBER ? "Cast Member" : "Stage Prop") << "\n";
    cout << "Name / Title      : " << res.resourceName << "\n";
    cout << "Role / Prop Type  : " << res.roleOrCategory << "\n";
    cout << "Assigned Scene    : Scene " << res.assignedScene << "\n";
    cout << "Readiness Status  : " << (res.readinessStatus == READY ? "Ready" : "Pending/Issue") << "\n";
    cout << "Memory Byte Size  : " << sizeof(RehearsalResource) << " bytes\n";
    cout << "--------------------------------------------------\n";
}

// -------------------------------------------------------------------------
// FILE OPERATIONS (CA2 - Unit III: Persistent Binary File Handling)
// -------------------------------------------------------------------------
const string DB_FILENAME = "rehearsal_db.dat";

// Helper to open file stream safely
void openDatabaseFile(fstream& file, ios::openmode mode) {
    file.open(DB_FILENAME, mode);
    if (!file) {
        // If file doesn't exist, create it by opening in out mode first
        file.clear();
        ofstream createFile(DB_FILENAME, ios::out | ios::binary);
        createFile.close();
        file.open(DB_FILENAME, mode);
    }
}

// Check if an ID already exists in the binary file
bool isDuplicateKeyInFile(int newId) {
    fstream file;
    openDatabaseFile(file, ios::in | ios::binary);
    
    RehearsalResource tempRecord;
    // Sequential access read loop
    while (file.read((char*)&tempRecord, sizeof(RehearsalResource))) {
        if (!tempRecord.getIsDeleted() && tempRecord.getResourceId() == newId) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Write a new record to the end of the file
void saveRecordToFile(const RehearsalResource& record) {
    fstream file;
    openDatabaseFile(file, ios::app | ios::binary);
    file.write((char*)&record, sizeof(RehearsalResource));
    file.close();
}

// Read and display all active records
void displayAllRecordsFromFile() {
    fstream file;
    openDatabaseFile(file, ios::in | ios::binary);
    
    RehearsalResource tempRecord;
    bool foundAny = false;

    cout << "\n------------------------------------------------------------------------------------------\n";
    cout << left << setw(8)  << "ID" << setw(14) << "Type" << setw(22) << "Name / Title"
         << setw(18) << "Role / Category" << setw(8)  << "Scene" << setw(10) << "Hrs/Qty" << setw(10) << "Status" << endl;
    cout << "------------------------------------------------------------------------------------------\n";

    while (file.read((char*)&tempRecord, sizeof(RehearsalResource))) {
        if (!tempRecord.getIsDeleted()) {
            tempRecord.displayRecordRow();
            foundAny = true;
        }
    }
    
    if (!foundAny) {
        cout << "No active records found in database.\n";
    }
    cout << "------------------------------------------------------------------------------------------\n";
    file.close();
}

// Search for a specific record using sequential scan, then print it
void searchRecordInFile(int searchId) {
    fstream file;
    openDatabaseFile(file, ios::in | ios::binary);
    
    RehearsalResource tempRecord;
    bool found = false;

    // We scan sequentially to find it
    while (file.read((char*)&tempRecord, sizeof(RehearsalResource))) {
        if (!tempRecord.getIsDeleted() && tempRecord.getResourceId() == searchId) {
            printDetailedAuditCard(tempRecord);
            found = true;
            break;
        }
    }
    if (!found) cout << "\n[Not Found] No record exists with Resource ID " << searchId << ".\n";
    file.close();
}

// Helper input functions
void getValidatedInt(const string& prompt, int& valueOut, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        if (cin >> valueOut && valueOut >= minVal && valueOut <= maxVal) {
            cin.ignore(1000, '\n'); 
            break;
        } else {
            cout << "   [Invalid Input] Please enter a number between " << minVal << " and " << maxVal << ".\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

string getValidatedString(const string& prompt, size_t minLen, size_t maxLen) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        size_t firstChar = input.find_first_not_of(" \t");
        if (firstChar == string::npos) {
            cout << "   [Invalid Input] Text cannot be empty.\n";
            continue;
        }
        input.erase(0, firstChar);
        if (input.length() >= minLen && input.length() <= maxLen) return input;
        cout << "   [Invalid Input] Length must be between " << minLen << " and " << maxLen << " characters.\n";
    }
}

// In-Place Update using seekg (to find) and seekp (to overwrite)
void updateRecordInPlace(int targetId) {
    fstream file;
    openDatabaseFile(file, ios::in | ios::out | ios::binary);
    
    RehearsalResource tempRecord;
    bool found = false;
    long byteOffset = 0;

    // Use seekg to read sequentially until we find the record
    file.seekg(0, ios::beg);
    while (file.read((char*)&tempRecord, sizeof(RehearsalResource))) {
        if (!tempRecord.getIsDeleted() && tempRecord.getResourceId() == targetId) {
            found = true;
            // The read pointer has now moved past the object, so we calculate the start offset
            byteOffset = (long)file.tellg() - (long)sizeof(RehearsalResource);
            break;
        }
    }

    if (!found) {
        cout << "\n[Not Found] Resource ID " << targetId << " not found for updating.\n";
        file.close();
        return;
    }

    cout << "\n--- Updating Record ID " << targetId << " ---\n";
    
    int newScene, newMetric, newStatus;
    getValidatedInt("Enter New Assigned Scene Number (1-25): ", newScene, 1, 25);
    getValidatedInt("Enter New Rehearsal Hours/Qty (1-200): ", newMetric, 1, 200);
    getValidatedInt("Select New Status (1 = Pending, 2 = Ready, 3 = Issue): ", newStatus, 1, 3);

    // Call Parameterized Constructor to create the updated object locally
    RehearsalResource updatedRecord(
        tempRecord.getResourceId(), tempRecord.getResourceType(), 
        tempRecord.getResourceName(), tempRecord.getRoleOrCategory(),
        newScene, newMetric, newStatus
    );

    // Use seekp() to move the write pointer to the exact byte offset of the old record
    file.seekp(byteOffset, ios::beg);
    // Overwrite the old record with the new updated object
    file.write((char*)&updatedRecord, sizeof(RehearsalResource));
    
    cout << "\n[Success] Record ID " << targetId << " updated securely in-place using seekp().\n";
    file.close();
}


int main() {
    int choice = 0;
    while (choice != 5) {
        cout << "\n==========================================================\n";
        cout << "     THEATRE REHEARSAL MANAGER (CA2 - Binary File I/O)    \n";
        cout << "==========================================================\n";
        cout << "1. Add New Cast / Prop Record (Appends to binary file)\n";
        cout << "2. Display All Records (Reads sequentially from file)\n";
        cout << "3. Search Record by ID (Friend Audit via seekg)\n";
        cout << "4. Update Record by ID (In-place overwrite via seekp)\n";
        cout << "5. Exit Application\n";
        cout << "----------------------------------------------------------\n";

        getValidatedInt("Enter your choice (1-5): ", choice, 1, 5);

        if (choice == 1) {
            int id, type, scene, metric, status;
            while (true) {
                getValidatedInt("\nEnter Unique Resource ID (1001-9999): ", id, 1001, 9999);
                if (isDuplicateKeyInFile(id)) cout << "   [Error] ID exists in database!\n";
                else break;
            }
            getValidatedInt("Select Type (1 = Cast, 2 = Prop): ", type, 1, 2);
            string nameStr = getValidatedString("Enter Name / Title: ", 2, 49);
            string roleStr = getValidatedString("Enter Role / Category: ", 2, 39);
            getValidatedInt("Enter Assigned Scene Number (1-25): ", scene, 1, 25);
            getValidatedInt("Enter Hours / Qty (1-200): ", metric, 1, 200);
            getValidatedInt("Select Status (1 = Pending, 2 = Ready, 3 = Issue): ", status, 1, 3);

            // Using Parameterized Constructor
            RehearsalResource newRecord(id, type, nameStr, roleStr, scene, metric, status);
            saveRecordToFile(newRecord);
            cout << "\n[Success] Record saved permanently to rehearsal_db.dat\n";
        }
        else if (choice == 2) {
            displayAllRecordsFromFile();
        }
        else if (choice == 3) {
            int searchId;
            getValidatedInt("\nEnter Resource ID to search: ", searchId, 1001, 9999);
            searchRecordInFile(searchId);
        }
        else if (choice == 4) {
            int updateId;
            getValidatedInt("\nEnter Resource ID to update: ", updateId, 1001, 9999);
            updateRecordInPlace(updateId);
        }
    }
    return 0;
}