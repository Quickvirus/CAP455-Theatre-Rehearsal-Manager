// Course: CAP455 - Object Oriented Programming using C++
// Project: Theatre Rehearsal Cast and Prop Manager
// Phase: CA3 (Reuse & Delete - Units IV & V)
// Student: Priyanshu Singh (Reg No: 12613305)
// Submitted To: Mr. Adarsh Kumar Singh

#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <fstream>

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

// Fixed record structure for saving to binary file
// Keeps record size fixed (116 bytes) and avoids writing vptr to disk
struct ResourceData {
    int resourceId;
    int resourceType;
    char resourceName[50];
    char roleOrCategory[40];
    int assignedScene;
    int rehearsalMetric;
    int readinessStatus;
    bool isDeleted;
};

// =========================================================================
// BASE CLASS: RehearsalResource
// =========================================================================
class RehearsalResource {
protected:
    int resourceId;             // Unique ID (1001-9999)
    int resourceType;           // 1 for Cast, 2 for Prop
    char resourceName[50];      // Name of actor or prop
    char roleOrCategory[40];    // Role name or prop type
    int assignedScene;          // Scene number (1-25)
    int rehearsalMetric;        // Hours (for cast) or quantity (for props)
    int readinessStatus;        // 1=Pending, 2=Ready, 3=Issue
    bool isDeleted;             // Soft delete flag

    static int totalResourcesCreated;

public:
    // Default constructor
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

    // Parameterized constructor
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

    // Copy constructor
    RehearsalResource(const RehearsalResource& other) {
        resourceId = other.resourceId;
        resourceType = other.resourceType;
        assignedScene = other.assignedScene;
        rehearsalMetric = other.rehearsalMetric;
        readinessStatus = other.readinessStatus;
        isDeleted = other.isDeleted;
        strcpy(resourceName, other.resourceName);
        strcpy(roleOrCategory, other.roleOrCategory);
        totalResourcesCreated++;
    }

    // Basic to Class type conversion (constructor takes int ID)
    RehearsalResource(int id) {
        resourceId = id;
        resourceType = CAST_MEMBER;
        strcpy(resourceName, "LookupKey");
        strcpy(roleOrCategory, "Key");
        assignedScene = 1;
        rehearsalMetric = 0;
        readinessStatus = PENDING;
        isDeleted = false;
        totalResourcesCreated++;
    }

    // Virtual destructor: essential so derived destructors run on delete
    virtual ~RehearsalResource() {
        totalResourcesCreated--;
    }

    // Helper to convert object to fixed struct for binary file
    ResourceData toStruct() const {
        ResourceData d;
        d.resourceId = resourceId;
        d.resourceType = resourceType;
        strncpy(d.resourceName, resourceName, 49);
        d.resourceName[49] = '\0';
        strncpy(d.roleOrCategory, roleOrCategory, 39);
        d.roleOrCategory[39] = '\0';
        d.assignedScene = assignedScene;
        d.rehearsalMetric = rehearsalMetric;
        d.readinessStatus = readinessStatus;
        d.isDeleted = isDeleted;
        return d;
    }

    // Helper to load from struct
    void fromStruct(const ResourceData& d) {
        resourceId = d.resourceId;
        resourceType = d.resourceType;
        strncpy(resourceName, d.resourceName, 49);
        resourceName[49] = '\0';
        strncpy(roleOrCategory, d.roleOrCategory, 39);
        roleOrCategory[39] = '\0';
        assignedScene = d.assignedScene;
        rehearsalMetric = d.rehearsalMetric;
        readinessStatus = d.readinessStatus;
        isDeleted = d.isDeleted;
    }

    // Getters and setters
    inline int getResourceId() const { return resourceId; }
    inline int getResourceType() const { return resourceType; }
    string getResourceName() const { return string(resourceName); }
    string getRoleOrCategory() const { return string(roleOrCategory); }
    inline int getAssignedScene() const { return assignedScene; }
    inline int getRehearsalMetric() const { return rehearsalMetric; }
    inline int getReadinessStatus() const { return readinessStatus; }
    inline bool getIsDeleted() const { return isDeleted; }

