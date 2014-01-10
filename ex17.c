#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
	int id;
	int set;
	char *name;
	char *email;
	char tel[10];
};

struct Database {
	int maxrows;
	int maxdata;
	struct Address *rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close(struct Connection *conn)
{
	if(conn) {
		if(conn->file) {
			fclose(conn->file);
		}
		if(conn->db) {
			if(conn->db->rows) {
				struct Address *addr;
				for(addr = conn->db->rows;
					addr < conn->db->rows + conn->db->maxrows;
					addr++)
				{
					if(addr->name) {
						free(addr->name);
					}
					if(addr->email) {
						free(addr->email);
					}
				}
				free(conn->db->rows);	
			}
			free(conn->db);	
		}
		free(conn);
	}
}

void die (struct Connection *conn, const char *message)
{
	Database_close(conn);
	if(errno) {
		perror(message);
	} else {
		printf("ERROR : %s.\n", message);
	}
	exit(1);
}

char *format_tel (char addr_tel[])
{
	int i = 0; // for addr_tel
	int j = 0; // for local tel
	int spc = (sizeof(addr_tel) / 2) - 1; // number of spaces to insert
	int tel_size = sizeof(addr_tel) + spc;
	char *tel = malloc(tel_size);

	for (;i < tel_size;) {
		if (i%2 == 0) {
			tel[j] = ' ';
			j++;
		}
		tel[j] = addr_tel[i];
		j++;
		i++;
	}
	return tel;
}

void Address_print (struct Address *addr)
{
	printf("%d %s %s %s\n",
			addr->id, addr->name, addr->email, format_tel(addr->tel));
}

void Database_load (struct Connection *conn)
{
	struct Database *db = conn->db;
	struct Address *addr;

	if (fread(&db->maxrows, sizeof(db->maxrows), 1, conn->file) != 1) {
		die(conn, "Failed to load maxrows.");
	}
	if (fread(&db->maxdata, sizeof(db->maxdata), 1, conn->file) != 1) {
		die(conn, "Failed to load maxdata.");
	}

	db->rows = malloc(db->maxrows * sizeof(struct Address));
	if(!db->rows) die(conn, "Memory error.");

	for (addr = db->rows; addr < db->rows + db->maxrows; ++addr) {
		if (fread(&addr->id, sizeof(addr->id), 1, conn->file) != 1) {
			die(conn, "Failed to load id.");
		}
		if (fread(&addr->set, sizeof(addr->set), 1, conn->file) != 1) {
			die(conn, "Failed to load set.");
		}

		addr->name = malloc(db->maxdata);
		if(!addr->name) die(conn, "Memory error (name)");
		if (fread(addr->name, db->maxdata, 1, conn->file) != 1) {
			die(conn, "Failed to load name.");
		}

		addr->email = malloc(db->maxdata);
		if(!addr->email) die(conn, "Memory error (email)");
		if (fread(addr->email, db->maxdata, 1, conn->file) != 1) {
			die(conn, "Failed to load email.");
		}

		if (fread(addr->tel, sizeof(addr->tel), 1, conn->file) != 1) {
			die(conn, "Failed to load telephone number.");
		}
	}
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if (!conn) die(conn, "Memory error");
	
	conn->db = malloc(sizeof(struct Database));
	if (!conn->db) die(conn, "Memory error");

	if(mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
		if(conn->file) {
			Database_load(conn);
		}
	}

	if(!conn->file) die(conn, "Failed to open the file");

	return conn;
}

void Database_write(struct Connection *conn)
{
	struct Database *db = conn->db;
	struct Address *addr;
	int rc;

	rewind(conn->file);

	rc = fwrite(&db->maxrows, sizeof(db->maxrows), 1, conn->file);
	if (rc != 1) die(conn, "Failed to write database");
	rc = fwrite(&db->maxdata, sizeof(db->maxdata), 1, conn->file);
	if (rc != 1) die(conn, "Failed to write database");

	for (addr = db->rows; addr < db->rows + db->maxrows; addr++) {
		if (fwrite(&addr->id, sizeof(addr->id), 1, conn->file) != 1)
			die(conn, "Failed to write database !");
		if (fwrite(&addr->set, sizeof(addr->set), 1, conn->file) != 1)
			die(conn, "Failed to write database !");
		if (fwrite(addr->name, db->maxdata, 1, conn->file) != 1)
			die(conn, "Failed to write database !");
		if (fwrite(addr->email, db->maxdata, 1, conn->file) != 1)
			die(conn, "Failed to write database !");
		if (fwrite(addr->tel, sizeof(addr->tel), 1, conn->file) != 1)
			die(conn, "Failed to write database !");
	}
	rc = fflush(conn->file);
	if (rc == -1) die(conn, "Cannot flush database");
}

void Database_create(struct Connection *conn)
{
	int i;
	struct Database *db = conn->db;
	struct Address *addr;

	db->rows = malloc(db->maxrows * sizeof(struct Address));
	if (!db->rows) die(conn, "Memory error.");

	for(i = 0; i < db->maxrows; i++) {
		addr = &db->rows[i];
		addr->id = i;
		addr->set = 0;
		addr->tel[0] = '\0';

		//addr->name = calloc(db->maxdata, sizeof(char));
		addr->name = malloc(db->maxdata);
		if(!addr->name) die(conn, "Memory error");
		addr->name[0] = '\0';

		//addr->email = calloc(db->maxdata, sizeof(char));
		addr->email = malloc(db->maxdata);
		if(!addr->email) die(conn, "Memory error.");
		addr->email[0] = '\0';
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email, const char *tel)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die(conn, "Already set, delete it first");

	addr->set = 1;
	// WARNING : buf, read the "How to break it" and fix this
	char *res = strncpy(addr->name, name, conn->db->maxdata);
	// demonstrate the strnbpy bug
	if (!res) die(conn, "Name copy failed");

	res = strncpy(addr->email, email, conn->db->maxdata);
	if(!res) die(conn, "Email copy failed");

	res = strncpy(addr->tel, tel, sizeof(addr->tel));
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(addr);
	} else {
		die(conn, "ID is not set");
	}
}

