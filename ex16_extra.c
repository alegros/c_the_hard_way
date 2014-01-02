#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
	char *name;
	int age;
	int height;
	int weight;
};

struct Person Person_create (char *name, int age, int height, int weight)
{
	struct Person who;
	who.name = strdup(name);
	who.age = age;
	who.height = height;
	who.weight = weight;
	
	return who;
}

//void Person_destroy (struct Person who)
//{
//	assert (who != NULL);
//	free(who.name);
//	free(who);
//}

void Person_print (struct Person who)
{
	printf("Name : %s\n", who.name);
	printf("Age : %d\n", who.age);
	printf("Height : %d\n", who.height);
	printf("Weight : %d\n", who.weight);
}

int main (int grac, char *argv[])
{
	struct Person joe;
	joe = Person_create("Joe", 20, 70, 140);
	Person_print(joe);

	return 0;
}
