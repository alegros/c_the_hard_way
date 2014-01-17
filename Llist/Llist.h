#ifndef _Llist_h
#define _Llist_h

typedef struct Llist {
	void *el;
	struct Llist *next; // The element that will be pop'd NEXT
} Llist;

void push(Llist **head, void *el);
void *pop(Llist **head);

#endif
