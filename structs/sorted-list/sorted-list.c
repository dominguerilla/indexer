//JIMMY LE          jl1415
//CARLOS DOMINGUEZ  cmd363

/*
 * sorted-list.c
 */

#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"sorted-list.h"

//NEW FUNCTIONS
int addLL(Node* head, CompareFuncT cf, Node* n) {
	Node *x, *y;
	
	x = (Node*)head->next;
	y = head;

	while(1) {
		if(x == 0) {
			y->next = n;
			return 1;
		}
		if((cf)(x->dPtr,n->dPtr) < 0) {
			y->next = n;
			n->next = x;
			return 1;
		}
		y = x;
		x = x->next;
	}
}

int removeLL(Node* head, CompareFuncT cf, void* newObj) {
	Node *x, *y;
	x = (Node*)head->next;
	y = head;
	
	while(1) {
		if(x == 0) {
			return 0;
		}
		if((cf)(x->dPtr,newObj) == 0) {
			y->next = y->next;
			if(x->next!=0) {
				((Node*)(x->next))->ref++;
			}

			x->ref--;
			if(x->ref==0) {
				if(x->next!=0) {
					((Node*)(x->next))->ref--;
				}
				free(x);
			}
			return 1;
		}
		y = x;
		x = x->next;
	}
}

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df) {
	SortedListPtr list = malloc(sizeof(struct SortedList));
	memset(list,0,sizeof(struct SortedList));
	list->cf = cf;
	list->df = df;
	return list;
}

void SLDestroy(SortedListPtr list) {
	if (list == NULL){
		return;
	}

	Node* node;
	node = list->head;

	while(node != NULL){
		Node* n_next;
		if(node->next != NULL){
			n_next = node->next;
		}
		list->head->df(node);
		node = node->next;
	}
	free(list);
}

int SLInsert(SortedListPtr list, void* newObj) {
	Node* n = malloc(sizeof(Node));
	n->next = 0;
	n->ref = 1;
	n->dPtr = newObj;
	list->nItem++;
	if(list->head == 0) { /* there is nothing in the list! */
		list->head = n;
		return 1;
	}
	if((list->cf)((list->head)->dPtr,newObj) < 0) { /* the new item deserves to be king! */
		n->next = list->head;
		list->head = n;
		return 1;
	}
	if(list->nItem==2) { /* only one node in the list */
		(list->head)->next = n;
		return 1;
	}
	return addLL(list->head,list->cf,n);
}

int SLRemove(SortedListPtr list, void* newObj) {
	Node* n;
	if((list->cf)((list->head)->dPtr,newObj) == 0) {
		(list->head)->ref--;
		n = list->head;
		list->head = (list->head)->next;
		if(n->ref==0) {
			free(n);
		}
		return 1;
	}
	return removeLL(list->head,list->cf,newObj);
}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
	SortedListIteratorPtr ret;
	ret = malloc(sizeof(struct SortedListIterator));
	ret->item = list->head;
	(list->head)->ref++;
	return ret;
}

void SLDestroyIterator(SortedListIteratorPtr iter) {
	free(iter);
}

void* SLGetItem(SortedListIteratorPtr iter) {
	void* ret = 0;
	Node* n;

	int f0 = 0; 

	if(iter->item == 0) {
		return ret;
	}

	n = iter->item;
	ret = (iter->item)->dPtr;

	if((iter->item)->ref == 1) {
		f0 = 1;
	}
	iter->item = (iter->item)->next;
	n->ref--;

	if(f0) {
		free(n);
	}

	if(iter->item != 0) {
		(iter->item)->ref++;
	}

	return ret;
}

/*int sizeOfList(SortedListPtr list){
	int size = 0;
	SortedListPtr ptr = list;
	while(ptr != NULL){
		size++;
		ptr = ptr->next;
	}
	return size;
}*/

