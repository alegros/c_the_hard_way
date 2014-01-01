#include <stdio.h>
int main (int argc, char *argv[])
{
	printf("%d\n", argc);
	int i = argc-1;
	int nb_states = 4;
	char *states[] = {
		"California", "Montana",
		"Ohio", "New York"
	};
	while (i != 0) {
		printf("%s\n", argv[i]);
		i--;
	}
	i = 0;
	while (i < argc-1 && i < nb_states) {
		states[i] = argv[i+1];
		i++;
	}
	i = 0;
	while (i < 4) {
		printf("%s\n", states[i]);
		i++;
	}

	return 0;
}
