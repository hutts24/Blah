#include <malloc.h>
#include <string.h>
#include <stdio.h>


#include "blah_list.h"
#include "blah_types.h"
#include "blah_macros.h"
#include "blah_util.h"

/* Element Function Definitions */
Blah_List_Element *Blah_List_Element_new(void *data)
{	//Creates a new list element.  Returns a pointer to newly created element
	//on success, or NULL pointer if error occurred.
	Blah_List_Element *newElement = malloc(sizeof(Blah_List_Element));

	if (newElement) //Check that memory allocation succeeded
	{
		Blah_List_Element_init(newElement, data); //Initialise new structure
	}

	return newElement;
}

void Blah_List_Element_init(Blah_List_Element *element, void *data)
{	//Initialises given element structure
	element->prev=element->next=NULL;
	element->data=data;
}

void Blah_List_Element_callFunction(Blah_List_Element* element, blah_list_element_func* function) {
	//call function for with data pointer of element
	function(element->data);
}

void Blah_List_Element_callWithArg(Blah_List_Element* element, blah_list_element_func_1arg* function, void* arg) {
	//call function for with data pointer of element
	function(element->data,arg);
}

bool Blah_List_Element_callArgReturnBool(Blah_List_Element* element, blah_list_element_bool_func_1arg* function, void* arg) {
	//call function for with data pointer of element
	return function(element->data,arg);
}

/* List Function Definitions */

void Blah_List_init(Blah_List *list, const char *name)
{	//Sets the name of the list, and all element pointers to NULL
	list->first = NULL;
	list->last = NULL;
	list->length = 0;
	blah_util_strncpy(list->name, name, BLAH_LIST_NAME_LENGTH);  //set name property
	list->destroyElementFunction = NULL;
}

Blah_List *Blah_List_new(const char *name)
{	//Creates a new empty list given a name as a null terminated string in parameter 'name'.
	//Function returns pointer to new list on success, or NULL pointer if error occurred.
	Blah_List *newList = malloc(sizeof(Blah_List));
	// TODO - error handling
	if (newList != NULL) { Blah_List_init(newList, name); }  //If structure creation succeeded, initialise new structure
	return newList;
}

Blah_List_Element *Blah_List_findElement(const Blah_List *list, const void *data) { //finds and returns a pointer to the element structure holding data pointer
	Blah_List_Element *tempElement = list->first;
	bool found = false;

	while (tempElement!=NULL && !found) {
		if (tempElement->data == data) { //check if data pointers match
			found = true;
		}
		else {
			tempElement=tempElement->next;
		}
	}
	return tempElement;
}

bool Blah_List_removeElement(Blah_List *list, void *data) {
	//remove given list element from liste.  Does not destroy data
	Blah_List_Element *tempElement = Blah_List_findElement(list, data);

	if (tempElement) { //if a matching element was found
		//Deal with previous link
		if (tempElement->prev==NULL)  //if current element is first in the list
			list->first=tempElement->next;		//second element now becomes first
		else
			tempElement->prev->next=tempElement->next; //else link prev with next
		//Deal with next link
		if (tempElement->next!=NULL)
			tempElement->next->prev=tempElement->prev; //next element gets prev link from current element
		else //if removing last element of list, update the last element pointer
			list->last=tempElement->prev;
		list->length--;
		return true;	//removed matching element
	} else
		return false;	//no match was found
}

/* Function Blah_List_pop_element
	Removes first element from list and returns data pointer */
void *Blah_List_popElement(Blah_List *list) {
	Blah_List_Element *tempElement = list->first;
	void *tempData = NULL;	//assume no data

	if (tempElement) { //if first element points to a valid element structure
		tempData = tempElement->data;
		list->first = tempElement->next; //make next element first element
		if (list->first) //If new first element pointer is valid
			list->first->prev = NULL; //Set prev pointer of new first element to NULL
		free(tempElement);				//Once list is reconstructed, free element structure
		list->length--;
	}

	return tempData; //return data pointer, NULL or valid
}

void Blah_List_removeAll(Blah_List *list) {
	//removes all elements but retains empty list structure.  Does not free data
	Blah_List_Element *tempElement = list->first, *freeElement;

	while (tempElement!=NULL) {
		freeElement = tempElement;  //remember current pointer
		tempElement = tempElement->next;  //prepare for next element
		free(freeElement);	//free current element
	}
	list->first=list->last=NULL;
	list->length=0;
}

// clears all memory allocated for elements and data but does not destroy basic list header
void Blah_List_destroyElements(Blah_List *list) {
	Blah_List_Element *tempElement = list->first;
	blah_list_element_dest_func* destFunc = list->destroyElementFunction ? list->destroyElementFunction : free;
	//If there is a valid destory function, we will use it, else we will just use free()

	while (tempElement != NULL) {
		Blah_List_Element *destElement = tempElement;  //remember current pointer
		tempElement = tempElement->next;  //prepare for next element
		destFunc(destElement->data); //Call destroy function to free/destroy data
		free(destElement);	//free current element
	}
	list->first = list->last = NULL;  //clear list to empty
	list->length = 0;
}


void Blah_List_destroy(Blah_List *list) { //clears all memory allocated for elements, list header and contained data
	Blah_List_destroyElements(list);	//remove all elements and data
	free(list);	//clear the list itself
}

