#include <stdio.h>

// return 0 if EOF, else 1
int read_line(char line[])
{
	printf("(line : %p -- ", line);
	printf("*line : %d)", *line);
	char c;
	//int i = 0;
	while ((c = getchar()) != 0 && c != '\n' && c != EOF) {
		*line++ = c;
	}
	//for (i = 0; (c = getchar()) != 0 && c != '\n' && c != EOF; line[i++]=c);
	*line++ = '\n';
	*line = '\0';
	if (c == EOF)
		return 0;
	else
		return 1;
}

int main (int argc, char *argv[])
{
//	char c;
	char s[1000];
	int count = 0;
	while(read_line(s)) {
		printf("%s", s);
		// increase count only if s doesn't only contain '\n'
		if (s[0] != '\n')
			count++;
	}
	printf("----- %d lignes -----\n", count);
	return 0;
}
