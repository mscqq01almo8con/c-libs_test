#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

#define X 1

int Nx, Ny, Nz;
int width, height, depth;
int i; /* width */
int j; /* depth */
int k; /* height */
int totalSteps;

int parseCmdLineArgs(int, char**);
int offset(int, int, int);
double dRand(double dMin, double dMax);
void computeJacobi(double*);
void computePhysics(double*);

int main(int argc, char* argv[]) {

	double *data = NULL;
	double *results = NULL;
	int steps = 0;

	if (parseCmdLineArgs(argc, argv) == 1) {
		return 1;
	}

	width = Nx; // x
	depth = Ny; // y
	height = Nz; // z

	printf("\nN = %d nodes.\n", Nx * Ny * Nz);
	printf("\nC = %d iterations.\n", totalSteps);

	srand(time(NULL));
	data = calloc((width + 4) * (depth + 4) * (height + 2), sizeof(double));
	results = calloc((width + 4) * (depth + 4) * (height + 2), sizeof(double));
	for (i = 2; i < width + 2; i++) {
		for (j = 2; j < depth + 2; j++) {
			for (k = 1; k < height + 1; k++) {
				data[offset(i, j, k)] = dRand(10, 1000);
			}
		}
	}

	double t1, t2;

	MPI_Init(NULL, NULL);
	t1 = MPI_Wtime();
	while (steps < totalSteps) {
		steps++;
		computeJacobi(data);
		computePhysics(results);
	}
	t2 = MPI_Wtime();
	MPI_Finalize();

	/* elapsed time */
	printf("\nElapsed time: Dt = %.3f msec.\n\n", (t2 - t1) * 1000);

	return 0;
}

int offset(int x, int y, int z) {
	return z + ((height + 2) * y) + ((height + 2) * (depth + 4) * x);
}

/* compute finite difference */
void computeJacobi(double *data) {
	for (i = 2; i < width + 2; i++) {
		for (j = 2; j < depth + 2; j++) {
			for (k = 1; k < height + 1; k++) {
				data[offset(i, j, k)] = (4 * data[offset(i, j, k)]
						+ data[offset(i - 1, j, k)] + data[offset(i, j - 1, k)]
						+ data[offset(i - 2, j, k)] + data[offset(i, j - 2, k)]
						+ data[offset(i + 1, j, k)] + data[offset(i, j + 1, k)]
						+ data[offset(i + 2, j, k)] + data[offset(i, j + 2, k)]
						+ data[offset(i, j, k - 1)] + data[offset(i, j, k + 1)])
						/ 14;
			}
		}
	}
}

void computePhysics(double *results) {
	for (i = 2; i < width + 2; i++) {
		for (j = 2; j < width + 2; j++) {
			results[offset(i, j, 1)] = pow(X, 10);
			for (k = 2; k < height + 1; k++) {
				results[offset(i, j, k)] = pow(
						0.9 * results[offset(i, j, k - 1)], 10);
			}
		}
	}
}

/* generate random doubles between 2 values */
double dRand(double dMin, double dMax) {
	double d = (double) rand() / RAND_MAX;
	return dMin + d * (dMax - dMin);
}

int parseCmdLineArgs(int argc, char **argv) {
	if (argv[1] != NULL && strcmp(argv[1], "-nodes") == 0) {
		if (argv[2] != NULL && argv[3] != NULL && argv[4] != NULL) {
			Nx = atoi(argv[2]);
			Ny = atoi(argv[3]);
			Nz = atoi(argv[4]);
		} else {
			printf("\nMust specify grid of nodes.\n\n");
			return 1;
		}
	} else {
		printf("\nMust specify grid of nodes.\n\n");
		return 1;
	}

	/* specify number of iterations */
	if (argv[5] != NULL && strcmp(argv[5], "-steps") == 0) {
		if (argv[6] != NULL) {
			totalSteps = atoi(argv[6]);
		} else {
			printf("\nSpecify number of iterations.\n\n");
			return 1;
		}
	} else {
		printf("\nSpecify number of iterations.\n\n");
		return 1;
	}
	return 0;
}