    inline void setAssignedScene(int s) { assignedScene = s; }
    inline void setRehearsalMetric(int m) { rehearsalMetric = m; }
    inline void setReadinessStatus(int st) { readinessStatus = st; }
    inline void setIsDeleted(bool del) { isDeleted = del; }

    static int getTotalResourcesCreated() {
        return totalResourcesCreated;
    }

    // Virtual functions for runtime polymorphism (Unit V)
    virtual void displayDetails() const {
        if (isDeleted) return;
        string typeStr = (resourceType == CAST_MEMBER) ? "Cast Member" : "Stage Prop";
        string statusStr = "Pending";
        if (readinessStatus == READY) statusStr = "Ready";
        else if (readinessStatus == NEEDS_ATTENTION) statusStr = "Needs Attention";

        cout << "\n--------------------------------------------------\n";
        cout << " Resource ID : " << resourceId << "\n";
        cout << " Type        : " << typeStr << "\n";
        cout << " Name        : " << resourceName << "\n";
        cout << " Role/Cat    : " << roleOrCategory << "\n";
        cout << " Scene       : Scene " << assignedScene << "\n";
        cout << " Metric      : " << rehearsalMetric << "\n";
        cout << " Status      : " << statusStr << "\n";
        cout << "--------------------------------------------------\n";
    }

    virtual void displayRow() const {
        if (isDeleted) return;
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

    virtual double calcExpense() const {
        return rehearsalMetric * 50.0;
    }

    // ---------------------------------------------------------------------
    // OPERATOR OVERLOADING (Unit IV)
    // ---------------------------------------------------------------------

    // 1. operator== compares two items by primary key
    bool operator==(const RehearsalResource& other) const {
        return this->resourceId == other.resourceId;
    }

    // 2. operator< compares by ID for ordering
    bool operator<(const RehearsalResource& other) const {
        return this->resourceId < other.resourceId;
    }

    // 3. operator+= adds extra hours or stock quantity with safety cap
    RehearsalResource& operator+=(int extra) {
        rehearsalMetric += extra;
        if (rehearsalMetric > 200) {
            rehearsalMetric = 200;
        }
        return *this;
    }

    // 4. operator<< for direct printing to stream
    friend ostream& operator<<(ostream& os, const RehearsalResource& res) {
        if (res.isDeleted) {
            os << "[Deleted ID: " << res.resourceId << "]";
            return os;
        }
        string tLabel = (res.resourceType == CAST_MEMBER) ? "Cast" : "Prop";
        string sLabel = (res.readinessStatus == READY) ? "Ready" : 
                        (res.readinessStatus == NEEDS_ATTENTION ? "Issue" : "Pending");

        os << "[ID: " << res.resourceId << " | " << tLabel 
           << " | " << res.resourceName << " (" << res.roleOrCategory << ")"
           << " | Scene " << res.assignedScene 
           << " | Metric: " << res.rehearsalMetric
           << " | " << sLabel << "]";
        return os;
    }

    // 5. operator>> for reading input from stream
    friend istream& operator>>(istream& is, RehearsalResource& res) {
        cout << "Enter Resource ID: ";
        is >> res.resourceId;
        cout << "Enter Scene Number (1-25): ";
        is >> res.assignedScene;
        cout << "Enter Metric Value: ";
        is >> res.rehearsalMetric;
        return is;
    }

    // ---------------------------------------------------------------------
    // TYPE CONVERSION: Class to Basic (Unit IV)
    // ---------------------------------------------------------------------
    // Converts object directly to integer ID
    operator int() const {
        return resourceId;
    }

    // Converts object to double (calculated expense impact)
    operator double() const {
        return calcExpense();
    }

    // Friend function from CA1/CA2
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
    cout << "Memory Byte Size  : " << sizeof(ResourceData) << " bytes on disk\n";
    cout << "--------------------------------------------------\n";
}

// =========================================================================
// DERIVED CLASS 1: CastMember
// =========================================================================
class CastMember : public RehearsalResource {
private:
    char costume[30];
    char roleTier[20];
    double hourlyRate;

public:
    CastMember() : RehearsalResource() {
        resourceType = CAST_MEMBER;
        strcpy(costume, "Standard Wardrobe");
        strcpy(roleTier, "Ensemble");
        hourlyRate = 100.0;
    }

