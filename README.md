# Theatre Rehearsal Cast and Prop Manager
**MCA · Course Code: CAP455 (Object-Oriented Programming using C++) · Session 2026–27**

- **Student Name:** Priyanshu Singh  
- **Registration No:** 12613305  
- **Submitted To:** Mr. Adarsh Kumar Singh  
- **GitHub Repository:** https://github.com/Quickvirus/CAP455-Theatre-Rehearsal-Manager  
- **Current Submission Phase:** Phase CA3 (Units IV & V: Reuse & Delete)  

---

## 1. Project Overview & Problem Statement
During theatrical productions and drama rehearsals, stage management teams coordinate dozens of actors, understudies, stagehands, and critical physical stage props across multiple scenes. Traditional manual record-keeping leads to common failure modes such as duplicate resource IDs, lost backstage prop allocations, and double-booked rehearsal time slots.

The **Theatre Rehearsal Cast and Prop Manager** is an enterprise-grade console application written in standard C++. It provides persistent binary database storage, strict input validation, in-place binary seek operations, single inheritance hierarchies, operator overloading, and runtime polymorphism.

---

## 2. Roadmap & Implementation by Phase

### Phase CA1 (Units I & II: Design & Create)
- Core `RehearsalResource` entity class.
- Member access control (`private`, `public`, `protected`), `this` pointer usage.
- Inline functions for performance-critical field access.
- `friend` function (`printDetailedAuditCard`) for privileged resource inspection.
- Fixed 2D memory arrays and pointer traversal.
- Input validation and duplicate key prevention.

### Phase CA2 (Unit III: Persist & Search)
- Default, Parameterized, and Copy Constructors; class Destructor.
- Persistent fixed-size binary storage engine (`rehearsal_db.dat`).
- Sequential search and record inspection via `seekg()`.
- Direct byte-offset in-place record modification via `seekp()`.
- Data persistence guaranteed across application restarts.

### Phase CA3 (Units IV & V: Reuse & Delete — Current Submission)
- **Inheritance Hierarchy:** Base class `RehearsalResource` with two derived classes: `CastMember` and `StageProp`.
- **Operator Overloading:** Overloaded `<<` (stream output), `>>` (stream input), `==` (key equality), `<` (ordering/sorting), and `+=` (arithmetic logging of hours/stock with upper bound capping).
- **Type Conversions:**
  - *Basic to Class:* Conversion constructor `RehearsalResource(int id)` allows converting an integer ID directly into a lookup key object.
  - *Class to Basic:* Conversion operator `operator int()` extracts the resource ID; `operator double()` calculates dynamic financial impact/rehearsal stipend.
- **Dynamic Memory & Polymorphism:**
  - Dynamic heap allocation of base pointer arrays (`RehearsalResource**`).
  - Runtime late binding via virtual method table (`VTABLE`) for `displayDetails()`, `displayRow()`, and `calculateExpenseImpact()`.
  - Safe polymorphic cleanup using a **`virtual ~RehearsalResource()`** destructor to prevent memory leaks when deleting derived objects through base class pointers.
- **Safe Update & Delete Operations:**
  - Locates target records via `seekg()` and calculates exact byte offset.
  - Gated behind interactive user confirmation prompts (`'y'`/`'n'`).
  - Safe in-place record update via `seekp(byteOffset, ios::beg)`.
  - Safe soft-delete marking `isDeleted = true` via `seekp()` in-place overwrite.
  - Restore/undelete functionality for accidental deletions.

---

## 3. Repository Organization

```text
CPP Project/
├── ca 1/
│   ├── 12613305_CAP455_CA1_TheatreRehearsalManager.cpp    # CA1 Source Code (Arrays, Pointers, Inline, Friend)
│   ├── ca1_uml_diagram.png                                # CA1 UML Class Diagram
│   └── ca1_flowchart.png                                  # CA1 Execution Flowchart
├── ca 2/
│   ├── 12613305_CAP455_CA2_TheatreRehearsalManager.cpp    # CA2 Source Code (Constructors, Binary File Persistence)
│   ├── ca2_uml_diagram.png                                # CA2 UML Class Diagram
│   └── ca2_flowchart.png                                  # CA2 Binary File Operations Flowchart
├── ca 3/
│   ├── 12613305_CAP455_CA3_TheatreRehearsalManager.cpp    # CA3 Source Code (Inheritance, Polymorphism, Safe Delete)
│   ├── ca3_uml_diagram.png                                # CA3 Inheritance & Architecture Diagram
│   ├── ca3_flowchart.png                                  # CA3 Safe Update/Delete Execution Flowchart
│   └── rehearsal_db.dat                                   # Binary database engine file (116 bytes/record)
├── src/
│   └── 12613305_CAP455_CA3_TheatreRehearsalManager.cpp    # Latest CA3 C++ Source Code
├── data/
│   └── rehearsal_db.dat                                   # Persistent binary database file
├── tests/
│   ├── edge_cases_test_table.md                           # 10-point comprehensive edge-case test matrix
│   └── test_run_log.txt                                   # Terminal transcript capturing full test execution
└── README.md                                              # Project build, run, and architecture guide
```

---

## 4. Build & Run Instructions

The code is strictly written in standard C++ and requires no external third-party libraries. It compiles cleanly on any standard C++ compiler (GCC, MinGW, Clang, Microsoft Visual C++, or Dev-C++ 5.11).

### Compiling via GCC / MinGW (Command Line)
```bash
g++ -std=c++11 -Wall -O2 src/12613305_CAP455_CA3_TheatreRehearsalManager.cpp -o TheatreManager.exe
./TheatreManager.exe
```

### Compiling via Dev-C++ 5.11
1. Open Dev-C++.
2. Go to **File -> Open** and select `src/12613305_CAP455_CA3_TheatreRehearsalManager.cpp`.
3. Go to **Execute -> Compile & Run** (or press `F11`).

### Compiling via Microsoft Visual Studio
1. Open Visual Studio Command Prompt.
2. Run:
   ```cmd
   cl /EHsc src\12613305_CAP455_CA3_TheatreRehearsalManager.cpp /Fe:TheatreManager.exe
   TheatreManager.exe
   ```

---

## 5. Operational Verification & Demonstration Workflow

To verify the core capabilities implemented in Phase CA3, execute the following workflow:
- **Menu Option 2 (Display Active):** Verifies sequential binary file reading via `seekg()`, loading persistent records from `rehearsal_db.dat`.
- **Menu Option 4 (Safe Update):** Demonstrates byte-offset location, two-step confirmation gating, and in-place overwriting via `seekp(byteOffset, ios::beg)`. Entering `'n'` cancels safely without file changes.
- **Menu Option 5 (Safe Delete):** Demonstrates soft deletion by setting `isDeleted = true` at the exact record byte offset following user confirmation. Subsequent Option 2 executions verify the record is omitted.
- **Menu Option 7 (Operator Overloading & Type Conversion):** Demonstrates overloaded stream insertion `<<`, relational `==` and `<`, arithmetic assignment `+=`, and explicit/implicit type conversions (`int` and `double`).
- **Menu Option 8 (Runtime Polymorphism):** Demonstrates dynamic allocation via `new`, VTABLE late binding of overridden `displayDetails()`, and leak-free memory reclamation via `virtual ~RehearsalResource()`.
