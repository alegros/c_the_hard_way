#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

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
	int db_size;
	int _size; // Size of type and name
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
		printf("ERROR : %s\n", msg);
	}
	exit(1);
}

void
Doc_print (Doc *row) {
	printf("%d\t%s\t%s\t%s\t%s\t%s\n", 
		row->id, row->name, row->args, row->type, row->sdesc, row->desc);
}

void
Database_malloc () {
	Doc *row;
	
	conn = malloc(sizeof(Connection));
	if (!conn) {
		die("Memory error");
	}
	conn->db = malloc(sizeof(Database));
	if (!conn->db) {
		die("Memory error");
	}
	
	Database *db = conn->db;
	db->db_size = 50;
	db->_size = 50;
	db->args_size = 100;
	db->sdesc_size = 100;
	db->desc_size = 500;
	db->rows = malloc(sizeof(Doc) * db->db_size);
	if (!db->rows) {
		die("Memory error");
	}
	
	int i;
	for (row = db->rows, i = 0; row < db->rows + db->db_size; row++, i++) {
		row->id = i;
		row->set = 0;
		row->name = malloc(db->_size);
		if (!row->name) {
			die("Memory error");
		}
		row->type = malloc(db->_size);
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
Database_write () {
	Database *db = conn->db;
	//Doc *row;
	
	if (
	fwrite(&db->db_size, sizeof(db->db_size), 1, conn->file) != 1
	|| fwrite(&db->_size, sizeof(db->_size), 1, conn->file) != 1
	|| fwrite(&db->args_size, sizeof(db->args_size), 1, conn->file) != 1
	|| fwrite(&db->sdesc_size, sizeof(db->sdesc_size), 1, conn->file) != 1
	|| fwrite(&db->desc_size, sizeof(db->desc_size), 1, conn->file) != 1)
	{
		die("Write error");
	}

	//for (row = db->rows; row < db->rows + db->db_size; row++) {
	//	fwrite(
	//}
}

void
Database_open(int create) {
	Connection *conn = malloc(sizeof(Connection));
	if (!conn) {
		die("Memory error");
	}
	conn->db = malloc(sizeof(Database));
	if (!conn->db) {
		die("Memory error");
	}

	if (create) {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
	}

	if (!conn->file) {
		die("Memory error");
	}
}

void
Database_list() {
	Doc *row;
	Database *db = conn->db;
	
	for (row = db->rows; row < db->rows + db->db_size; row++) {
		Doc_print(row);
	}
}

void Database_info () {
	Database *db = conn->db;
	
	if (fread(&db->_size, sizeof(db->_size), 1, conn->file) != 1) {
		die("Read error");
	}
	printf("%d rows\n", db->_size);

}

void 
Database_close() {
	if (conn->file) {
		fclose(conn->file);
	}
}

int main(int argc, char **argv)
{	
	Database_malloc();
	//Database_list();
	Database_write();
	Database_info();
	Database_close();
	return 0;
}

