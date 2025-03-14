#ifndef clox_chunk_h
#define clox_chunk_h


#include "common.h"
#include "value.h"

typedef enum{
    OP_RETURN, //return from current func; opcode
    OP_CONSTANT, 
    OP_NEGATE, 
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
} Opcode;

typedef struct {
    int count; //how many of the allocated entries are acctually in use
    int capacity; //capacity of the array
    uint8_t* code; //dynamic array
    int* lines; //to store which line each bytecode came from
    ValueArray constants; 
} Chunk; 

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line); //adding extra parameter line to know which line the bytecode came from
int addConstant(Chunk* chunk, Value value);




#endif