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
};

struct Database {
	struct Address *rows;
	int maxdata;
	int maxrows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close(struct Connection *conn)
{
	if(conn) {
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
		free(conn);
	}
}

void die (struct Connection *conn, const char *message)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR : %s.\n", message);
	}
	if(conn) Database_close(conn);
	exit(1);
}

void Address_print (struct Address *addr)
{
	printf("%d %s %s\n",
			addr->id, addr->name, addr->email);
}

void Database_load (struct Connection *conn)
{
	struct Database *db = conn->db;
	struct Address *addr;

	int rc = fread(&db->maxrows, sizeof(db->maxrows), 1, conn->file);
	if (rc != 1) die(conn, "Failed to load database.");
	rc = fread(&db->maxdata, sizeof(db->maxdata), 1, conn->file);
	if (rc != 1) die(conn, "Failed to load database.");
	db->rows = malloc(db->maxrows * sizeof(struct Address));
	if(!db->rows) die(conn, "Memory error.");
	for (addr = db->rows; addr < db->rows + db->maxrows; addr++) {
		rc = fread(&addr->id, sizeof(addr->id), 1, conn->file);
		if (rc != 1) die(conn, "Failed to load database.");
		rc = fread(&addr->set, sizeof(addr->set), 1, conn->file);
		if (rc != 1) die(conn, "Failed to load database.");

		addr->name = malloc(db->maxdata);
		rc = fread(addr->name, sizeof(addr->name), 1, conn->file);
		if (rc != 1) die(conn, "Failed to load database.");
		addr->email = malloc(db->maxdata);
		rc = fread(addr->email, sizeof(addr->email), 1, conn->file);
		if (rc != 1) die(conn, "Failed to load database.");
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
	rewind(conn->file);
	int rc = fwrite(&db->maxrows, sizeof(db->maxrows), 1, conn->file);
	if (rc != 1) die(conn, "Failed to write database");
	rc = fwrite(&db->maxdata, sizeof(db->maxdata), 1, conn->file);
	if (rc != 1) die(conn, "Failed to write database");
	rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) die(conn, "Failed to write database");

	rc = fflush(conn->file);
	if (rc == -1) die(conn, "Cannot flush database");
}

void Database_create(struct Connection *conn)
{
	int i;
	struct Database *db = conn->db;
	struct Address addr;

	db->rows = malloc(db->maxrows * sizeof(struct Address));
	if (!conn->db->rows) die(conn, "Memory error.");

	for(i = 0; i < db->maxrows; i++) {
		addr = db->rows[i];
		addr.id = i;
		addr.set = 0;

		addr.name = malloc(db->maxdata);
		if(!addr.name) die(conn, "Memory error");
		addr.name[0] = '\0';

		addr.email = malloc(db->maxdata);
		if(!addr.email) die(conn, "Memory error.");
		addr.email[0] = '\0';
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die(conn, "Already set, delete it first");

	addr->set = 1;
	// WARNING : buf, read the "How to break it" and fix this
	char *res = strncpy(addr->name, name, MAX_DATA);
	// demonstrate the strnbpy bug
	if (!res) die(conn, "Name copy failed");

	res = strncpy(addr->email, email, MAX_DATA);
	if(!res) die(conn, "Email copy failed");
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
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_update(struct Connection *conn, int id, const char *name, const char *email)
{
	Database_delete(conn, id);
	Database_set(conn, id, name, email);
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	for(i = 0; i < MAX_ROWS; i++) {
		struct Address *cur = &db->rows[i];
		if(cur->set) {
			Address_print(cur);
		}
	}
}

int main (int argc, char *argv[])
{
	if(argc < 3) die(NULL, "USAGE : ex17 <dbfile> <action> [action params] ");

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	//if (argc > 3) id = atoi(argv[3]);
	//if (id >= MAX_ROWS) die(conn, "There is not that many records");

	switch(action) {
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
			if(argc != 4) die(conn, "Need an id to get");
			Database_get(conn, id);
			break;
		case 's':
			if(argc != 6) die(conn, "Need id, name, email to set");
			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if(argc != 4) die(conn, "Need id to delete");
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		case 'u':
			Database_update(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		default:
			die(conn, "Invalid action, only : c = create, g = get, s = set, d = del, l = list");
	}

	Database_close(conn);

	return 0;
}
