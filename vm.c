//give the virtual machine a "chunk" of code and it runs it

#include <stdio.h>
#include "common.h"
#include "vm.h"
#include "debug.h"

VM vm; //global instantation -> bad (no flexibility, poor mem management); will change later

static void resetStack() {
    vm.stackTop = vm.stack; //point the top to the beginning of the stack to indicate that stack is empty
}

void initVM() {
    resetStack(); 
}

void freeVM() {

}

void push(Value value){
    *vm.stackTop = value; 
    vm.stackTop++; 
}

Value pop(){
    vm.stackTop--;
    return *vm.stackTop; 
}

static InterpretResult run() {
    //macros for reading bytes and constants
    #define READ_BYTE() (*vm.ip++) //fetches the next inst
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()]) //fetches the constatn value

    #define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)

    for (;;){

        #ifdef DEBUG_TRACE_EXECUTION 

            printf("          ");
            for(Value* slot = vm.stack; slot < vm.stackTop; slot++){
                printf("[ "); 
                printValue(*slot); 
                printf(" ]");
            }
            printf("\n"); 

            disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code)); 

        #endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()){
            case OP_RETURN: {
                printValue(pop()); 
                printf("\n");
                return INTERPRET_OK;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT(); 
                push(constant);
                break;
            }
            case OP_NEGATE: {
                push(-pop()); 
                break;
            }
            case OP_ADD:      BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE:   BINARY_OP(/); break;
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk){
    vm.chunk = chunk;
    vm.ip = vm.chunk->code; //set the ip to the first line of code
    return run();
}

