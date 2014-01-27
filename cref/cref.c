#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


#define DOC_MAXROWS 1000
#define DOC_NTL_SIZE 100
#define DOC_ARGS_SIZE 200
#define DOC_SDESC_SIZE 100
#define DOC_DESC_SIZE 500

typedef struct Doc {
	int id;
	int set;
	char *library;
	char *type;
	char *name;
	char *args;
	char *sdesc; // Short description
	char *desc;
} Doc; // Function documentation

typedef struct Database {
	int maxrows;
	int ntl_size; // Size of name, type and library
	int args_size;
	int sdesc_size;
	int desc_size;
	Doc *rows;
} Database;

typedef struct Connection {
	Database *db;
	FILE *file;
	
} Connection;


static Connection *conn;
static char *filename = "/home/alex/c_the_hard_way/cref/cref.dat";


void 
Database_close() {
	int i;

	if (conn) {
		if (conn->file) {
			fclose(conn->file);
		}
		if (conn->db) {
			if (conn->db->rows) {
				for (i = 0; i < conn->db->maxrows; i++) {
					free(conn->db->rows[i].name);
					free(conn->db->rows[i].type);
					free(conn->db->rows[i].args);
					free(conn->db->rows[i].sdesc);
					free(conn->db->rows[i].desc);
					free(conn->db->rows[i].library);
				}
			}
			free(conn->db);
		}
	}
	free(conn);
}

void
die (const char *msg) {
	Database_close();

	if (errno) {
		perror(msg);
	} else {
		printf("%s\n", msg);
	}
	exit(1);
}

void 
Database_info () {
	Database *db = conn->db;
	
	printf("%d rows\n", db->maxrows);
	printf("Size of name, type and library : %d\n", db->ntl_size);
	printf("Size of args : %d\n", db->args_size);
	printf("Size of short description : %d\n", db->sdesc_size);
	printf("Size of long description : %d\n", db->desc_size);
}

void
Doc_print (Doc *row) {
	printf("%d\t%s\t%s\t%s\t%s\t%s\t%s\n", 
		row->id, row->library, row->type, row->name, row->args, row->sdesc, row->desc);
}

void
Doc_set(int id, const char *library, const char *name, const char *type,
		const char *args, const char *sdesc, const char *desc) {
	Database *db = conn->db;
	Doc *d = &conn->db->rows[id-1];

	if (d->set) {
		die("Already set, delete it first");
	}

	d->set = 1;
	char *res = strncpy(d->library, library, db->ntl_size);
	if (!res) {
		die("String copy error");
	}
	res = strncpy(d->name, name, db->ntl_size);
	if (!res) {
		die("String copy error");
	}
	res = strncpy(d->type, type, db->ntl_size);
	if (!res) {
		die("String copy error");
	}
	res = strncpy(d->args, args, db->args_size);
	if (!res) {
		die("String copy error");
	}
	res = strncpy(d->sdesc, sdesc, db->sdesc_size);
	if (!res) {
		die("String copy error");
	}
	res = strncpy(d->desc, desc, db->desc_size);
	if (!res) {
		die("String copy error");
	}
}

void
Database_create (int *db_params, int param_cnt) { // Allocates memory for the database
	Database *db = conn->db;

	db->maxrows = DOC_MAXROWS;
	db->ntl_size = DOC_NTL_SIZE;
	db->args_size = DOC_ARGS_SIZE;
	db->sdesc_size = DOC_SDESC_SIZE;
	db->desc_size = DOC_DESC_SIZE;

	printf("\nInitializing database with :\n");
	Database_info();
	printf("\n...\n");

	db->rows = malloc(sizeof(Doc) * db->maxrows);
	if (!db->rows) {
		die("Memory error");
	}
	
	int i;
	for (i = 0; i < db->maxrows; i++) {
		db->rows[i].id = i;
		db->rows[i].set = 0;
		db->rows[i].name = malloc(db->ntl_size);
		if (!db->rows[i].name) {
			die("Memory error");
		}
		db->rows[i].type = malloc(db->ntl_size);
		if (!db->rows[i].type) {
			die("Memory error");
		}
		db->rows[i].library = malloc(db->ntl_size);
		if (!db->rows[i].library) {
			die("Memory error");
		}
		db->rows[i].args = malloc(db->args_size);
		if (!db->rows[i].args) {
			die("Memory error");
		}
		db->rows[i].sdesc = malloc(db->sdesc_size);
		if (!db->rows[i].sdesc) {
			die("Memory error");
		}
		db->rows[i].desc = malloc(db->desc_size);
		if (!db->rows[i].desc) {
			die("Memory error");
		}
	}
	printf("Done\n");
}

