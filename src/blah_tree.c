#include <malloc.h>
#include <string.h>
#include <stdio.h>


#include "blah_tree.h"
#include "blah_types.h"
#include "blah_macros.h"
#include "blah_util.h"

/* Private internal functions */

static Blah_Tree_Element **Blah_Tree_Element_findPosition(Blah_Tree_Element **beginAddr, char *key);
	//Returns address of pointer where element should be located in tree

static void Blah_Tree_Element_recursiveCall(Blah_Tree_Element *element, void *function) { 
	//Recurse into left and right elements of parent element and
	//call function for with data pointer for every element
	if (element->left)
		Blah_Tree_Element_recursiveCall(element->left, function);
	else {
		Blah_Tree_Element_callFunction(element, function);
		
		if (element->right)
			Blah_Tree_Element_recursiveCall(element->right, function);
	}	
}

static void Blah_Tree_Element_recursiveCallWithArg(Blah_Tree_Element *element, void *function, void *arg) { 
	//Recurse into left and right elements of parent element and
	//call function for with data pointer for every element with single argument 'arg'
	if (element->left) //call for left if valid
		Blah_Tree_Element_recursiveCallWithArg(element->left, function, arg); 
	if (element->right) //call for right if valid
		Blah_Tree_Element_recursiveCallWithArg(element->right, function, arg); 
	
	Blah_Tree_Element_callWithArg(element, function, arg);
}

static void Blah_Tree_Element_recursiveRemove(Blah_Tree_Element *element) { 
	//Recurse into left and right elements of parent element and
	//frees memory occupied by element structure, but does not free data
	if (element->left) //call for left if valid
		Blah_Tree_Element_recursiveRemove(element->left); 
	if (element->right) //call for right if valid
		Blah_Tree_Element_recursiveRemove(element->right); 
	
	free(element);
}

static void Blah_Tree_Element_recursiveDestroy(Blah_Tree_Element *element,
	blah_tree_element_dest_func *destFunc) { 
	//Recurse into left and right elements of parent element and
	//frees memory occupied by element structure, and also destroys the data contained
	//stored in element->data
	//fprintf(stderr,"Recursive element destroy %p\n",element);
	if (element->left) //call for left if valid
		Blah_Tree_Element_recursiveDestroy(element->left, destFunc); 
	if (element->right) //call for right if valid
		Blah_Tree_Element_recursiveDestroy(element->right, destFunc); 
	//fprintf(stderr,"calling dest element func:%p for element data:%p\n",dest_func,element);
	//if (dest_func == free)
	//	fprintf(stderr,"using standard free function\n");
	destFunc(element->data);  //Use destroy function
	//fprintf(stderr,"call ok, now freeing element\n");	
	free(element);	//free current element
	//fprintf(stderr,"freed ok\n");
}

/* Element Function Definitions */

Blah_Tree_Element *Blah_Tree_Element_new(char *key, void *data) {
	//Creates a new tree element 
	Blah_Tree_Element *newElement = malloc(sizeof(Blah_Tree_Element));
	newElement->left = newElement->right = NULL;
	blah_util_strncpy(newElement->keystring, key, BLAH_TREE_ELEMENT_NAME_LENGTH);
	newElement->data = data;
	
	return newElement;
}

static Blah_Tree_Element **Blah_Tree_Element_findPosition(Blah_Tree_Element **beginAddr, char *key) {
	//Returns address of pointer where element should be located in tree
	Blah_Tree_Element *tempElement = *(Blah_Tree_Element **)beginAddr;
	Blah_Tree_Element **positionAddr = beginAddr;
	blah_bool found = BLAH_FALSE;
	int stringComp;
	
	while (tempElement && !found) {
		stringComp = strcmp(key, tempElement->keystring);
		if (!stringComp) { //check if key strings match
			found = BLAH_TRUE;  //set found flag to true
		}
		else {
			if (stringComp < 0) { //if element should go left of current element
				positionAddr = &tempElement->left;
				tempElement = tempElement->left; //navigate left
			} else {
				positionAddr = &tempElement->right;					
				tempElement = tempElement->right; //navigate right
			}
		}
	}
	
	return positionAddr;
}


void Blah_Tree_Element_callFunction(Blah_Tree_Element *element, blah_tree_element_func function) { 
	//call function with data pointer of element
	function(element->data);
}

void Blah_Tree_Element_callWithArg(Blah_Tree_Element *element, blah_tree_element_func_1arg function, void *arg) { 
	//call function with data pointer of element and single void pointer arg
	function(element->data, arg);
}

blah_bool Blah_Tree_Element_callArgReturnBool(Blah_Tree_Element *element, blah_tree_element_bool_func_1arg function, void *arg) { 
	//call function for with data pointer of element
	return function(element->data, arg);
}

