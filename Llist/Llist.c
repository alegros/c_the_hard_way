#include <stdio.h> 
#include <stdlib.h>
#include "Llist.h"

/* TODO :
- Make "data" a pointer to have a list of any type
- void init_from_array()
- Make functions part of the struct
- put tests in another file
- Rename Llist to List or something
*/

void push(struct Llist **head, void *el) {
	Llist *node = malloc(sizeof(struct Llist));
	if (!node) {
		printf("Memory error, aborting...\n");
		exit(1);
	}
	node->el = el;
	node->next = *head;	
	*head = node;
}

void *pop(struct Llist **head) {
	if (!*head) {
		printf("No element to pop, aborting...\n");
		exit(1);
	}
	//Get a new pointer to the head element
	Llist *top = *head;
	//Save its data into the variable to return
	void *el = top->el;
	//Head becomes the pointer that it previously pointed at
	*head = top->next;
	//We now free top as we saved its data in value
	free(top);
	return el;
}
