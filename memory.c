#include <stdlib.h>

#include "memory.h"


void* reallocate(void* pointer, size_t oldSize, size_t newSize){
    if (newSize == 0){
        free(pointer); 
        return NULL; 
    }

    void* result = realloc(pointer, newSize); // considers all the cases (oldSize > newSize; oldSize < newSize; oldSize>0 and newSize = 0, ..) and handles them
    if (result == NULL) exit(1); 
    return result; 

}

