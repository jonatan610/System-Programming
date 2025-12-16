//YEHONATAN_SHAPIRA_ID:208969246
#include <fcntl.h>      // Needed for opening files
#include <unistd.h>     // Needed for read, write, close
#include <stdlib.h>     // Needed for exit
//------------------------------------------------------                                     
#define BUFFER_SIZE 4096
#define NEW_FILE_RIGHTS 0644  // Rights: Owner can read/write, others can read
//------------------------------------------------------   
int main(int argc, char *argv[]) 
{
    // Variables
    int sourceFile;         // ID number for the file we copy FROM
    int destinationFile;    // ID number for the file we copy TO
    int amountRead;         // How many bytes we actually read from the disk
    char dataBuffer[BUFFER_SIZE]; // A temporary container for the data
    char userReply;         // To save the 'y' or 'n' from the user
//------------------------------------------------------   
    // 1. Check if the user gave us the right number of words
    // We need: The program name + Source file + Target file (3 words total)
    if (argc != 3) {
        write(2, "Usage: ./my_copy <source> <target>\n", 35);
        exit(1);
    }
//------------------------------------------------------   
    // 2. Open the source file
    // We only need to read from it (O_RDONLY)
    sourceFile = open(argv[1], O_RDONLY);
    
    // If the ID is less than 0, it means there was an error
    if (sourceFile < 0) {
        write(2, "Error: Cannot open source file\n", 31);
        exit(1);
    }
//------------------------------------------------------   
    // 3. Safety check: Does the target file already exist?
    // access() checks if the file is there (F_OK)
    if (access(argv[2], F_OK) == 0) {
        
        // Loop forever until we get a valid answer ('y' or 'n')
        while(1) {
            write(1, "Target exists. Overwrite? (y/n): ", 33);
            
            // Read 1 key from keyboard
            read(0, &userReply, 1);

            // Skip the "Enter" key (newline) so we don't print the question twice
            if (userReply == '\n') {
                continue;
            }
            
            // If user says 'n' (No), we stop the program
            if (userReply == 'n') {
                write(1, "Canceled.\n", 10);
                close(sourceFile);
                exit(0);
            }

            // If user says 'y' (Yes), we break the loop and continue
            if (userReply == 'y') {
                break; 
            }

            // If we got here, the input was not 'y' and not 'n', so the loop runs again.
        }
    }
//------------------------------------------------------   
    // 4. Create (or open) the destination file
    // O_WRONLY: We want to write to it
    // O_CREAT: Create it if it doesn't exist
    // O_TRUNC: If it exists, erase everything inside (start fresh)
    destinationFile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, NEW_FILE_RIGHTS);
    
    if (destinationFile < 0) {
        write(2, "Error: Cannot create target file\n", 33);
        close(sourceFile);
        exit(1);
    }
//------------------------------------------------------   
    // 5. The Main Loop
    // Keep reading chunks of data until the file ends
    // read() puts data into 'dataBuffer' and returns how much it read
    while ((amountRead = read(sourceFile, dataBuffer, BUFFER_SIZE)) > 0) {
        
        // Write the data we just read to the new file
        if (write(destinationFile, dataBuffer, amountRead) != amountRead) {
            write(2, "Error: Write failed\n", 20);
            close(sourceFile);
            close(destinationFile);
            exit(1);
        }
    }
//------------------------------------------------------   
    // 6. Finish up
    write(1, "Success!\n", 9);
    
    // close files when we done
    close(sourceFile);
    close(destinationFile);

    return 0;
}