#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

int *test_varargs(int nbArgs, ...)
{
	va_list pa;
	int *args = malloc(sizeof(int) * nbArgs);
	int i;

	va_start(pa, nbArgs);
	for (i = 0; i < nbArgs; i++) {
		args[i] = va_arg(pa, int);
	}
	va_end(pa);

	return args;
}

int main(int argc, char **argv)
{
	int i;
	int nbArgs = 3;

	int *args = test_varargs(nbArgs, 76, 46, 33, 94, 36, 3463);

	for (i = 0; i < nbArgs; i++) {
		printf("Argument %d : %d\n", i+1, args[i]);
	}

	return 0;
}
