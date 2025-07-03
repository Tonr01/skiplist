# Skip List in C with Interactive Testing (QTest)

This project implements a **Skip List data structure in C**, featuring an interactive test interface via `qtest` and a command parser module `console`. You can interactively insert, search, and manage the skip list through simple commands.

---

## Project Structure

```
.
├── Makefile         # Build rules
├── main.c           # Entry point (optional)
├── skiplist.c/.h    # Skip List implementation
├── qtest.c/.h       # Test interface and logic
├── console.c/.h     # Command parser module
└── README.md
```

---

## Build Instructions

Compile using GCC and the Makefile:

```bash
make
```

This will produce the executable:

```bash
make run
```

To clean up build files:

```bash
make clean
```

---

## Running the Test Interface

### Launch the interactive CLI:

```bash
make run
```

Then enter any of the following commands:

```
help           # Show all available commands
new            # Create a new skip list
insert 5       # Insert node 5
maxinsert 10   # Insert all nodes from 1 to 10
search 7       # Search for node 7
erase 10       # Delete node 10
print          # Display current skip list
free           # Free the entire skip list
quit           # Exit the program
```

---

## QTest Command Set (console)

| Command         | Description                        |
|----------------|------------------------------------|
| `help`         | Show available commands            |
| `new`          | Create a new skip list             |
| `insert <key>` | Insert a node                      |
| `maxinsert <k>`| Insert keys from 1 to k            |
| `search <key>` | Search if node exists              |
| `erase <key>`  | Delete a node                      |
| `print`        | Display skip list structure        |
| `free`         | Free the skip list from memory     |
| `quit`         | Exit the test interface            |

---

## Notes

- All memory is manually managed via `malloc`/`free`
- Requires a C99-compatible compiler (e.g., `gcc`)
- Recommended: Use `valgrind` to check for memory leaks

---
