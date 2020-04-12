/* blah_stack.h - Implements a stack structure.
	A stack is a generic memory buffer that can be added and subtracted from.
	Standard push and pop operations allow a non type specific amount of bytes
	to be added or retrieved from the heap.	Implementation is not very flexible
	and aimed at speed. */

#ifndef _BLAH_STACK

#define _BLAH_STACK

#define BLAH_STACK_NAME_LENGTH	7  //number of characters allowed for name property

#include "blah_types.h"

/* Structure Definitions */

typedef struct Blah_Stack {
	char name[BLAH_STACK_NAME_LENGTH+1];//name of list!
	unsigned int capacity; //number of bytes buffer can hold
	unsigned int bytesStored; //Number of bytes stored in stack memory
	void *storageBuffer;			//pointer to the storage buffer
} Blah_Stack;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Stack *Blah_Stack_new(char *stackName, unsigned int capacity);
	//creates new empty stack of given capacity

void Blah_Stack_destroy(Blah_Stack *stack);
	//Frees all memory occupied by stack structure.  First data buffer, then remaining
	//memory

void Blah_Stack_destroyBuffer(Blah_Stack *stack);
	//clears only memory buffer allocated for data storage

void Blah_Stack_disable(Blah_Stack *stack);
	//frees stack data buffer

void Blah_Stack_init(Blah_Stack *stack, char *stackName, unsigned int capacity);
	//Initialises a stack to be empty, assigns name and allocates storage buffer

bool Blah_Stack_pop(Blah_Stack *stack, void *destination, unsigned int bytes);
	//Moves 'bytes' number of bytes from the end of the stack buffer into the
	//memory location designated by 'destination'.  Returns TRUE on success, else FALSE

bool Blah_Stack_push(Blah_Stack *stack, void *source, unsigned int bytes);
	//Copies 'bytes' number of bytes from to the end of the stack buffer from the
	//memory location designated by 'source'.  Returns TRUE on success, else FALSE


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