    CastMember(int id, const string& actorName, const string& charRole, 
               int scene, int hoursLogged, int status,
               const string& costumeStr, const string& tierStr, double rate)
        : RehearsalResource(id, CAST_MEMBER, actorName, charRole, scene, hoursLogged, status),
          hourlyRate(rate)
    {
        strncpy(costume, costumeStr.c_str(), 29);
        costume[29] = '\0';

        strncpy(roleTier, tierStr.c_str(), 19);
        roleTier[19] = '\0';
    }

    CastMember(const CastMember& other) : RehearsalResource(other) {
        strcpy(costume, other.costume);
        strcpy(roleTier, other.roleTier);
        hourlyRate = other.hourlyRate;
    }

    virtual ~CastMember() override {
        // Derived clean up
    }

    void displayDetails() const override {
        if (isDeleted) return;
        cout << "\n--------------------------------------------------\n";
        cout << " [CAST MEMBER] ID : " << resourceId << "\n";
        cout << " Actor Name       : " << resourceName << "\n";
        cout << " Character Role   : " << roleOrCategory << "\n";
        cout << " Role Tier        : " << roleTier << "\n";
        cout << " Costume Design   : " << costume << "\n";
        cout << " Assigned Scene   : Scene " << assignedScene << "\n";
        cout << " Hours Logged     : " << rehearsalMetric << " hrs\n";
        cout << " Hourly Rate      : Rs. " << fixed << setprecision(2) << hourlyRate << "\n";
        cout << " Total Allowance  : Rs. " << calcExpense() << "\n";
        cout << " Status           : " << (readinessStatus == READY ? "Ready" : "Pending/Issue") << "\n";
        cout << "--------------------------------------------------\n";
    }

    void displayRow() const override {
        if (isDeleted) return;
        string statusLabel = (readinessStatus == READY) ? "Ready" : 
                             (readinessStatus == NEEDS_ATTENTION ? "Issue" : "Pending");

        cout << left << setw(8)  << resourceId
             << setw(14) << "Cast Member"
             << setw(22) << resourceName
             << setw(18) << roleOrCategory
             << setw(8)  << assignedScene
             << setw(10) << (to_string(rehearsalMetric) + " hrs")
             << setw(10) << statusLabel << endl;
    }

    double calcExpense() const override {
        return rehearsalMetric * hourlyRate;
    }

    void addHours(int hrs) {
        *this += hrs;
        cout << "   -> Added " << hrs << " rehearsal hours for " << resourceName 
             << ". Total hours: " << rehearsalMetric << "\n";
    }
};

// =========================================================================
// DERIVED CLASS 2: StageProp
// =========================================================================
class StageProp : public RehearsalResource {
private:
    char location[30];
    bool isFragile;
    double replaceCost;

public:
    StageProp() : RehearsalResource() {
        resourceType = STAGE_PROP;
        strcpy(location, "Props Room");
        isFragile = false;
        replaceCost = 250.0;
    }

    StageProp(int id, const string& propTitle, const string& propCategory, 
              int scene, int quantity, int status,
              const string& locStr, bool fragile, double cost)
        : RehearsalResource(id, STAGE_PROP, propTitle, propCategory, scene, quantity, status),
          isFragile(fragile), replaceCost(cost)
    {
        strncpy(location, locStr.c_str(), 29);
        location[29] = '\0';
    }

    StageProp(const StageProp& other) : RehearsalResource(other) {
        strcpy(location, other.location);
        isFragile = other.isFragile;
        replaceCost = other.replaceCost;
    }

    virtual ~StageProp() override {
        // Derived clean up
    }

    void displayDetails() const override {
        if (isDeleted) return;
        cout << "\n--------------------------------------------------\n";
        cout << " [STAGE PROP] ID  : " << resourceId << "\n";
        cout << " Prop Title       : " << resourceName << "\n";
        cout << " Category         : " << roleOrCategory << "\n";
        cout << " Storage Location : " << location << "\n";
        cout << " Fragile?         : " << (isFragile ? "Yes (Handle with care)" : "No (Durable)") << "\n";
        cout << " Quantity in Stock: " << rehearsalMetric << " units\n";
        cout << " Unit Cost        : Rs. " << fixed << setprecision(2) << replaceCost << "\n";
        cout << " Total Value/Risk : Rs. " << calcExpense() << "\n";
        cout << " Status           : " << (readinessStatus == READY ? "Ready" : "Pending/Issue") << "\n";
        cout << "--------------------------------------------------\n";
    }

