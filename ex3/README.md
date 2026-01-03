# bash_mini – Project Description & Design

## GitHub Submission Link

All required files are available at:
https://github.com/jonatan610/System-Programming/blob/main/ex3

Project Description

This project implements a simple shell (command-line interpreter) in the C programming language.
It was developed as part of the "System Programming" course at Shenkar College.

The shell behaves like a minimal version of Bash. It reads user commands, checks whether the command is internal (like cd or exit) or external (like ls, cat, date), and handles each accordingly using direct system calls only, with no high-level wrapper functions.

Project Objectives

- Understand the internal structure of a Unix shell.
- Learn how to use system calls directly in C (fork, execv, waitpid).
- Practice managing processes, parsing input, and handling directories.
- Write efficient and reliable code that interacts with the Linux kernel.

How the Program Works

1. The program enters a loop and displays a prompt: bash-mini$
2. The user types a command (for example: ls -l)
3. Input is read using fgets()
4. The line is split into tokens using strtok()
5. The shell checks if the command is internal:
   - exit causes the shell to exit
   - cd changes the current working directory
     - If a path is provided, chdir() is used with that path
     - If no path is provided, the shell changes to the HOME directory using getenv("HOME")
6. If the command is external:
   - The shell searches for an executable file first in the HOME directory and then in /bin
   - If found and executable:
     - fork() creates a child process
     - execv() runs the command in the child process
     - waitpid() makes the parent wait for completion
   - If not found, the shell prints: [command]: Unknown Command
7. If a system call fails, perror() prints the system error
8. The loop repeats until the user types exit

Supported Commands

Internal Commands:
- exit – exits the shell
- cd [path] – changes to the specified path
- cd – changes to the HOME directory

External Commands:
Any executable found in the HOME directory or /bin, such as:
- ls
- pwd
- date
- cat filename

If a command is not found, the shell prints:
[command]: Unknown Command

If the command exists but cannot be executed, perror() displays the error.

Example Session

bash-mini$ ls
main.c  bash_mini
Command finished with return code: 0

bash-mini$ cd ..
bash-mini$ pwd
/home/student
Command finished with return code: 0

bash-mini$ cd nosuchfolder
cd: No such file or directory

bash-mini$ foobar
[foobar]: Unknown Command

bash-mini$ exit

System Calls Used

fgets() – read user input
strtok() – parse command and arguments
getenv() – access HOME environment variable
chdir() – change current working directory
access() – check if file exists and is executable
fork() – create child process
execv() – execute external command
waitpid() – wait for child process to finish
perror() – print system error messages

Program Flow Summary

Display prompt
Read input with fgets()
Parse input with strtok()
If command is exit, terminate shell
If command is cd, change directory
Otherwise search executable in HOME and /bin
If found, fork and execv, then waitpid
If not found, print error
Repeat loop

Efficiency

No dynamic memory allocation is used.
All buffers are fixed-size arrays.
Parsing is done in place with no unnecessary copying.
Only required system calls are performed.

Error Handling

All system calls are checked for errors.
perror() is used to print detailed error messages.
Invalid directories and unknown commands are handled gracefully.


## Program Design and Architecture

The program is designed as a simple loop-based shell that runs continuously until the user exits.  
The entire logic is implemented inside a single main control loop, similar to how a real Unix shell operates.

The shell is divided conceptually into the following stages:

### 1. Input Handling
- The shell prints a fixed prompt and waits for user input.
- Input is read using a fixed-size buffer with `fgets()`, avoiding dynamic memory allocation.
- If `fgets()` returns `NULL` (e.g. Ctrl+D), the program exits.

### 2. Parsing
- The input line is parsed in-place using `strtok()`.
- It splits the command line into tokens based on spaces and tabs.
- The first token is treated as the command, and the rest as arguments.

### 3. Command Classification
- After parsing, the shell checks whether the command is:
  - Internal (`cd`, `exit`)
  - External (e.g., `ls`, `pwd`, `date`)

### 4. Internal Command Design
- Internal commands are handled directly inside the shell process:
  - `exit` — terminates the shell loop.
  - `cd` — changes the shell’s working directory using `chdir()`.

> Why `cd` must be internal: Changing the directory in a child process would not affect the parent shell, so it must be done in the main process.

### 5. External Command Execution
- If the command is not internal, the shell looks for an executable:
  1. First in the user's `$HOME` directory
  2. Then in the `/bin` directory
- If found and executable:
  - The shell creates a child process using `fork()`.
  - The child uses `execv()` to execute the command.
  - The parent waits using `waitpid()` and prints the return code.

> This model mimics how traditional shells handle external programs.

### 6. Process Management Design
- Each external command runs in its own child process.
- The shell always waits for the command to finish before continuing.
- This ensures simple, predictable behavior (no background jobs, no multitasking).

### 7. Efficiency Considerations
- No use of `malloc()` or dynamic memory
- Fixed-size buffers for input and arguments
- In-place parsing with `strtok()`
- Minimal and necessary system calls only

### 8. Error Handling Design
- All system calls are checked for failure.
- On error, `perror()` prints a system-level explanation (e.g., permission denied, file not found).
- The shell never crashes due to invalid input or failed commands.
- Unknown commands and invalid directories are handled gracefully with clear error messages.



Files in the Project

bash_mini.c – C source code
Makefile – compilation and run automation
bash_mini – compiled executable
README.md – project description and design


How to Compile and Run

Compile the program:
make

Clean compiled files:
make clean

Run the shell:
make run

Or run manually:
./bash_mini

Notes

This shell is intentionally minimal.
It does not support pipes, redirection, or background execution.
The project focuses on process management and system call usage.
