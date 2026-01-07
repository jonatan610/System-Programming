# Assignment 2: File Copy Program (my_copy)-SDD
**GitHub Repository:**  
[Click here to view the project on GitHub](https://github.com/jonatan610/System-Programming/tree/main/ex2)

**Student Name:** Yehonatan Shapira  
**ID:** 208969246  


## 1. Description
This program is a file copy tool for Linux. It works exactly like the standard `cp` command.

Instead of using standard C functions (like `fopen`) which act as wrappers, this program uses **Linux System Calls** directly (like `open`, `read`, `write`).
* The program runs in **User Space** (where applications live).
* It interacts directly with the **Kernel** (the core of the OS) to access the hard drive.

## 2. How it Works (Logic)
The program follows these simple steps:

1.  **Check Input:** First, it checks if you provided the Source file and the Target file.
2.  **Open Source:** It tries to open the source file to read from it.
3.  **Safety Check:** Before creating the new file, it checks if a file with that name already exists.
    * If yes: It asks you: "Target exists. Overwrite? (y/n)".
    * If you say 'n': It stops.
    * If you say 'y': It continues.
4.  **Create Target:** It creates the new file (or clears the old one if you said 'y').
5.  **Copy Loop:** It reads a chunk of data (4KB) from the source and writes it to the target. It repeats this until the whole file is copied.
6.  **Finish:** It closes the files and prints "Success!".

## 3. Efficiency & Kernel Interaction
**Buffer Size:** 4096 Bytes (4KB).

**Why is this efficient? (User Space vs. Kernel Space)**
* **The Cost of System Calls:** My program runs in *User Mode*, but accessing the disk requires *Kernel Mode*.
* **Context Switching:** Every time we call `read()` or `write()`, the CPU must pause the program, switch to the Kernel to do the work, and then switch back. This operation (Context Switch) is "expensive" in terms of performance.
* **The Buffer Solution:**
    * Reading byte-by-byte means 1,000,000 switches for a 1MB file.
    * Reading 4KB at a time means only ~250 switches for the same file.
    * **Result:** By using a buffer that matches the Linux Page Size (4KB), we minimize these expensive switches and make the program significantly faster.

## 4. System Calls Used
I used these 5 main system calls:

* **`open()`**: To request the Kernel to open/create files.
* **`access()`**: To ask the Kernel if a file already exists.
* **`read()`**: To read data from the file (or keyboard) into a buffer.
* **`write()`**: To write data from the buffer to the disk or screen.
* **`close()`**: To tell the Kernel we are done with the file descriptors.

## 5. Error Handling
The program is safe. If anything goes wrong (like a missing file or no permission), it:
1.  Detects the error immediately (checking return values).
2.  Prints a clear message to the error output (stderr).
3.  Stops the program safely.

## 6. How to Run
**Compile:**
```bash
gcc -Wall my_copy.c -o my_copy