#include <stdio.h>

int main (int argc, char *argv[])
{
	if (argc != 2) {
		printf("Error : You need one argument");
		return 1;
	}
	
	int i = 0;
	char *letter = argv[1];
	for (; *letter != '\0'; letter++) {
		//char **letter = argv;	
		switch (*letter) {
			case 'a':
			case 'A':
				printf("%d:'A'\n", letter-argv[1]);
				break;
			case 'e':
			case 'E':
				printf("%d:'E'\n", letter-argv[1]);
				break;
			case 'i':
			case 'I':
				printf("%d:'I'\n", letter-argv[1]);
				break;
			case 'o':
			case 'O':
				printf("%d:'O'\n", letter-argv[1]);
				break;
			case 'u':
			case 'U':
				printf("%d:'U'\n", letter-argv[1]);
				break;
			case 'y':
			case 'Y':
				printf("%d:'Y'\n", letter-argv[1]);
				break;
			default:
				printf("%c is not a vowel.\n", *letter);
		}
	}
	return 0;
}
