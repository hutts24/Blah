/* blah_stack.c - Implements a operations on stack structures.
	A stack is a generic memory buffer that can be added and subtracted from.
	Standard push and pop operations allow a non type specific amount of bytes
	to be added or retrieved from the heap.	Implementation is not very flexible
	and aimed at speed. */

#include <malloc.h>
#include <string.h>
#include <stdio.h>


#include "blah_stack.h"
#include "blah_types.h"
#include "blah_util.h"

/* Function Declarations */

Blah_Stack *Blah_Stack_new(char *stackName, unsigned int capacity) {
	//creates new empty stack of given capacity
	Blah_Stack *newStack = (Blah_Stack*)malloc(sizeof(Blah_Stack));
	Blah_Stack_init(newStack, stackName, capacity);
	//allocate storage buffer

	return newStack;
}
		
void Blah_Stack_destroy(Blah_Stack *stack) {
	//Frees all memory occupied by stack structure.
	//First data buffer, then remaining memory
	Blah_Stack_disable(stack);
	free(stack);
}

void Blah_Stack_destroyBuffer(Blah_Stack *stack) { 
	//clears only memory buffer allocated for data storage
	free(stack->storageBuffer);
}

void Blah_Stack_disable(Blah_Stack *stack) {
	Blah_Stack_destroyBuffer(stack);
}

void Blah_Stack_init(Blah_Stack *stack, char *stackName, unsigned int capacity) {
	//Initialises a stack to be empty, assigns name and allocates storage buffer
	blah_util_strncpy(stack->name, stackName, BLAH_STACK_NAME_LENGTH);
	stack->bytesStored = 0;
	stack->capacity = capacity;
	stack->storageBuffer = malloc(capacity);	
}


blah_bool Blah_Stack_pop(Blah_Stack *stack, void *destination, unsigned int bytes) {
	//Moves 'bytes' number of bytes from the end of the stack buffer into the
	//memory location designated by 'destination'.  Returns TRUE on success, else FALSE
	if (bytes > stack->bytesStored)  //If requested data size is more than available
		return BLAH_FALSE;  //return false for failure
	else { //copy data into stack buffer
		memcpy(destination, stack->storageBuffer + stack->bytesStored, bytes); 
		stack->bytesStored -= bytes;
		return BLAH_TRUE; //return true for success
	}	
}		
	
	
blah_bool Blah_Stack_push(Blah_Stack *stack, void *source, unsigned int bytes) {
	//Copies 'bytes' number of bytes from to the end of the stack buffer from the
	//memory location designated by 'source'.  Returns TRUE on success, else FALSE
	if (bytes > (stack->capacity - stack->bytesStored))  //If new block of data is too large to fit,
		return BLAH_FALSE;  //return false for failure
	else {
		memcpy(stack->storageBuffer + stack->bytesStored, source, bytes); //copy data into stack buffer
		stack->bytesStored+=bytes;
		return BLAH_TRUE; //return true for success
	}	
}		
		
