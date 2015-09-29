#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

void die(int errorcode, char *message) {
	fprintf(stderr, "ERROR: %s", message);
	exit(errorcode); 
}	

void warn(char *message) {
	fprintf(stderr, "WARNING: %s\n", message);
	/*getchar();*/
}

#ifdef DEBUG
void debug(char *message) {
	fprintf(stderr, "%s", message);
}
#else
void debug(char *message) {}
#endif
