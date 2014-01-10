#include <stdio.h>
#include <stdlib.h>

struct Stack {
	int data;
	struct Stack *next; // The element that will be pop'd NEXT
};

void push(struct Stack **head, int value) {
	struct Stack *node = malloc(sizeof(struct Stack));
	if (!node) {
		printf("Memory error, aborting...\n");
		exit(1);
	}
	node->data = value;
	node->next = *head;	
	*head = node;
}

int pop(struct Stack **head) {
	if (!*head) {
		printf("No element to pop, aborting...\n");
		exit(1);
	}
	struct Stack *top = *head; //Get a new pointer to the head element
	int value = top->data; //Save its data into the variable to return
	*head = top->next; //Head becomes the pointer that it previously pointed at
	free(top); //We now free top as we saved its data in value/
	return value;
}

int main(int argc, char *argv[]) {
	struct Stack **list = malloc(sizeof(char *));
	push(list, 33);
	push(list, 22);
	push(list, 4);
	printf("pop %d\n", pop(list));
	printf("pop %d\n", pop(list));
	push(list, 267);
	printf("pop %d\n", pop(list));
	printf("pop %d\n", pop(list));
	printf("pop %d\n", pop(list)); // Empty stack
	return 0;
}
