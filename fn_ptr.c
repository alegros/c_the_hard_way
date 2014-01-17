#include <stdio.h>
#include <stdlib.h>

/* Let's say we need a struct to represent a tile in a
turn based strategy game*/
typedef struct Tile {
	char type[20];
	int x, y;
	int bonus;
	fn desc;
} Tile;
typedef void (*fn)(Tile *t);


void fn1()
{
	printf("Exécution fonction 1\n");
}

void fn2()
{
	printf("Exécution fonction 2\n");
}

void fn3()
{
	printf("Exécution fonction 3\n");
}

void Tile_desc(Tile *t)
{
	printf("(%d;%d) %s [%d]", t->x, t->y, t->type, t->bonus);
}

int main(int argc, char* argv[])
{
	//fn *ptr_func = malloc(sizeof(fn) * 2);
	//*ptr_func = fn1;
	//*(ptr_func + 1) = fn2;
	//*(ptr_func + 2) = fn3;
	//(*ptr_func)();
	//(*(ptr_func + 1))();
	//(*(ptr_func + 2))();
	//printf("...\n");

	Tile *myTile = malloc(sizeof(Tile));
	myTile->desc = malloc(sizeof(fn));
	myTile->desc = &Tile_desc;
	myTile->desc(myTile);
	return 0;
}
