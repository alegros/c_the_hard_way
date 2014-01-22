#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


#define DOC_MAXROWS 50
#define DOC_NT_SIZE 50
#define DOC_ARGS_SIZE 100
#define DOC_SDESC_SIZE 100
#define DOC_DESC_SIZE 500

typedef struct Doc {
	int id;
	int set;
	char *type;
	char *name;
	char *args;
	char *sdesc; // Short description
	char *desc;
} Doc; // Function documentation

typedef struct Database {
	Doc *rows;
	int maxrows;
	int nt_size; // Size of name and type
	int args_size;
	int sdesc_size;
	int desc_size;
} Database;

typedef struct Connection {
	Database *db;
	FILE *file;
	
} Connection;


static Connection *conn;
static char *filename = "/home/alex/c_the_hard_way/cref/cref.dat";


void
die (const char *msg) {
	// TODO : free the connection
	if (errno) {
		perror(msg);
	} else {
		printf("%s\n", msg);
	}
	exit(1);
}

void
Doc_print (Doc *row) {
	printf("%d\t%s\t%s\t%s\t%s\t%s\n", 
		row->id, row->name, row->args, row->type, row->sdesc, row->desc);
}

void
Database_create (int *db_params, int param_cnt) { // Allocates memory for the database
	Database *db = conn->db;
	Doc *row;

	printf("%d parameters given\n", param_cnt);
	
	db->maxrows = DOC_MAXROWS;
	db->nt_size = DOC_NT_SIZE;
	db->args_size = DOC_ARGS_SIZE;
	db->sdesc_size = DOC_SDESC_SIZE;
	db->desc_size = DOC_DESC_SIZE;
	db->rows = malloc(sizeof(Doc) * db->maxrows);
	if (!db->rows) {
		die("Memory error");
	}
	
	int i;
	for (row = db->rows, i = 0; row < db->rows + db->maxrows; row++, i++) {
		row->id = i;
		row->set = 0;
		row->name = malloc(db->nt_size);
		if (!row->name) {
			die("Memory error");
		}
		row->type = malloc(db->nt_size);
		if (!row->type) {
			die("Memory error");
		}
		row->args = malloc(db->args_size);
		if (!row->args) {
			die("Memory error");
		}
		row->sdesc = malloc(db->sdesc_size);
		if (!row->sdesc) {
			die("Memory error");
		}
		row->desc = malloc(db->desc_size);
		if (!row->desc) {
			die("Memory error");
		}
	}
}

void
Database_load() {
	Database *db = conn->db;
	FILE *f = conn->file;
	//Doc *row;

	if (
	fread(&db->maxrows, sizeof(db->maxrows), 1, f) != 1
	|| fread(&db->nt_size, sizeof(db->nt_size), 1, f) != 1
	|| fread(&db->args_size, sizeof(db->args_size), 1, f) != 1
	|| fread(&db->sdesc_size, sizeof(db->sdesc_size), 1, f) != 1
	|| fread(&db->desc_size, sizeof(db->desc_size), 1, f) != 1)
	{
		die("Read error");
	};
}

void
Database_write() {
	Database *db = conn->db;
	FILE *f = conn->file;
	//Doc *row;

	rewind(conn->file);
	
	if (
	fwrite(&db->maxrows, sizeof(db->maxrows), 1, f) != 1
	|| fwrite(&db->nt_size, sizeof(db->nt_size), 1, f) != 1
	|| fwrite(&db->args_size, sizeof(db->args_size), 1, f) != 1
	|| fwrite(&db->sdesc_size, sizeof(db->sdesc_size), 1, f) != 1
	|| fwrite(&db->desc_size, sizeof(db->desc_size), 1, f) != 1)
	{
		die("Write error");
	}

	//for (row = db->rows; row < db->rows + db->maxrows; row++) {
	//	fwrite(
	//}

	if (fflush(conn->file) == -1) {
		die("Cannot flush the database");
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
		Database_load();
	}

	if (!conn->file) {
		die("Memory error");
	}
}

void
Database_list() {
	Doc *row;
	Database *db = conn->db;
	
	for (row = db->rows; row < db->rows + db->maxrows; row++) {
		Doc_print(row);
	}
}

void Database_info () {
	Database *db = conn->db;
	
	if (fread(&db->maxrows, sizeof(db->maxrows), 1, conn->file) != 1) {
		die("Read error");
	}
	printf("%d rows\n", db->maxrows);

}

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
					free(conn->db->rows[i]->name);
					free(conn->db->rows[i]->type);
					free(conn->db->rows[i]->args);
					free(conn->db->rows[i]->sdesc);
					free(conn->db->rows[i]->desc);
					free(conn->db->rows[i]);
				}
			}
			free(db);
		}
	}
}

int main(int argc, char **argv)
{	
	char cmd;
	int *db_params; // Args to parameterize fields sizes
	int param_cnt = 0;

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
				// TODO : Handle error if parameter is not numeric
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
	default:
		die("Invalid command");
	}

	Database_close();
	return 0;
}

