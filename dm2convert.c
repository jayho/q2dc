#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"
#include "tools.h"
#include "block.h"
#include "dm2.h"
#include "dm2convert.h"

int main(int argc, char **argv) {
	int opt;
	unsigned long blocks;
	char *fname_out = NULL;
	DM2FILE_T f_in, f_out;

	strcpy(gProgname, argv[0]);
	
	fprintf(stdout, "\nquake2 demo converter for r1q2 demos\n");
	fprintf(stdout, "\n");
	
	/* Check, get and set options/names etc... */
	if (argc < 2) 
		syntax();
	while ((opt = getopt(argc, argv, "fho:")) != -1) {
        	switch(opt) {
        		case 'f':
				gFixBlocks = 1;
				break;

			case 'o':
				fname_out = optarg;
				break;

			case 'h':
			case '?':
				syntax();
		}
	}

	if (!argv[optind]) 
		syntax();
	strncpy(f_in.name, argv[optind], FILENAME_MAX-3);
	if (!fname_out) {
		strncpy(f_out.name, f_in.name, strlen(f_in.name)+1);
		strcat(f_out.name, "_34");
	} else
		strncpy(f_out.name, fname_out, FILENAME_MAX);	
	
	/* Open files, fetch file info and init input file */
	if (openDM2(&f_in, "rb") == 0)
		die(-1, "Could not open input file");
	if ((blocks = checkDM2(&f_in)) != 0)
		fprintf(stderr, "%s: %li blocks\n", f_in.name, blocks);
	else
		die(-1, "Bad DM2 input file format");
	fprintf(stderr, "Hit RETURN to convert %s -> %s", f_in.name, f_out.name);
	if (gFixBlocks != 0)
		fprintf(stderr, " (fix block lengths)");
	else
		fprintf(stderr, " (preserve block lengths)");
	getchar();	
	if (openDM2(&f_out, "wb") == 0)
		die(-1, "Could not open output file");
		
	/* Process files */
	if (processDM2(&f_in, &f_out) == 0)
		die(-1, "Error during conversion");
	
	closeDM2(&f_in);
	closeDM2(&f_out);
	
	/* Final check */
	openDM2(&f_out, "rb"); 
	if ((blocks = checkDM2(&f_out)) != 0)
		fprintf(stderr, "%s: %li frames\n", f_out.name, blocks);
	else
		die(-1, "Output file seems to be corrupt");

	fprintf(stderr, "Done");
	closeDM2(&f_out);

	exit(0);
}

void syntax() {
	fprintf(stderr,"Usage: %s [-h] [-f] [-o output.dm2] input.dm2\n", gProgname);
	fprintf(stderr,	"\n"
			"Options:\n" 
			"  -f     fix block lengths (MAX_MSGLEN error)\n"
			"  -o     use specified filename as output (if empty: demo.dm2_34)\n"
			"  -h     prints usage information, see also README.md\n");
	exit(0);
}
