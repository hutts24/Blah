/* blah_tree.h - Implements a binary tree structure.
	Tree structure maintains the pointer to the first Blah_Tree_Element structure */
#ifndef _BLAH_TREE

#define _BLAH_TREE

#define BLAH_TREE_NAME_LENGTH	20  //number of characters allowed for name property
#define BLAH_TREE_ELEMENT_NAME_LENGTH	50

#include "blah_types.h"

/* Function Type Definitions */

typedef blah_bool blah_tree_search_func(void *element_data, void *param);
	//This type function type should perform a check on element_data, with given
	//parameter supplied, and return TRUE if check passes

typedef void blah_tree_element_dest_func(void *element_data);
	//This type of function is used to destroy data pointed to by *element_data

typedef void blah_tree_element_func(void *element_data);
	//This type of function will perform some sort of non destructive operation
	//using the object pointed to by element_data

typedef void blah_tree_element_func_1arg(void *element_data, void *param);
	//This type of function will perform some sort of non destructive operation
	//using the object pointed to by element_data with supplied argument

typedef blah_bool blah_tree_element_bool_func_1arg(void *element_data, void *param);
	//This type of function will perform some sort of non destructive operation
	//using the object pointed to by element_data and supplied argument, returning a
	//boolean result

typedef int blah_tree_sort_func(void *elem_data_1, void *elem_data_2);
	//Function to sort elements in a tree by comparison of two elements at a time.
	//Function is called as comp((void*)elem1->data,(void*)elem2->data).
	//Function should behave like strcmp() and return a value smaller than 0
	//if elem1->data < elem2->data, return 0 if equal, or > 0 if elem1->data > elem2->data


/* Structure Definitions */

typedef struct Blah_Tree_Element {
	//defines a simple element structure to hold a pointer to tree data
	char keystring[BLAH_TREE_ELEMENT_NAME_LENGTH+1];
	struct Blah_Tree_Element *left;	//pointer to left element
	struct Blah_Tree_Element *right;	//pointer to right element
	void *data;					//pointer to element data
} Blah_Tree_Element;

typedef struct Blah_Tree {
	char name[BLAH_TREE_NAME_LENGTH+1];//name of tree!
	Blah_Tree_Element *first;		//pointer to first element in tree
	blah_tree_element_dest_func *destroyElementFunction; //custom function to destroy element data
	unsigned int count;					//number of elements in the tree
} Blah_Tree;

/* Element Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Tree_Element *Blah_Tree_Element_new(char *key, void *data);
	//Creates a new tree element

void Blah_Tree_Element_callFunction(Blah_Tree_Element *element, blah_tree_element_func *function);
	//call function with data pointer of element

void Blah_Tree_Element_callWithArg(Blah_Tree_Element *element, blah_tree_element_func_1arg *function, void *arg);
	//call function with data pointer of element and single void pointer arg

blah_bool Blah_Tree_Element_callArgReturnBool(Blah_Tree_Element *element, blah_tree_element_bool_func_1arg *function, void *arg);
	//call function for with data pointer of element

void *Blah_Tree_Element_search(Blah_Tree_Element *treeElement,
 blah_tree_search_func *searchFunction, void *arg);
	//Performs a linear recursive search from location of given element in tree.
	//Calls search_function for each element of the tree in sort order, using the
	//element's data as the argument and 'arg' as a second argument.  Returns the data
	//pointer of the first element for which search_function returns BLAH_TRUE, or
	//NULL if no match;

/* Tree Function Prototypes */

Blah_Tree *Blah_Tree_new(char *name);	//creates new empty tree with given name

blah_bool Blah_Tree_removeElement(Blah_Tree *tree, char *key);
	// Function Blah_Tree_remove_element:
	//Removes element with given key from tree.  Does not free data pointed to by tree element.
	//Returns zero if error

blah_bool Blah_Tree_insertElement(Blah_Tree *tree, char *key, void *data);
	// Function Blah_Tree_insert_element:
	//Inserts a new element into the tree, with given key and data pointer

void Blah_Tree_callFunction(Blah_Tree *tree, void *function);
	//Function Blah_Tree_call_function:
	//Calls function for each element, using data pointer as argument to given function

void Blah_Tree_callWithArg(Blah_Tree *tree, void *function, void *arg);
	// Function Blah_Tree_call_with_arg: Calls function for each element, using data pointer as argument to given function

//FIXME: Add destroy element function as parameter to init routine
void Blah_Tree_init(Blah_Tree *tree, const char *name);
	// Function Blah_Tree_init_function: Sets the name of the tree, and first element pointer to NULL

Blah_Tree_Element *Blah_Tree_findElement(Blah_Tree *tree, const char *key);
	//Function Blah_Tree_find_element: Finds tree element with given key */

void *Blah_Tree_search(Blah_Tree *tree, blah_tree_search_func *searchFunction, void *arg);
	//Calls search_function for each element of the tree in sort order, using the
	//element's data as the argument and 'arg' as second.  Returns the data pointer
	//of the first element for which search_function returns BLAH_TRUE,
	//or NULL if no match;

void Blah_Tree_destroy(Blah_Tree *tree);
	// Function Blah_Tree_destroy:
	//Clears all memory occupied by tree and elements

void Blah_Tree_destroyElements(Blah_Tree *tree);
	//clears all memory allocated for elements and data but does not destroy basic tree header

void Blah_Tree_removeAll(Blah_Tree *tree);
	//removes all elements but retains empty tree structure.  Does not free data

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
