# 🔎 Word Search Engine (C++ with Qt GUI)

This project is a **search engine** that allows users to search for words across multiple documents using an efficient **Trie (prefix tree)** data structure. It is implemented in **C++** using the **Qt GUI framework**, as part of a Data Structure course.

## 💡 Features

- Searching for a specific word across all given documents.
- Displaying which documents contain the word.
- A search feature for suggesting similar words if the input word is not found (with one character difference).
- Supports advanced word filtering, allowing the user to:
  - Find documents that contain at least one of the selected words
  - Find documents that contain all of the selected words
  - Find documents that contain none of the selected words
- All words are inserted into a single Trie for fast lookup.

---

## 🛠 Technical Details

- Implemented in **C++**
- Uses **Qt framework** for GUI
- Uses custom implemented **Trie** (no external tree libraries used)

---

## 📁 Project Structure

```
/SearchProject
├── main.cpp
├── FileNamesTable.h / .cpp
├── Node.h / .cpp
├── Search.h / .cpp / .ui
├── Tree.h / .cpp
├── scr-ui.pro
└── README.md
```

---

## 🚀 How to Run

- **Qt Creator IDE** (used to develop the entire project)
- **Qt libraries and dependencies** (to build and run correctly)
- Make sure the compiler is set to support C++17 or later

---

## 👨‍💻 Author

- Developed as part of a C++ Data Structure course final project