void
Database_write() {
	Database *db = conn->db;
	Doc *row;
	FILE *f = conn->file;

	rewind(f);
	
	if (
	fwrite(&db->maxrows, sizeof(db->maxrows), 1, f) != 1
	|| fwrite(&db->ntl_size, sizeof(db->ntl_size), 1, f) != 1
	|| fwrite(&db->args_size, sizeof(db->args_size), 1, f) != 1
	|| fwrite(&db->sdesc_size, sizeof(db->sdesc_size), 1, f) != 1
	|| fwrite(&db->desc_size, sizeof(db->desc_size), 1, f) != 1)
	{
		die("Write error");
	}

	for (row = db->rows; row < db->rows + db->maxrows; ++row) {
		if (
		fwrite(&row->id, sizeof(row->id), 1, f) != 1
		|| fwrite(&row->set, sizeof(row->set), 1, f) != 1
		|| fwrite(row->library, db->ntl_size, 1, f) !=1
		|| fwrite(row->type, db->ntl_size, 1, f) !=1
		|| fwrite(row->name, db->ntl_size, 1, f) !=1
		|| fwrite(row->args, db->args_size, 1, f) !=1
		|| fwrite(row->sdesc, db->sdesc_size, 1, f) !=1
		|| fwrite(row->desc, db->desc_size, 1, f) !=1)
		{
			die("Write error");
		}
	}

	if (fflush(f) == -1) {
		die("Cannot flush the database");
	}
}

void
Database_list() {
	Doc *row;
	Database *db = conn->db;
	
	for (row = db->rows; row < db->rows + db->maxrows; row++) {
		if (row->set) {
			Doc_print(row);
		}
	}
}

void
Database_load() {
	Database *db = conn->db;
	Doc *row;

	if (
	fread(&db->maxrows, sizeof(db->maxrows), 1, conn->file) != 1
	|| fread(&db->ntl_size, sizeof(db->ntl_size), 1, conn->file) != 1
	|| fread(&db->args_size, sizeof(db->args_size), 1, conn->file) != 1
	|| fread(&db->sdesc_size, sizeof(db->sdesc_size), 1, conn->file) != 1
	|| fread(&db->desc_size, sizeof(db->desc_size), 1, conn->file) != 1)
	{
		die("Load error");
	}
	
	db->rows = malloc(sizeof(struct Doc) * db->maxrows);
	if (!db->rows) {
		die("Memory error");
	}

	for (row = db->rows; row < db->rows + db->maxrows; ++row) {
		row->library = malloc(db->ntl_size);
		row->name = malloc(db->ntl_size);
		row->type = malloc(db->ntl_size);
		row->args = malloc(db->args_size);
		row->sdesc = malloc(db->sdesc_size);
		row->desc = malloc(db->desc_size);
		if (
		!row->name
		|| !row->type
		|| !row->library
		|| !row->args
		|| !row->sdesc
		|| !row->desc)
		{
			die("Memory error");
		}

		if (
		fread(&row->id, sizeof(row->id), 1, conn->file) != 1
		|| fread(&row->set, sizeof(row->set), 1, conn->file) != 1
		|| fread(row->library, db->ntl_size, 1, conn->file) != 1
		|| fread(row->type, db->ntl_size, 1, conn->file) != 1
		|| fread(row->name, db->ntl_size, 1, conn->file) != 1
		|| fread(row->args, db->args_size, 1, conn->file) != 1
		|| fread(row->sdesc, db->sdesc_size, 1, conn->file) != 1
		|| fread(row->desc, db->desc_size, 1, conn->file) != 1)
		{
			die("Load error");
		}
	}
}

void
Database_open(char cmd) {
	conn = malloc(sizeof(Connection));
	if (!conn) {
		die("Memory error");
	}
	conn->db = malloc(sizeof(Database));
	if (!conn->db) {
		die("Memory error");
	}

	if (cmd == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
		if (conn->file) {
			Database_load();
		}
	}

	if (!conn->file) {
		die("Memory error");
	}
}

int main(int argc, char **argv)
{	
	char cmd;
	int *db_params; // Args to parameterize fields sizes
	int param_cnt = 0;
	int id;

	if (argc > 1) {
		cmd = argv[1][0];
	} else {
		die("Usage : cref [command]");
	}

	Database_open(cmd);

	switch (cmd) {
	case 'c':
		if (argc == 2) {
			printf("\nYou can give more parameters in the following order :\ncref c [Number of rows] [Size of name and type] [Size of arguments] [Size of short description] [Size of long description]\nArguments are optional but their order must remain the same.\n\n");
		} else if (argc > 2) {
			db_params = malloc(sizeof(int) * argc-2);
			for (param_cnt = 2; param_cnt <= argc; param_cnt++, db_params++) {
				// TODO : Check that id is a number
				*db_params = atoi(argv[param_cnt]);
			}
			param_cnt -= 2; // Keep track of how many parameters were passed
		}
		Database_create(db_params, param_cnt);
		Database_write();
		break;
	case 'i':
		Database_info();
		break;
	case 'l':
		Database_list();
		break;
	case 's':
		if (argc == 9) {
			// TODO : Check that id is a number
			id = atoi(argv[2]);
			Doc_set(id, argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
		} else {
			printf("Pass arguments in the following order :\n[id] [Library] [Type] [Name] [Args] [Sdesc] [Desc]\n");
		}
		Database_write();
		break;
	default:
		die("Invalid command");
	}

	Database_close();
	return 0;
}
