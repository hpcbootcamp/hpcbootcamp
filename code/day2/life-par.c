#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {

  char *fname;
  FILE *fd;
  int xlimit, ylimit, iterations, iter;
  short *board1_data = NULL, *board2_data = NULL;
  short **b1_rows, **b2_rows;
  short **oldb = NULL, **newb = NULL, **tempb;
  int i, j, x, y, count;
  

  /* read the command line */
  if (argc != 5) {
    fprintf(stderr, "life <input file name> <# of generations> <X_limit> <Y_limit>\n");
    exit(-1);
      }

  fname = argv[1];
  if (sscanf(argv[2], "%d", &iterations) != 1) {
    fprintf(stderr, "# generations argument is not an integer\n");
    exit(-1);
  }
  if (sscanf(argv[3], "%d", &xlimit) != 1) {
    fprintf(stderr, "X_limit argument is not an integer\n");
    exit(-1);
  }
  if (sscanf(argv[4], "%d", &ylimit) != 1) {
    fprintf(stderr, "Y_limit argument is not an integer\n");
    exit(-1);
  }

  if ((fd = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "Couldn't open input data file %s\n", fname);
    exit(-1);
  }
  if (xlimit < 2 || ylimit < 2 || iterations < 1) {
    fprintf(stderr, "Limits < 2 or iteration number < 1\n");
    exit(-1);
  }

  /* set up the boards - use 2, 1 for previous and 1 for current iteration,
   switching them every iteration */

  board1_data = (short *) malloc(xlimit * ylimit * sizeof(short));
  if (board1_data == NULL) {
    fprintf(stderr, "Allocation failed for board1\n");
    exit(-1);
  }
  board2_data = (short *) malloc(xlimit * ylimit * sizeof(short));
  if (board2_data == NULL) {
    fprintf(stderr, "Allocation failed for board1\n");
    exit(-1);
  }

  /* row pointers */
  b1_rows = (short **) malloc(xlimit * sizeof(short*));
  b1_rows[0] = board1_data;
  for (i = 1; i < xlimit; i++) {
    b1_rows[i] = b1_rows[i-1] + ylimit;
  }
  b2_rows = (short **) malloc(xlimit * sizeof(short*));
  b2_rows[0] = board2_data;
  for (i = 1; i < xlimit; i++) {
    b2_rows[i] = b2_rows[i-1] + ylimit;
  }

  /* initialize board1 with input data */
  newb = b1_rows;  oldb = b2_rows;
  while (fscanf(fd, "%d %d", &x, &y) != EOF) {
    if (x < 0 || x >= xlimit || y < 0 || y >= ylimit) {
      fprintf(stderr, "Input x or y value outside board: %d %d\n", x, y);
      exit(-1);
    }
    newb[x][y] = 1;
  }

  /* perform the iterations, each time switching boards */
  for (iter = 0; iter < iterations; iter++) {
    /* switch boards */
    tempb = oldb;
    oldb = newb;
    newb = tempb;
    /* compute the new board, treating first/last row/column specially */
    /* first row has no row above */
    i = 0;
    /* first column has no cell on left */
    j = 0;
    count =                  oldb[i][j+1]   +
	      oldb[i+1][j] + oldb[i+1][j+1];
    if ( (oldb[i][j] == 1 && (count == 2 | count == 3)) ||
	 (oldb[i][j] == 0 && (count == 3))) newb[i][j] = 1;
    else newb[i][j] = 0;
      
    #pragma omp parallel for private(count)
    for (j = 1; j < ylimit - 1; j++) {
      count = oldb[i][j-1]   +                oldb[i][j+1]   +
	      oldb[i+1][j-1] + oldb[i+1][j] + oldb[i+1][j+1];
      if ( (oldb[i][j] == 1 && (count == 2 | count == 3)) ||
	   (oldb[i][j] == 0 && (count == 3))) newb[i][j] = 1;
      else newb[i][j] = 0;
    }

    /* last column has no cell on right */
    j = ylimit - 1;
    count = oldb[i][j-1]   + 
	    oldb[i+1][j-1] + oldb[i+1][j];
    if ( (oldb[i][j] == 1 && (count == 2 | count == 3)) ||
	 (oldb[i][j] == 0 && (count == 3))) newb[i][j] = 1;
    else newb[i][j] = 0;
    
    /* middle rows */
    #pragma omp parallel for private(j, count)
    for (i = 1; i < xlimit - 1; i++) {
      /* first column has no cell on left */
      j = 0;
      count = oldb[i-1][j] + oldb[i-1][j+1] +
	                     oldb[i][j+1]   +
	      oldb[i+1][j] + oldb[i+1][j+1];
      if ( (oldb[i][j] == 1 && (count == 2 | count == 3)) ||
	   (oldb[i][j] == 0 && (count == 3))) newb[i][j] = 1;
      else newb[i][j] = 0;
      
      for (j = 1; j < ylimit - 1; j++) {
	count = oldb[i-1][j-1] + oldb[i-1][j] + oldb[i-1][j+1] +
	        oldb[i][j-1]   +                oldb[i][j+1]   +
	        oldb[i+1][j-1] + oldb[i+1][j] + oldb[i+1][j+1];
	if ( (oldb[i][j] == 1 && (count == 2 | count == 3)) ||
	     (oldb[i][j] == 0 && (count == 3))) newb[i][j] = 1;
	else newb[i][j] = 0;
      }

      /* last column has no cell on right */
      j = ylimit - 1;
      count = oldb[i-1][j-1] + oldb[i-1][j] +
	      oldb[i][j-1]   + 
	      oldb[i+1][j-1] + oldb[i+1][j];
      if ( (oldb[i][j] == 1 && (count == 2 | count == 3)) ||
	   (oldb[i][j] == 0 && (count == 3))) newb[i][j] = 1;
      else newb[i][j] = 0;
    }

    /* last row has no row below */
    i = xlimit - 1;
    /* first column has no cell on left */
    j = 0;
    count = oldb[i-1][j] + oldb[i-1][j+1] +
                           oldb[i][j+1];
    if ( (oldb[i][j] == 1 && (count == 2 | count == 3)) ||
	 (oldb[i][j] == 0 && (count == 3))) newb[i][j] = 1;
    else newb[i][j] = 0;
      
    #pragma omp parallel for private(count)
    for (j = 1; j < ylimit - 1; j++) {
      count = oldb[i-1][j-1] + oldb[i-1][j] + oldb[i-1][j+1] +
	      oldb[i][j-1]   +                oldb[i][j+1];
      if ( (oldb[i][j] == 1 && (count == 2 | count == 3)) ||
	   (oldb[i][j] == 0 && (count == 3))) newb[i][j] = 1;
      else newb[i][j] = 0;
    }

    /* last column has no cell on right */
    j = ylimit - 1;
    count = oldb[i-1][j-1] + oldb[i-1][j] +
	    oldb[i][j-1];
    if ( (oldb[i][j] == 1 && (count == 2 | count == 3)) ||
	 (oldb[i][j] == 0 && (count == 3))) newb[i][j] = 1;
    else newb[i][j] = 0;
    
  }

  /* print out final board */
  for (i = 0; i < xlimit; i++) {
    for (j = 0; j < ylimit; j++) {
      if (newb[i][j] == 1) {
	printf("%d %d\n", i, j);
      }
    }
  }

  exit(0);
}
