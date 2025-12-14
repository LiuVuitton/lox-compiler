# C++ Lox Interpreter

This project is a C++ implementation inspired by **_Crafting Interpreters_** by Robert Nystrom.  
While the original book implements the Lox language in Java (and later in C), this repository explores the same ideas using **modern C++**.

---

## Motivation

I originally started this project while working on a **multi-language visual programming environment with AI assistance**. That work naturally raised questions about language design, execution models, and how compilers and interpreters actually function under the hood.

In a sense, the project was also connected to experimenting with a **common robot language**, where multiple frontends could map to a shared execution model. To better understand these concepts, I decided to study interpreters and compilers more deeply, and *Crafting Interpreters* provided the perfect foundation.

---

## Project History

This project was developed **gradually in my spare time**. During that process, I learned a great deal about:

- Parsing and AST construction  
- The visitor pattern  
- Runtime representation of values  
- Error handling in interpreters  
- C++ design tradeoffs compared to Java  

After gaining more experience with modern C++, I decided to **revisit and reimplement the project from scratch**, applying better design choices and cleaner abstractions.

---

## Current Status

**Work in progress**

The interpreter is still under active development and refactoring.  
Expect incomplete features, API changes, and evolving design decisions.

---

## Acknowledgements

- **Robert Nystrom**, for the excellent book *Crafting Interpreters*  
- The open-source community for tools, discussions, and inspiration
