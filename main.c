#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]){

    initVM(); //initialize the virtual machine when interpreter first starts
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2); 
    writeChunk(&chunk, OP_CONSTANT, 123); 
    writeChunk(&chunk, constant, 123); 
    int constant2 = addConstant(&chunk, 2.3); 
    writeChunk(&chunk, OP_CONSTANT, 223); 
    writeChunk(&chunk, constant2, 223); 

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

    constant = addConstant(&chunk, 3.4);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
  
    writeChunk(&chunk, OP_ADD, 123);
  
    constant = addConstant(&chunk, 5.6);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
  
    writeChunk(&chunk, OP_DIVIDE, 123);
    
    //after we create a chunk, pass it to disassembler
    disassembleChunk(&chunk, "test chunk");

    interpret(&chunk); 
    freeVM(); 

    freeChunk(&chunk); 

    return 0;
}