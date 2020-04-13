/* blah_list.h - Implements a doubly linked list structure.
	List structure maintains the pointer to the first Blah_List_Element structure */

#ifndef _BLAH_LIST

#define _BLAH_LIST

#define BLAH_LIST_NAME_LENGTH 20  //number of characters allowed for name property

#include "blah_types.h"


/* Type Definitions */

typedef bool blah_list_search_func(void* elementData, void* param); // This type function type should perform a check on element_data, with given parameter supplied, and return TRUE if check passes
typedef void blah_list_element_dest_func(void* elementData); // This type of function is used to destroy data pointed to by *element_data
typedef void blah_list_element_func(void* elementData); // This type of function will perform some sort of non destructive operation using the object pointed to by element_data
typedef void blah_list_element_func_1arg(void* elementData, void* param); // This type of function will perform some sort of non destructive operation using the object pointed to by element_data with supplied argument
typedef bool blah_list_element_bool_func_1arg(void* elementData, void* param); // This type of function will perform some sort of non destructive operation using the object pointed to by element_data and supplied argument, returning a boolean result

typedef int blah_list_sort_func(void* elemData1, void* elemData2);
	// Function to sort elements in a list by comparison of two elements at a time.
	// Function is called as comp((void*)elem1->data,(void*)elem2->data).
	// Function should behave like strcmp() and return a value smaller than 0
	// if elem1->data < elem2->data, return 0 if equal, or > 0 if elem1->data > elem2->data

/* Structure Definitions */

typedef struct Blah_List_Element { //defines a simple element structure to hold a pointer to list data
	struct Blah_List_Element* prev;	//pointer to previous list element
	struct Blah_List_Element* next;	//pointer to next list element
	void* data;					//pointer to element data
} Blah_List_Element;

typedef struct Blah_List {
	char name[BLAH_LIST_NAME_LENGTH+1];//name of list!
	Blah_List_Element* first;		//pointer to start of element list
	Blah_List_Element* last;			//pointer to end of element list
	blah_list_element_dest_func* destroyElementFunction; //custom function to destroy element data
	int length;					//number of elements in the list
} Blah_List;

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

/* Element Function Prototypes */

Blah_List_Element *Blah_List_Element_new(void *data);
	//Creates a new list element.  Returns a pointer to newly created element
	//on success, or NULL pointer if error occurred.

void Blah_List_Element_init(Blah_List_Element *element,  void *data);
	//Initialises given element structure

void Blah_List_Element_callFunction(Blah_List_Element* element, blah_list_element_func* function);
void Blah_List_Element_callWithArg(Blah_List_Element* element, blah_list_element_func_1arg* function, void *arg);
bool Blah_List_Element_callArgReturnBool(Blah_List_Element* element, blah_list_search_func* func, void* arg);

/* List Function Prototypes */


void Blah_List_appendElement(Blah_List* list, void* data);
	//Appends a new element to the end of the list with given data ptr

void Blah_List_callFunction(Blah_List* list, blah_list_element_func* function);
	//Calls function for each element, using data pointer as argument to given function

void Blah_List_callWithArg(Blah_List* list, blah_list_element_func_1arg* function, void* arg);
	//Calls function for each element, using data pointer as argument to given function

blah_pointerstring Blah_List_createPointerstring(Blah_List *list);
	//Returns an allocated array of pointers to the data contained in each of the
	//list elements, in the order they occur.  The last element of the return array
	//is a NULL pointer to signify the end.

void Blah_List_destroy(Blah_List *list);
	// Clears all memory occupied by list and elements

void Blah_List_destroyElements(Blah_List *list);
	// clears all memory allocated for elements and data but does not destroy basic list header

Blah_List_Element *Blah_List_findElement(const Blah_List *list, const void *data);
	// Finds list element with given data

void Blah_List_init(Blah_List *list, const char *name);
	// Sets the name of the list, and all element pointers to NULL

void Blah_List_insertElement(Blah_List *list, void *data);
	// Inserts a new element at the beginning of the list with given data ptr

Blah_List *Blah_List_new(const char *name);
	// Creates a new empty list given a name as a null terminated string in parameter 'name'.
	// Function returns pointer to new list on success, or NULL pointer if error occurred.

void *Blah_List_popElement(Blah_List *list);
	// Function Blah_List_pop_element
	// Removes first element from list and returns data pointer


void Blah_List_removeAll(Blah_List *list);
	// removes all elements but retains empty list structure.  Does not free data

bool Blah_List_removeElement(Blah_List *list, void *data);
	// Removes element with given data from list.  Does not free data pointed to by list element.
	// Returns zero if error

void* Blah_List_search(Blah_List* list, blah_list_search_func* searchFunction, void* searchArg);
	// Calls search_function for each element of the list, using the element's data
	// as the first argument and 'arg' as second.  Returns the data pointer of the first element for which
	// search_function returns true, or NULL if no match;

//FIXME: Make this function into a parameter of init routine
void Blah_List_setDestroyElementFunction(Blah_List* list, blah_list_element_dest_func* function);
	// Sets the function pointer to the given function used to destroy element data
	// contained in elements of this list

void Blah_List_sort(Blah_List* list, blah_list_sort_func* compareFunction);
	// Performs a linear sort of the list, using the supplied function to compare two elements.
	// compare_function is called as comp((void*)elem1->data,(void*)elem2->data).
	// compare_function should behave like strcmp() and return a value smaller than 0
	// if elem1->data < elem2->data, return 0 if equal, or > 0 if elem1->data > elem2->data

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