void *Blah_Tree_Element_search(Blah_Tree_Element *treeElement,
	blah_tree_search_func *searchFunction, void *arg) {
	//Performs a linear recursive search from location of given element in tree.
	//Calls search_function for each element of the tree in sort order, using the
	//element's data as the argument and 'arg' as a second argument.  Returns the data
	//pointer of the first element for which search_function returns BLAH_TRUE, or
	//NULL if no match;
	if (treeElement->left)
		return Blah_Tree_Element_search(treeElement->left, searchFunction, arg);
	else {
		if (Blah_Tree_Element_callArgReturnBool(treeElement, searchFunction, arg))
			return treeElement->data;
		else if (treeElement->right)
			return Blah_Tree_Element_search(treeElement->right, searchFunction, arg);
		else
			return NULL;
	}
}	
		
		
/* Tree Function Definitions */

void Blah_Tree_init(Blah_Tree *tree, char *name) {
	tree->first = NULL;
	tree->count=0;
	blah_util_strncpy(tree->name, name, BLAH_TREE_NAME_LENGTH);  //set name property
	tree->destroyElementFunction = NULL;
}

Blah_Tree *Blah_Tree_new(char *name) { //Creates a new empty tree given name
	Blah_Tree *newTree = malloc(sizeof(Blah_Tree));
	Blah_Tree_init(newTree, name);
	return newTree;
}

Blah_Tree_Element *Blah_Tree_findElement(Blah_Tree *tree, char *key) {
	//finds and returns a pointer to the element structure holding data pointer
	//or NULL if no element found with matching key string
	Blah_Tree_Element **elementPtrAddr;
	
	elementPtrAddr = Blah_Tree_Element_findPosition(&tree->first, key);

	return *elementPtrAddr;
}

void *Blah_Tree_search(Blah_Tree *tree, blah_tree_search_func *searchFunction, void *arg) {
	//Calls search_function for each element of the tree in sort order, using the
	//element's data as the argument.  Returns the data pointer of the first element
	//for which search_function returns BLAH_TRUE, or NULL if no match;

	/* Simply return value from recursive element search function */
	return Blah_Tree_Element_search(tree->first, searchFunction, arg);
}


blah_bool Blah_Tree_removeElement(Blah_Tree *tree, char *key) { 
	//remove given tree element from tree.  Does not destroy data
	Blah_Tree_Element **elementPtrAddr;
	Blah_Tree_Element *removeMe;
	Blah_Tree_Element **newPtrAddr;
	
	elementPtrAddr = Blah_Tree_Element_findPosition(&tree->first, key);
	removeMe = *elementPtrAddr;
	
	if (removeMe) { //if a matching element was found
		if (!removeMe->left) //if there is no left element attached
			*elementPtrAddr = removeMe->right; //link parent with right element
		else if (!removeMe->right) //if there is no right element
			*elementPtrAddr = removeMe->left; //link parent with left element
		else { //deal with both elements
			*elementPtrAddr = removeMe->left; //link parent with left element
			newPtrAddr = Blah_Tree_Element_findPosition(elementPtrAddr, removeMe->right->keystring);
			*newPtrAddr = removeMe->right;	//Insert right element at appropriate position
		}
		tree->count--;
		return BLAH_TRUE;
	} else
		return BLAH_FALSE; //return false because remove failed
}

void Blah_Tree_removeAll(Blah_Tree *tree) {
	//removes all elements but retains empty tree structure.  Does not free data
	Blah_Tree_Element_recursiveRemove(tree->first); //call free on all element pointers
	tree->first = NULL;
	tree->count = 0;
}	

void Blah_Tree_destroyElements(Blah_Tree *tree) { 
	//clears all memory allocated for elements and data but does not destroy basic tree header
	if (tree->first) {
		blah_tree_element_dest_func *destFunc = tree->destroyElementFunction ?
			tree->destroyElementFunction : free;
		//If there is a valid destory function, we will use it, else we will just use free()
		Blah_Tree_Element_recursiveDestroy(tree->first, destFunc); //call free on all element pointers
		tree->first = NULL;
		tree->count = 0;
	}
}

void Blah_Tree_destroy(Blah_Tree *tree) { 
	//clears all memory allocated for elements, tree header and contained data
	Blah_Tree_destroyElements(tree);	//remove all elements and data
	free(tree);	//clear the tree itself
}

blah_bool Blah_Tree_insertElement(Blah_Tree *tree, char *key, void *data) {
	//inserts a new element with given key and data pointer
	//Returns TRUE on success, or FALSE if an element with same key already exists

	Blah_Tree_Element **newPtrAddr = Blah_Tree_Element_findPosition(&tree->first, key);
	if (*newPtrAddr) //If search found existing element,
		return BLAH_FALSE;
	else {
		*newPtrAddr = Blah_Tree_Element_new(key, data);
		tree->count++;  //Create new tree element structure and insert into pointer
		return BLAH_TRUE;
	}
}


void Blah_Tree_callFunction(Blah_Tree *tree, void *function) { 
	//call function for with data pointer for every element
	Blah_Tree_Element_recursiveCall(tree->first, function);
}

void Blah_Tree_callWithArg(Blah_Tree *tree, void *function, void *arg) { 
	Blah_Tree_Element_recursiveCallWithArg(tree->first, function, arg);
}
