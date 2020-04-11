/* blah_region.c 
	Region related stuff */
	
#include "blah_region.h"

/* Function Declarations */

void Blah_Region_init(Blah_Region *region, int left, int bottom, int right, int top) {
	region->left = left;
	region->right = right;
	region->bottom = bottom;
	region->top = top;
}
