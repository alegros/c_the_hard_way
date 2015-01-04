#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"
#include <dlfcn.h>

typedef int (*lib_function)(const char *data, int length);
typedef int (*type_fail_on_purpose)(const char *data);

/*
int main (int argc, char *argv[])
{
	int rc = 0;
	check(argc == 4, "USAGE: ex29 libex29.so function data");
	
	char *libfile = argv[1];
	char *func_to_run = argv[2];
	char *data = argv[3];
	
	void *lib = dlopen(libfile, RTLD_NOW);
	check(lib != NULL, "Failed to open library %s: %s", libfile, dlerror());
	
	lib_function func = dlsym(lib, func_to_run);
	check(func != NULL, "Did not find function %s in library %s: %s", func_to_run, libfile, dlerror());
	
	int length = 0;
	while (*data++ != '\0') length++;
	printf("length:%d\n", length);
	data = data - ((length + 1) * sizeof(char));
	rc = func(data, length);
	check(rc == 0, "Function %s returned %d for data: %s", func_to_run, rc, data);
	
	rc = dlclose(lib);
	check(rc == 0, "Failed to close library: %s", libfile);
	
	return 0;
error:
	return 1;
}
*/
int main ()
{
	char *libfile = "./build/libex29.so";
	char *printmsg = "print_a_message";
	char *uppercase = "to_upper_case";
	char *lowercase = "to_lower_case";
	char *fail = "fail_on_purpose";
	char *message = "";
	int i;
	int j = 1;

	void *libhandle = dlopen(libfile, RTLD_NOW);
	check(libhandle != NULL, "Error opening file %s : %s\n", libfile, dlerror());

	lib_function func = dlsym(libhandle, "print_a_message");
	check(func != NULL, "Cannot access function %s in object file %s : %s\n", printmsg, libfile, dlerror());

	func = NULL;
	func = dlsym(libhandle, uppercase);
	check(func != NULL, "Cannot access function %s in object file %s : %s\n", uppercase, libfile, dlerror());
	message = "message en majuscules";
	for (i = 0; message[i] != '\0'; i++);
	printf("%d\t", j++);
	check (func(message, i) == 0, "Error in %s\n", uppercase);

	func = NULL;
	func = dlsym(libhandle, lowercase);
	check(func != NULL, "Cannot access function %s in object file %s : %s\n", lowercase, libfile, dlerror());
	message = "MESSAGE EN MINUSCULES";
	for (i = 0; message[i] != '\0'; i++);
	printf("%d\t", j++);
	check(func(message, i) == 0, "Error in %s\n", lowercase);

	type_fail_on_purpose fail_on_purpose = NULL;
	fail_on_purpose = dlsym(libhandle, fail);
	check(fail_on_purpose != NULL, "Cannot access function %s in object file %s : %s\n", fail, libfile, dlerror());
	printf("%d\tfail_on_purpose\n", j++);
	check(fail_on_purpose(message) == 1, "Error in %s\n", fail);

	dlclose(libhandle);
	return 0;
error:
	return 1;
}