void Database_delete(struct Connection *conn, int id)
{
	conn->db->rows[id].set = 0;
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	for(i = 0; i < db->maxrows; i++) {
		struct Address *cur = &db->rows[i];
		if(cur->set) {
			Address_print(cur);
		}
	}
}

void Database_find (struct Connection *conn, char *s)
{
	struct Database *db = conn->db;
	struct Address *addr;
	for (addr = db->rows; addr < db->rows + db->maxrows; addr++) {
		if (addr->set && *addr->name == *s) {
			Address_print(addr);
			return;
		}
	}
	printf("%s non trouvé\n", s);
}

void Database_info (struct Connection *conn)
{
	struct Database *db = conn->db;
	printf("max rows : %d\n", db->maxrows);
	printf("data size : %d (name, email)\n", db->maxdata);
}

void test_struct ()
{
	printf("Adresse:%lu\n", sizeof(struct Address));
	exit(0);
}

int main (int argc, char *argv[])
{
	if(argc < 3) die(NULL, "USAGE : ex17 <dbfile> <action> [action params] ");

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id;

	if ((action == 'g' || action == 's' || action == 'd')
		&& argc > 3)
	{
		id = atoi(argv[3]);
		if (id > conn->db->maxrows) {
			die(conn, "There is not that many records");
		}
	}

	switch(action) {
		case 'f':
			if (argc != 4) die(conn, "Need a string to search for");
			Database_find(conn, argv[3]);
			break;
		case 'c':
			if (argc == 3) {
				conn->db->maxrows = MAX_ROWS;
				conn->db->maxdata = MAX_DATA;
			} else if (argc == 4) {
				conn->db->maxrows = atoi(argv[3]);
				conn->db->maxdata = MAX_DATA;
			} else {
				conn->db->maxrows = atoi(argv[3]);
				conn->db->maxdata = atoi(argv[4]);
			}
			Database_create(conn);
			Database_write(conn);
			break;
		case 'g':
			if(argc < 4) die(conn, "Need an id to get");
			Database_get(conn, id);
			break;
		case 's':
			if(argc < 7) die(conn, "Need id, name, email and telephone number to set");
			Database_set(conn, id, argv[4], argv[5], argv[6]);
			Database_write(conn);
			break;
		case 'd':
			if(argc < 4) die(conn, "Need id to delete");
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		case 'i':
			Database_info(conn);
			break;
		default:
			die(conn, "Invalid action, only : c = create, g = get, s = set, d = del, l = list");
	}

	Database_close(conn);

	return 0;
}
