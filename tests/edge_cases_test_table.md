# Phase CA3: Edge-Case Test Execution Table

**Course:** CAP455 - Object-Oriented Programming using C++  
**Project:** Theatre Rehearsal Cast and Prop Manager  
**Student:** Priyanshu Singh (Registration No: 12613305)  
**Faculty:** Mr. Adarsh Kumar Singh  
**Phase:** CA3 (Reuse & Delete - Units IV & V)  

---

| Test ID | Test Scenario | Category | Input Values | Expected Behavior | Actual System Result | Verdict |
|---|---|---|---|---|---|---|
| **TC-01** | Search Non-Existent ID | Negative Test | Search ID = `9999` (Active database holds `1001-2002`) | Gracefully reports `[Not Found]` without infinite loop or application crash. | Prompted error message cleanly: `[Not Found] No active record exists with Resource ID 9999.` | **PASS** |
| **TC-02** | Duplicate Key Prevention on Insert | Constraint Validation | Attempt to add new record with ID = `1001` (already exists) | Checks binary database via sequential `seekg` scan and rejects duplicate key. | Rejection notice triggered: `[Error] ID 1001 already exists in database! Please choose another.` | **PASS** |
| **TC-03** | Boundary Value Lower Limit | Boundary Test | ID = `1000` (below range) then `1001`; Scene = `0` then `1` | Values below threshold rejected; lower boundary accepted. | Numeric validator loops until value satisfies `minVal <= val <= maxVal`. | **PASS** |
| **TC-04** | Boundary Value Upper Limit | Boundary Test | ID = `10000` (above range) then `9999`; Scene = `26` then `25`; Metric = `201` then `200` | Values exceeding threshold rejected; upper boundary accepted. | Gracefully displays bounds message `[Invalid Input] Please enter a number between min and max.` | **PASS** |
| **TC-05** | Safe Update User Cancellation | Edge Case | Target ID = `1001`, at confirmation prompt enters `'n'` | Aborts update workflow immediately; disk data remains intact at original values. | System outputs `[Cancelled] Update aborted by user. Record remains unchanged.` Byte offset untouched. | **PASS** |
| **TC-06** | Safe Delete User Cancellation | Edge Case | Target ID = `1001`, at warning prompt enters `'n'` | Aborts deletion; record stays active with `isDeleted = false`. | Displays `[Cancelled] Deletion aborted by user. Record is preserved.` Record visible in active queries. | **PASS** |
| **TC-07** | Querying Soft-Deleted Record | Functional Test | Attempt to search or update ID `2001` after confirmed soft delete | System filters out records having `isDeleted == true`. | Returns `[Not Found] No active record exists with Resource ID 2001.` Omitted from roster table. | **PASS** |
| **TC-08** | Arithmetic Assignment Operator (`+=`) Boundary Cap | Boundary Test | Cast member with 195 hours given `+15` hours via overloaded `+=` operator | Hours incremented, capped at maximum ceiling (`200`) without buffer overflow. | Updated to `200` hours ceiling; returns reference `*this`. | **PASS** |
| **TC-09** | Empty String & Whitespace Handling | Validation Test | Actor Name or Prop Name entered as empty string or spaces `"     "` | Strips leading whitespace; rejects blank entries. | Displays `[Invalid Input] Text cannot be empty.` Re-prompts until valid character length provided. | **PASS** |
| **TC-10** | Dynamic Polymorphic Memory Management | Memory Test | Dynamic base pointer array `new RehearsalResource*[3]`; heap delete | VTABLE dynamically resolves overridden `displayDetails()`; `delete` invokes virtual destructor cleanly. | Derived cards render correctly; `virtual ~RehearsalResource()` frees heap without leak. | **PASS** |
