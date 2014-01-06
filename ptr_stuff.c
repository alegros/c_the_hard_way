#include <stdio.h>
#include <stdlib.h>

void func1() {
	char *s = "hello";
	printf("%c ", *s); // h
	printf("%c ", *(s+4)); // o
	printf("%c ", *s); // h
	s++;
	printf("%s ", s); // hello
	printf("\n\n");
}

void func2() {
	char *m[] = {"abc", "def", "ghi"};
	printf("%c ", **m); // a
	printf("%c ", **(m+2)); // g
	printf("%c ", *(*m+1)); // b
	printf("%c ", *(*(m+1)+2)); // f
	printf("\n\n");
}

void func3(char *argv[]) {
	printf("adresse de argv : %p\n", argv);
	printf("premier argument : %s\n", *argv);
	printf("deuxième argument : %s\n", *(argv+1));
	argv++;

	printf("début zone mémoire du deuxième argument : %c\n", **argv); 

	/* afficher un à un les caratère du deuxième argument */
	for(; **argv != '\0'; printf("%c ", *(*argv)++));
	printf("\n\n");

}

void func4 ()
{
	char *s[] = {"let's do it", "hope it works", "maybe...", "oh god why..."};
	printf("Let's play with an array of strings, aka an array of characters arrays (s)\n");
	printf("Our array : {\"let\'s do it\", \"hope it works\", \"maybe...\", \"oh god why...\"}\n");
	printf("**s : %c\n", **s);
	printf("*s : %s\n", *s);
	char **p = s; 
	printf("Arrays are immutable. We will use a pointer (p) so we can incremente it\n");
	// Mieux vaut utiliser la syntaxe [] avec les tableaux
	printf("p++ : %s\n", *(++p)); // get the value pointed at by '++p'
	printf("\n\n");
}

int main (int argc, char *argv[])
{
	if (argc != 2) {
		printf("Need an argument");
		return 1;
	}
	func1();
	func2();
	func3(argv);
	func4();
	return 0;
}
