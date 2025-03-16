#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void repl() {
    char line[1024]; //fixed size ch array to store user input
    for (;;){
        printf("> "); 

        if(!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line); //real work happens here

    }
}

static void runFile(const char* path){
    char* source = readFile(path); 
    InterpretResult result = interpret(source);
    free(source); 

    if(result == INTERPRET_COMPILE_ERROR) exit(65);
    if(result == INTERPRET_RUNTIME_ERROR) exit(70);

}

static char* readFile(const char* path) {
    //open the file, go to the end to figure out the size, revert to the beginning, allocate mem, read, add null term, close, return

    FILE* file = fopen(path, "rb"); 

    if (file == NULL) {
        fprintf(stderr, "Couldn't open file \"%s\".\n", path); 
        exit(74);
    }

    fseek(file, 0L, SEEK_END); 
    size_t fileSize = ftell(file); 
    rewind(file); //can also do fseek(file, 0, SEEK_SET) to move back to start

    char* buffer = (char*) malloc(fileSize+1); //allocates mem for file contents (+1 to store the null terminator at the end)

    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n"), path;
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file); 

    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path); 
        exit(74);
    }

    buffer[bytesRead] = '\0'; //null terminator

    fclose(file); 
    return buffer; 

}

int main(int argc, const char* argv[]){

    initVM(); //initialize the virtual machine when interpreter first starts
    Chunk chunk;
    initChunk(&chunk);


    //     ==test chunk== (without line numbers)
    // 0000 OP_CONSTANT         0 ' 1.2
    // 0002 OP_CONSTANT         1 ' 2.3
    // 0004 OP_RETURN

    //     ==test chunk== (with same line numbers)
    // 0000  123 OP_CONSTANT         0 ' 1.2
    // 0002    | OP_CONSTANT         1 ' 2.3
    // 0004    | OP_RETURN

    //     ==test chunk== (with different line numbers)                                                        
    // 0000  123 OP_CONSTANT         0 ' 1.2
    // 0002  223 OP_CONSTANT         1 ' 2.3
    // 0004  334 OP_RETURN
    
    // writeChunk(&chunk, OP_NEGATE, 123);

    // writeChunk(&chunk, OP_RETURN, 334);

    //if you pass no arguments, you are dropped into repl
    //single cmd line is understood to be the path to a script to run
    if (argc == 1) {
        repl(); 
    } else if (argc == 2){
        runFile(argv[1]); 
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64); 
    }

    freeVM(); 

    freeChunk(&chunk); 

    return 0;
}