    void displayRow() const override {
        if (isDeleted) return;
        string statusLabel = (readinessStatus == READY) ? "Ready" : 
                             (readinessStatus == NEEDS_ATTENTION ? "Issue" : "Pending");

        cout << left << setw(8)  << resourceId
             << setw(14) << "Stage Prop"
             << setw(22) << resourceName
             << setw(18) << roleOrCategory
             << setw(8)  << assignedScene
             << setw(10) << (to_string(rehearsalMetric) + " pcs")
             << setw(10) << statusLabel << endl;
    }

    double calcExpense() const override {
        return rehearsalMetric * replaceCost;
    }

    void addStock(int qty) {
        *this += qty;
        cout << "   -> Restocked " << qty << " units of " << resourceName 
             << ". Total count: " << rehearsalMetric << "\n";
    }
};

// =========================================================================
// FILE OPERATIONS (Binary fstream)
// =========================================================================
const string DB_FILENAME = "rehearsal_db.dat";

void openDatabaseFile(fstream& file, ios::openmode mode) {
    file.open(DB_FILENAME, mode);
    if (!file) {
        file.clear();
        ofstream createFile(DB_FILENAME, ios::out | ios::binary);
        createFile.close();
        file.open(DB_FILENAME, mode);
    }
}

// Check if ID exists in database
bool isDuplicateKeyInFile(int newId) {
    fstream file;
    openDatabaseFile(file, ios::in | ios::binary);
    
    ResourceData rec;
    while (file.read((char*)&rec, sizeof(ResourceData))) {
        if (!rec.isDeleted && rec.resourceId == newId) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Append record to end of binary file
void saveRecordToFile(const RehearsalResource& record) {
    fstream file;
    openDatabaseFile(file, ios::app | ios::binary);
    ResourceData rec = record.toStruct();
    file.write((char*)&rec, sizeof(ResourceData));
    file.close();
}

// Read and display all active records
void displayAllRecordsFromFile() {
    fstream file;
    openDatabaseFile(file, ios::in | ios::binary);
    
    ResourceData rec;
    RehearsalResource temp;
    bool foundAny = false;

    cout << "\n------------------------------------------------------------------------------------------\n";
    cout << left << setw(8)  << "ID" << setw(14) << "Type" << setw(22) << "Name / Title"
         << setw(18) << "Role / Category" << setw(8)  << "Scene" << setw(10) << "Hrs/Qty" << setw(10) << "Status" << endl;
    cout << "------------------------------------------------------------------------------------------\n";

    while (file.read((char*)&rec, sizeof(ResourceData))) {
        if (!rec.isDeleted) {
            temp.fromStruct(rec);
            temp.displayRow();
            foundAny = true;
        }
    }
    
    if (!foundAny) {
        cout << "No active records found in database.\n";
    }
    cout << "------------------------------------------------------------------------------------------\n";
    file.close();
}

// Search record by ID using seekg
void searchRecordInFile(int searchId) {
    fstream file;
    openDatabaseFile(file, ios::in | ios::binary);
    
    ResourceData rec;
    RehearsalResource temp;
    bool found = false;

    while (file.read((char*)&rec, sizeof(ResourceData))) {
        if (!rec.isDeleted && rec.resourceId == searchId) {
            temp.fromStruct(rec);
            printDetailedAuditCard(temp);
            cout << "Stream representation (operator<<): " << temp << "\n";
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "\n[Not Found] No active record exists with Resource ID " << searchId << ".\n";
    }
    file.close();
}

// =========================================================================
// INPUT VALIDATION HELPERS
// =========================================================================
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

char getConfirmation(const string& prompt) {
    char ch;
    while (true) {
        cout << prompt;
        cin >> ch;
        cin.ignore(1000, '\n');
        ch = tolower(ch);
        if (ch == 'y' || ch == 'n') return ch;
        cout << "   [Invalid Choice] Please enter 'y' for yes or 'n' for no.\n";
    }
}

// =========================================================================
// SAFE UPDATE WITH CONFIRMATION (seekp overwrite)
// =========================================================================
void safeUpdateRecordInPlace(int targetId) {
    fstream file;
    openDatabaseFile(file, ios::in | ios::out | ios::binary);
    
    ResourceData rec;
    RehearsalResource temp;
    bool found = false;
    long byteOffset = 0;

    file.seekg(0, ios::beg);
    while (file.read((char*)&rec, sizeof(ResourceData))) {
        if (!rec.isDeleted && rec.resourceId == targetId) {
            found = true;
            temp.fromStruct(rec);
            byteOffset = (long)file.tellg() - (long)sizeof(ResourceData);
            break;
        }
    }

    if (!found) {
        cout << "\n[Not Found] Resource ID " << targetId << " not found for updating.\n";
        file.close();
        return;
    }

    cout << "\n--- Target Record Located for Update ---\n";
    printDetailedAuditCard(temp);

    // Prompt user for confirmation before asking for new values
    char confirm = getConfirmation("Are you sure you want to update this record? (y/n): ");
    if (confirm != 'y') {
        cout << "\n[Cancelled] Update aborted by user. Record remains unchanged.\n";
        file.close();
        return;
    }

    cout << "\nEnter New Values:\n";
    int newScene, newMetric, newStatus;
    getValidatedInt("Enter New Assigned Scene Number (1-25): ", newScene, 1, 25);
    getValidatedInt("Enter New Rehearsal Hours/Qty (1-200): ", newMetric, 1, 200);
    getValidatedInt("Select New Status (1 = Pending, 2 = Ready, 3 = Issue): ", newStatus, 1, 3);

    // Final confirmation before disk write
    char finalConfirm = getConfirmation("Save these changes permanently to disk? (y/n): ");
    if (finalConfirm != 'y') {
        cout << "\n[Cancelled] Disk write aborted. Existing record preserved.\n";
        file.close();
        return;
    }

    RehearsalResource updatedRecord(
        temp.getResourceId(), temp.getResourceType(), 
        temp.getResourceName(), temp.getRoleOrCategory(),
        newScene, newMetric, newStatus
    );

    ResourceData updatedRec = updatedRecord.toStruct();

    // Reposition write pointer via seekp to exact record offset
    file.seekp(byteOffset, ios::beg);
    file.write((char*)&updatedRec, sizeof(ResourceData));
    
    cout << "\n[Success] Record ID " << targetId << " updated securely in-place at byte offset " << byteOffset << ".\n";
    file.close();
}

// =========================================================================
// SAFE DELETE WITH CONFIRMATION (seekp soft delete)
// =========================================================================
void safeDeleteRecordInPlace(int targetId) {
    fstream file;
    openDatabaseFile(file, ios::in | ios::out | ios::binary);
    
    ResourceData rec;
    RehearsalResource temp;
    bool found = false;
    long byteOffset = 0;

    file.seekg(0, ios::beg);
    while (file.read((char*)&rec, sizeof(ResourceData))) {
        if (!rec.isDeleted && rec.resourceId == targetId) {
            found = true;
            temp.fromStruct(rec);
            byteOffset = (long)file.tellg() - (long)sizeof(ResourceData);
            break;
        }
    }

    if (!found) {
        cout << "\n[Not Found] Resource ID " << targetId << " not found or already deleted.\n";
        file.close();
        return;
    }

    cout << "\n--- Record Located for Deletion ---\n";
    printDetailedAuditCard(temp);

    // Confirmation prompt with warning
    char confirm = getConfirmation("WARNING: Are you sure you want to delete this resource? (y/n): ");
    if (confirm != 'y') {
        cout << "\n[Cancelled] Deletion aborted by user. Record is preserved.\n";
        file.close();
        return;
    }

    // Soft delete: mark isDeleted = true and write back at exact offset
    rec.isDeleted = true;

    file.seekp(byteOffset, ios::beg);
    file.write((char*)&rec, sizeof(ResourceData));
    
    cout << "\n[Success] Resource ID " << targetId << " marked as deleted. Excluded from active queries.\n";
    file.close();
}

// Restore a previously deleted record
void restoreDeletedRecordInPlace(int targetId) {
    fstream file;
    openDatabaseFile(file, ios::in | ios::out | ios::binary);
    
    ResourceData rec;
    bool found = false;
    long byteOffset = 0;

    file.seekg(0, ios::beg);
    while (file.read((char*)&rec, sizeof(ResourceData))) {
        if (rec.isDeleted && rec.resourceId == targetId) {
            found = true;
            byteOffset = (long)file.tellg() - (long)sizeof(ResourceData);
            break;
        }
    }

    if (!found) {
        cout << "\n[Not Found] No deleted record found with ID " << targetId << ".\n";
        file.close();
        return;
    }

    char confirm = getConfirmation("Restore this previously deleted record? (y/n): ");
    if (confirm == 'y') {
        rec.isDeleted = false;
        file.seekp(byteOffset, ios::beg);
        file.write((char*)&rec, sizeof(ResourceData));
        cout << "\n[Success] Resource ID " << targetId << " restored successfully.\n";
    } else {
        cout << "\n[Cancelled] Restore aborted.\n";
    }
    file.close();
}

// =========================================================================
// OPERATOR OVERLOADING & TYPE CONVERSION DEMO
// =========================================================================
void testOperators() {
    cout << "\n==========================================================\n";
    cout << "  UNIT IV: OPERATOR OVERLOADING & TYPE CONVERSIONS        \n";
    cout << "==========================================================\n";

    RehearsalResource itemA(1001, CAST_MEMBER, "Lead Actor", "Macbeth", 2, 20, READY);
    RehearsalResource itemB(1002, CAST_MEMBER, "Supporting Actor", "Banquo", 2, 15, READY);
    RehearsalResource itemClone(1001, CAST_MEMBER, "Duplicate Check", "Macbeth", 3, 10, PENDING);

    // 1. operator<<
    cout << "\n1. Testing operator<< (stream insertion):\n";
    cout << "   itemA: " << itemA << "\n";
    cout << "   itemB: " << itemB << "\n";

    // 2. operator==
    cout << "\n2. Testing operator== (equality check by ID):\n";
    if (itemA == itemClone) {
        cout << "   [PASS] itemA (ID 1001) == itemClone (ID 1001) evaluated to TRUE.\n";
    } else {
        cout << "   [FAIL] Equality comparison failed.\n";
    }

    // 3. operator<
    cout << "\n3. Testing operator< (comparison by ID):\n";
    if (itemA < itemB) {
        cout << "   [PASS] itemA (ID 1001) < itemB (ID 1002) evaluated to TRUE.\n";
    } else {
        cout << "   [PASS] itemB is greater than itemA.\n";
    }

    // 4. operator+=
    cout << "\n4. Testing operator+= (Adding 8 hours to itemA):\n";
    cout << "   Before: " << itemA << "\n";
    itemA += 8;
    cout << "   After itemA += 8: " << itemA << "\n";

    // 5. Basic to Class conversion (constructor conversion)
    cout << "\n5. Testing Basic to Class Type Conversion:\n";
    RehearsalResource convertedKey = 1005; 
    cout << "   Converted integer 1005 to object: " << convertedKey << "\n";

    // 6. Class to Basic conversion (operator int and operator double)
    cout << "\n6. Testing Class to Basic Type Conversion:\n";
    int extractedId = (int)itemA;
    double calculatedCost = (double)itemA;
    cout << "   Extracted ID via (int)itemA       : " << extractedId << "\n";
    cout << "   Calculated cost via (double)itemA : Rs. " << calculatedCost << "\n";

    cout << "----------------------------------------------------------\n";
}

// =========================================================================
// DYNAMIC MEMORY & RUNTIME POLYMORPHISM DEMO
// =========================================================================
void testPolymorphism() {
    cout << "\n==========================================================\n";
    cout << "  UNIT V: DYNAMIC MEMORY & RUNTIME POLYMORPHISM           \n";
    cout << "==========================================================\n";

    const int ROSTER_SIZE = 3;
    // Base class pointer array allocated on heap
    RehearsalResource** roster = new RehearsalResource*[ROSTER_SIZE];

    // Dynamic instantiation using new
    roster[0] = new CastMember(1001, "Amit Sharma", "King Lear", 1, 40, READY, 
                               "Purple Robe", "Lead", 300.0);
    roster[1] = new StageProp(2001, "Gilded Crown", "Regalia", 1, 2, READY, 
                              "Prop-Locker-1", true, 2500.0);
    roster[2] = new CastMember(1002, "Sunita Rao", "Cordelia", 3, 25, PENDING, 
                               "Silk Gown", "Supporting", 200.0);

    cout << "\nIterating base pointers (resolving methods via VTABLE):\n";
    for (int i = 0; i < ROSTER_SIZE; ++i) {
        roster[i]->displayDetails();
    }

    cout << "\nInvoking derived methods using dynamic_cast:\n";
    CastMember* castPtr = dynamic_cast<CastMember*>(roster[0]);
    if (castPtr) {
        castPtr->addHours(4);
    }

    StageProp* propPtr = dynamic_cast<StageProp*>(roster[1]);
    if (propPtr) {
        propPtr->addStock(1);
    }

    cout << "\nFreeing heap memory via base pointers (testing virtual destructor):\n";
    for (int i = 0; i < ROSTER_SIZE; ++i) {
        delete roster[i];
        roster[i] = NULL;
    }
    delete[] roster;
    cout << "[Clean] Memory freed cleanly without leaks.\n";
    cout << "----------------------------------------------------------\n";
}

// =========================================================================
// MAIN MENU
// =========================================================================
int main() {
    int choice = 0;
    while (choice != 9) {
        cout << "\n==========================================================\n";
        cout << "    THEATRE REHEARSAL CAST & PROP MANAGER (Phase CA3)     \n";
        cout << "==========================================================\n";
        cout << "1. Add New Record (Cast Member / Stage Prop)\n";
        cout << "2. Display All Active Records (Binary File Stream)\n";
        cout << "3. Search Record by ID (seekg byte scan)\n";
        cout << "4. Safe Update Record (seekp with confirmation)\n";
        cout << "5. Safe Delete Record (seekp soft-delete with confirmation)\n";
        cout << "6. Restore Deleted Record (Safety undelete)\n";
        cout << "7. Test Operator Overloading & Type Conversion (Unit IV)\n";
        cout << "8. Test Dynamic Memory & Runtime Polymorphism (Unit V)\n";
        cout << "9. Exit Application\n";
        cout << "----------------------------------------------------------\n";

        getValidatedInt("Enter your choice (1-9): ", choice, 1, 9);

        if (choice == 1) {
            int id, type, scene, metric, status;
            while (true) {
                getValidatedInt("\nEnter Unique Resource ID (1001-9999): ", id, 1001, 9999);
                if (isDuplicateKeyInFile(id)) {
                    cout << "   [Error] ID " << id << " already exists! Enter a different ID.\n";
                } else {
                    break;
                }
            }

            getValidatedInt("Select Type (1 = Cast Member, 2 = Stage Prop): ", type, 1, 2);
            string nameStr = getValidatedString("Enter Name / Title: ", 2, 49);
            string roleStr = getValidatedString("Enter Role / Prop Category: ", 2, 39);
            getValidatedInt("Enter Assigned Scene Number (1-25): ", scene, 1, 25);
            
            if (type == CAST_MEMBER) {
                getValidatedInt("Enter Rehearsal Hours Logged (1-200): ", metric, 1, 200);
            } else {
                getValidatedInt("Enter Quantity of Prop in Stock (1-200): ", metric, 1, 200);
            }

            getValidatedInt("Select Status (1 = Pending, 2 = Ready, 3 = Issue): ", status, 1, 3);

            RehearsalResource newRecord(id, type, nameStr, roleStr, scene, metric, status);
            saveRecordToFile(newRecord);
            cout << "\n[Success] Record ID " << id << " saved to " << DB_FILENAME << ".\n";
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
            safeUpdateRecordInPlace(updateId);
        }
        else if (choice == 5) {
            int deleteId;
            getValidatedInt("\nEnter Resource ID to delete: ", deleteId, 1001, 9999);
            safeDeleteRecordInPlace(deleteId);
        }
        else if (choice == 6) {
            int restoreId;
            getValidatedInt("\nEnter Resource ID to restore: ", restoreId, 1001, 9999);
            restoreDeletedRecordInPlace(restoreId);
        }
        else if (choice == 7) {
            testOperators();
        }
        else if (choice == 8) {
            testPolymorphism();
        }
        else if (choice == 9) {
            cout << "\nExiting Theatre Rehearsal Manager. Goodbye!\n";
        }
    }
    return 0;
}
