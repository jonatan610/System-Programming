# Homework 4 – Multithreaded Echo Server & Resource Synchronization

[Full project folder on GitHub](https://github.com/jonatan610/System-Programming/tree/main/ex4)

## 1. Project Objective

The purpose of this project is to implement a multithreaded Echo server using system calls in Linux. The server handles multiple clients concurrently using POSIX threads. It receives text from each client, converts all lowercase letters to uppercase, and sends the result back.

A global variable tracks the number of connected clients. This variable is shared across threads and protected using a POSIX mutex to ensure correct synchronization.

The client program simulates load by launching multiple threads (at least five), where each thread acts as an independent client that connects to the server, sends a message, and prints the response.

---

## 2. System Calls Used

| System Call            | Description |
|------------------------|-------------|
| socket()               | Creates a TCP socket |
| bind()                 | Binds the server socket to a local address and port |
| listen()               | Listens for incoming client connections |
| accept()               | Accepts a connection from a client |
| connect()              | Connects a client socket to the server |
| send()                 | Sends data through a socket |
| recv()                 | Receives data from a socket |
| close()                | Closes a socket descriptor |
| pthread_create()       | Creates a new thread |
| pthread_detach()       | Detaches a thread (server side) |
| pthread_join()         | Waits for thread termination (client side) |
| pthread_mutex_lock()   | Locks a mutex (enters critical section) |
| pthread_mutex_unlock() | Unlocks a mutex (exits critical section) |

---

## 3. Flow Description

### Server Side:
1. Create a TCP socket and bind it to port 12345 on localhost (127.0.0.1).
2. Start listening for incoming connections.
3. For each client connection:
   - Accept the connection.
   - Allocate memory for the socket descriptor.
   - Create a new thread to handle the client.
   - Inside the thread:
     - Lock the mutex and increment the global client counter.
     - Receive the message from the client using a custom recv_all function.
     - Convert the message to uppercase.
     - Send the modified message back using send_all.
     - Close the client socket.
     - Lock the mutex and decrement the client counter.
     - Exit the thread.

### Client Side:
1. Create five threads using pthread_create.
2. Each thread:
   - Creates a socket and connects to the server.
   - Sends a predefined message.
   - Receives the processed message from the server.
   - Prints the received message.
   - Closes the connection.

---

## 4. Buffer Management and Efficiency

- The server uses a fixed-size buffer of 4096 bytes.
- To ensure complete message delivery, `recv_all` and `send_all` helper functions were implemented using loops that continue reading/writing until the full message is transferred.
- This guarantees that even if a system call returns fewer bytes than requested, the operation will complete successfully.
- The number of system calls is minimized by reusing sockets and reading full messages in controlled chunks.

---

## 5. Error Handling

- Each system call is followed by an error check.
- If a call fails (e.g., `socket()`, `accept()`, `connect()`, etc.), the error is printed using `perror()` and handled appropriately (freeing resources, closing sockets).
- Threads are properly detached or joined to avoid memory leaks or zombie threads.
- Mutex operations are always paired (lock/unlock), ensuring the critical section is safely accessed.

---

## 6. Compilation and Execution
## How to Run

First, open a terminal in the project directory and compile the project by running:

make

This command will compile the source files and create two executables:  
server and client.

After compilation, run the server in one terminal window using:

./server

The server will start listening for incoming connections.

Then, open a second terminal window and run the client using:

./client

The client will create multiple threads, connect to the server, send messages, and print the responses received from the server.

If you want to remove the compiled files and rebuild the project, you can run:

make clean


