#include <stdlib.h>
#include <stdio.h>
#include "Llist.h"

void test_pushpop() {
	Llist **list = malloc(sizeof(Llist *));
	int i;
	int *el;
	for (i = 0; i < 10; i++) {
		el = malloc(sizeof(int));
		*el = i;
		push(list, el);
		printf("pushed %d\n", *el);
	}
	// *(int*)pop(list) = casts the pointer returned
	// by pop() to an int pointer and dereferences it
	printf("pop %d\n", *(int*)pop(list));
	printf("pop %d\n", *(int*)pop(list));
	printf("pop %d\n", *(int*)pop(list));
	for (i = 0; i < 20; i++) {
		el = malloc(sizeof(int));
		*el = i;
		push(list, el);
		printf("pushed %d\n", *el);
	}
	printf("pop %d\n", *(int*)pop(list));
	printf("pop %d\n", *(int*)pop(list));
	printf("pop %d\n", *(int*)pop(list));
	printf("pop %d\n", *(int*)pop(list));
	printf("pop %d\n", *(int*)pop(list));
}

int main(int argc, char *argv[]) {
	test_pushpop();
	return 0;
}
