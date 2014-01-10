#include <stdio.h>
#include <stdlib.h>

// A stack using an array

struct Stack {
	int size;
	int current;
	int *array;
};

void init(struct Stack *st) {
	st->current = -1;
	if (!st->size) {
		st->size = 1;
	}
	st->array = malloc(st->size * sizeof(st->size));
}

void push(struct Stack *st, int value) {
	if (st->current > st->size) {
		printf("Stack overflow, aborting...\n");
		exit(1);
	}
	st->array[++st->current] = value;
}

int pop(struct Stack *st) {
	if (st->current < 0) {
		printf("Stack is empty, aborting...\n");
		exit(1);
	}
	return st->array[st->current--];
}

int main(int argc, char *argv[]) {
	struct Stack *st = malloc(sizeof(struct Stack));
	st->size = 10;
	init(st);
	push(st, 3);
	push(st, 456);
	push(st, 300);
	push(st, 8);
	push(st, 14912);
	printf("Top : %d\n", pop(st));
	printf("Top : %d\n", pop(st));
	printf("Top : %d\n", pop(st));
	push(st, 33);
	printf("Top : %d\n", pop(st));
	printf("Top : %d\n", pop(st));
	printf("Top : %d\n", pop(st));
	printf("Top : %d\n", pop(st)); // Error: empty stack
	return 0;
}
