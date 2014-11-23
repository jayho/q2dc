/*
 * Copyright 2005 Jens Hoffmann
 * 
 * This file is part of q2dc.
 * 
 * q2dc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * 
 * Foobar is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with Foobar. If not, see
 * http://www.gnu.org/licenses/.
 */

/*
 * dm2convert.c
 *
 * The main file with contains the 'main' function and the main loop 
 * that iterates over blocks in the source demo file, translates them
 * and writes them to the target demo file
 */

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
	fprintf(stdout, "q2dc (c) j. hoffmann, 2005, v0.1\n");
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
	fprintf(stderr,"Usage: %s [-h] [-f] [-o output_filename] input_filename\n", gProgname);
	fprintf(stderr,	"\n"
			"Options:\n" 
			"  -f     fix block lengths (prevents the MAX_MSGLEN error)\n"
			"  -o     use 'output_filename'. If omitted, the output filename\n"
			"         will be the original filename with suffix '_34'\n"
			"  -h     prints this message...for more info see Readme.txt\n");
	exit(0);
}

/* EOF */
