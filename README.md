# Contact Book BST Assignment

This repository contains my solution for the first homework (`DZ1`) in the *Algorithms and Data Structures 2* course . The task is to model a contact book for a messaging application by building a binary search tree (BST) out of the provided directory files and then extend that foundation with additional communication history features.

## Repository Layout

- `asp2dz1p1.cpp` – console application for **Task 1** (binary search tree contact book). Handles loading contacts, balanced tree creation, searching, editing, inserting, deleting, and printing contacts, plus controlled cleanup.
- `asp2dz1p2.cpp` – console application for **Task 2**. Builds on Task 1 and adds message history with a size limit and share-history heuristics (transposition / move-to-front) driven by assignment parameters.
- `Contacts20.txt`, `Contacts100.txt` – sample input directories sorted lexicographically by last name, then first name. Each line contains `LastName,FirstName,Phone`.
- `13S112ASP2_DZ1_2425.pdf` – official assignment text used to guide the implementation.

## Task 1 Highlights (`asp2dz1p1.cpp`)

The first program satisfies the assignment requirements for implementing the contact book as a BST of minimal height built from a sorted list:

- **Loader** – reads contacts from a CSV-like text file, converts phone numbers to `unsigned long long`, and removes duplicated phone numbers while keeping the first occurrence.
- **Custom stack** – iterative stack template used to avoid recursion while building and traversing the tree.
- **Balanced construction** – converts the linked list of contacts into a near-minimal-height BST by iteratively inserting middle elements of subranges.
- **Core operations exposed via menu**:
  1. Load contacts from one of the sample files (or a custom file) and build the tree.
  2. Find a contact by last name, or by last name and first name.
  3. Edit a contact (rename, change phone number) by rebuilding the relevant node when structure changes.
  4. Insert a new contact. Duplicate phone numbers are rejected. Contacts sharing the same name are inserted as the predecessor (left child) in line with the assignment option.
  5. Delete a contact by full name using the standard BST delete cases.
  6. Print the tree contents (in-order or pre-order) using Morris traversal to keep memory usage low.
  7. Delete the entire tree iteratively (pre-order) and free memory.
  8. Exit the program.

## Task 2 Highlights (`asp2dz1p2.cpp`)

The second program reuses the BST core and introduces limited histories for messaging and content sharing, matching the specification for the selected problem variant (list for message history, array for share history with thresholds):

- **Initialization** – loads contacts into a BST and lets the user configure:
  - `n`: maximum number of recent contacts stored in the messaging history (implemented as a move-to-front singly linked list per contact).
  - `x`: threshold parameter that controls when a shared-content record jumps to the front of the list (implemented as a dynamic array with access counters).
- **Send message** – finds the recipient in the BST and updates the sender history:
  - Existing history entries are moved to the front.
  - New entries are inserted at the front and the list is trimmed to length `n`.
- **Share content** – tracks per-contact sharing history:
  - Access counters increase for each share.
  - Records use a transposition heuristic; once a counter exceeds `x`, the record is moved straight to the front (move-to-front hybrid strategy from the assignment).
- **Diagnostics** – prints the resulting histories after each operation to help with manual verification.
- **Cleanup** – reuses the iterative tree deletion to release all resources on exit.

## Building

The programs target standard C++ (tested with C++17). Example build commands using `g++` on Windows:

```cmd
:: Task 1
g++ -std=c++17 -O2 -Wall asp2dz1p1.cpp -o contactbook_task1.exe

:: Task 2
g++ -std=c++17 -O2 -Wall asp2dz1p2.cpp -o contactbook_task2.exe
```

Any other modern C++ compiler (MSVC, Clang) should work as long as it supports C++17.

## Running

Execute one of the binaries and follow the on-screen menu:

```cmd
contactbook_task1.exe
```

Task 1 prompts you to choose an input file and then exposes the BST operations listed above. Make sure the chosen contact file is located alongside the executable or provide a path when asked for a custom filename.

```cmd
contactbook_task2.exe
```

Task 2 begins with the same load step, then asks for the messaging history limit `n` and the sharing threshold `x`. Afterwards you can:

1. Send a message to a contact (adds/moves an entry in the message history).
2. Share content with a contact (updates share statistics and reorders records based on the hybrid heuristics).
3. Exit (releasing all resources).

The console output shows the current state of the histories to make manual validation easier during the defense of the assignment.

## Dataset Format Reminder

Each data file begins with a header row and then lines in the following form:

```
LastName,FirstName,PhoneNumber
```

Example:

```
Lastname,Firstname,Phone
Balle,Danna,5012064229
Bohlin,Kriste,6771919493
```

Providing files with the same layout allows you to test the programs with custom datasets.