void Blah_List_appendElement(Blah_List *list, void *data) { //adds a new element with given data pointer
	Blah_List_Element *newElement = Blah_List_Element_new(data);
	//Deal with previous link
	if (list->first==NULL) //if list is empty
		list->first=list->last=newElement;
	else {//last element is guaranteed to be existent
		list->last->next=newElement;  //link last to new
		newElement->prev=list->last;	//link new to last
		list->last=newElement;			//make new last
	}
	list->length++;
}

void Blah_List_insertElement(Blah_List *list, void *data) { //inserts a new element with given data pointer
	Blah_List_Element *newElement = Blah_List_Element_new(data);
	//Deal with previous link
	if (list->first==NULL) //if list is empty
		list->first=list->last=newElement;
	else {//first element is guaranteed to be existent
		newElement->next=list->first;  //link first element to new element
		list->first->prev=newElement;  //link new to original first
		list->first=newElement;		//make first element pointer of list point to new element
	}
	list->length++;
}


void Blah_List_callFunction(Blah_List* list, blah_list_element_func* function) {
	//call function for with data pointer for every element
	Blah_List_Element *currentElement=list->first, *nextElement;

	while (currentElement) {
		nextElement = currentElement->next;
		//grab the next element in case function destroys current element
		Blah_List_Element_callFunction(currentElement, function);
		currentElement = nextElement;
	}
}

void Blah_List_callWithArg(Blah_List* list, blah_list_element_func_1arg* function, void* arg) {
	//call function for with data pointer and single argument for every element
	Blah_List_Element *currentElement=list->first, *nextElement;

	while (currentElement) {
		nextElement = currentElement->next;
		//grab the next element in case function destroys current element
		Blah_List_Element_callWithArg(currentElement, function, arg);
		currentElement = nextElement;
	}
}

void *Blah_List_search(Blah_List* list, blah_list_search_func* searchFunction, void* searchArg) {
	//Calls search_function for each element of the list, using the element's data
	//as the first argument, and 'arg' as second argument.  Returns the data pointer
	//of the first element for which search_function returns true, or NULL if no
	//match

	//call function for with data pointer and single argument for every element
	Blah_List_Element *currentElement=list->first, *nextElement, *match = NULL;

	while (currentElement && !match) {
		nextElement = currentElement->next;
		//grab the next element in case function destroys current element
		if (Blah_List_Element_callArgReturnBool(currentElement, searchFunction, searchArg))
			match = currentElement->data;
		else
			currentElement = nextElement;
	}

	return match;
}

void Blah_List_sort(Blah_List* list, blah_list_sort_func* compareFunction) {
	//Performs a linear sort of the list, using the supplied function to compare two elements.
	//compare_function is called as comp((void*)elem1->data,(void*)elem2->data).
	//compare_function should behave like strcmp() and return a value smaller than 0
	//if elem1->data < elem2->data, return 0 if equal, or > 0 if elem1->data > elem2->data
	Blah_List_Element *currentElement, *nextElement, *compareElement;
	bool placeFound;

	if (list->first) { //Make sure list isn't empty before continuing
		currentElement = list->first->next; //We want to start comparing second elem
		if (currentElement) { //Don't continue if there isn't a second element!
			while (currentElement) { //Process each element in order until end of list
				nextElement = currentElement->next; //hold pointer to next element
				compareElement = currentElement->prev;
				placeFound = false;
				while (compareElement && !placeFound) {
					if (compareFunction(currentElement->data, compareElement->data) < 0)
						compareElement = compareElement->prev; //step backwards
					else
						placeFound = true;
				}
				if (compareElement != currentElement->prev) {
				/* If last compared element is not the previous in the chain,
					then we need to shift the element along in the list to where it should be */

					//remove current_element from list and relink
					currentElement->prev->next = currentElement->next;
					if (currentElement->next)
						currentElement->next->prev = currentElement->prev;
					else //We must be moving the last element in the list
						list->last=currentElement->prev; //fix pointer to last element

					if (!compareElement) { //Reinsert element at beginning of list
						currentElement->next=list->first;  //link first element to current element
						list->first->prev=currentElement;  //link current to original first
						list->first=currentElement;		//make first element pointer of list point to new element
						currentElement->prev=NULL;
					} else { //Reinsert in appropriate position
						//fix link to next element
						compareElement->next->prev = currentElement;
						currentElement->next = compareElement->next;
						//fix link to prev element
						compareElement->next = currentElement;
						currentElement->prev = compareElement;
					}
				}
				currentElement = nextElement; //Begin processing next element
			}
		}
	}
}




void Blah_List_setDestroyElementFunction(Blah_List* list, blah_list_element_dest_func* function) {
	//Sets the function pointer to the given function used to destroy element data
	//contained in elements of this list
	list->destroyElementFunction = function;
}


blah_pointerstring Blah_List_createPointerstring(Blah_List *list) {
	//Returns an allocated array of pointers to the data contained in each of the
	//list elements, in the order they occur.  The last element of the return array
	//is a NULL pointer to signify the end.
	void **newPointerArray = malloc(sizeof(void*) * (list->length+1));
	Blah_List_Element *tempElement = list->first;
	unsigned int index =0;
	//Allocate memory and allow one extra element for NULL pointer
	while (tempElement) {
		newPointerArray[index] = tempElement->data;
		index++;
		tempElement = tempElement->next;
	}
	return newPointerArray;
}
