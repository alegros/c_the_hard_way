#include <stdio.h>
#include <ctype.h>
#include "dbg.h"

int print_a_message (const char *msg)
{
	printf("A string : %s\n", msg);
    
	return 0;
}

int to_upper_case (const char *msg, int length)
{
	int i;
	
	for (i = 0; i < length; i++) {
		printf("%c", toupper(msg[i]));
	}
	
	printf("\n");
	
	return 0;
}

int to_lower_case (const char *msg, int length)
{
	int i;
	
	for (i = 0; i < length; i++) {
		printf("%c", tolower(msg[i]));
	}
	
	printf("\n");
	
	return 0;
}

int fail_on_purpose (const char *msg)
{
	return 1;
}
