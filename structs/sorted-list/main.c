//JIMMY LE          jl1415
//CARLOS DOMINGUEZ  cmd363
/*
 * main.c
 */

#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include 	<ctype.h>
#include	<errno.h>

#include	"sorted-list.h"

/*
 * Specific Testing Compares:
 */
int compareInt(void * first, void * second) {
	int num1 = *(int*)first;
	int num2 = *(int*)second;
	return num1 - num2;
}

int compareStrings(void * first, void * second) {
	char *string1 = first;
	char *string2 = second;
	return strcmp(string1, string2);
}

/*
 * Main testing Compare Function:

int compareMain(void* first, void* second){
	SortedListPtr node1 = (SortedListPtr) first;
	SortedListPtr node2 = (SortedListPtr) second;
	int num1 = *((int*)node1->data);
	int num2 = *((int*)node2->data);

	if(num1 < num2) return -1;
	if(num1 == num2) return 0;
	return 1;
}*/


void destroy(void* list){
	free(list);
}

int main(int argc, char **argv) {
	int x, *y, *z;
	y = malloc(sizeof(int));
	
	//Checks if the user enters the right amount of inputs - only 1 input at a time
	if(argc > 1){
		printf("Invalid Number of Input\n");
		exit(EXIT_FAILURE);
	}

	SortedListPtr list = SLCreate(compareInt, destroy);
	SortedListIteratorPtr iter;

	while(scanf("%d",&x)==1) {
		z = malloc(sizeof(int));
		*z = x;
		SLInsert(list,z);
	}

	iter = SLCreateIterator(list);

	while(1) {
		y = SLGetItem(iter);
		if(y == 0) {
			break;
		}
		printf("%d\n",*y);
	}
	return 0;
}
