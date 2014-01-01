#include <stdio.h>

int main (int argc, char *argv[])
{
	int i = 0;

	// Go through each string in argv
	for (; i < argc; i++) {
		printf("arg %d : %s\n", i, argv[i]);
	}

	// Let's make our own array of strings
	char *states[] = {
		"California", "Oregon",
		"Washington", NULL
	};
	// We can modify a given argument
	argv[1] = states[0];
	// Or use it to modify a variable
	states[3] = argv[1];
	int num_states = 4;
	for (i = 0; i < num_states; i++) {
		printf("state %d : %s\n", i, states[i]);
	}

	return 0;
}
