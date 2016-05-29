#include "mpi.h" /*using mpi to measure time.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

			/*Editable defined vals are indented.*/
			#define NX 64
			#define NY 64
			#define NZ 20

/*Don't really need to edit the rest defined vals.*/
#define M 100 /*number of loop iterations.*/
#define npsDENOM (4+8+2)
#define Radiation_Factor 1-0.1
#define Physics_Power 10

double x[NX][NY][NZ],z[NX][NY][NZ];
void init_data(double x[][NY][NZ]);
void XY_plane_finite_dif(double x[][NY][NZ]);
void physics(double z[][NY][NZ]);

int main(int argc,char *argv[])
{
		int i;
		double T_init=0.0,T_final=0.0,T_comp,T_c;
		MPI_Init(NULL,NULL);
    init_data(x);
    
    //starting timer.
		T_init=MPI_Wtime();

		//commencing iterations
	  for(i=0;i<M;i++)
	  {
	      XY_plane_finite_dif(x);
	      physics(z);
	  }
	  //stop timer.
		T_final=MPI_Wtime();
		MPI_Finalize();
		
		//calculating time
		T_comp=(T_final-T_init)*1000.0;
		T_c=T_comp/(double)(NX*NY*NZ);
	  printf("\nSequential program total elapsed time:\nT_comp=%.2lf, \
	  	NX=%d,NY=%d,NX*NY*NZ=%d,T_c=%.4lf\nMPI libs used - 1 process defined.\n",T_comp,NX,NY,NX*NY*NZ,T_c);
  	printf("(T_comp->only as an indication)\n");
  	
    return 0;
}

//initialize 3D 'double' matrix 'x'
void init_data(double x[][NY][NZ])
{	int i,j,k;
	srand(time(NULL));
	for(i=0;i<NX;i++)
		for(j=0;j<NY;j++)
			for(k=0;k<NZ;k++)
				x[i][j][k]=(rand()%(int)10E6)*10E-4;	
}

//finite-difference stencil computation
void XY_plane_finite_dif(double x[][NY][NZ])
{
	int i,j,k;
	for(i=2;i<NX-2;i++)
		for(j=2;j<NY-2;j++)
			for(k=1;k<NZ-1;k++)	  		
				x[i][j][k]=( 4*x[i][j][k]+
				x[i-1][j][k]+x[i-2][j][k]+x[i][j-1][k]+x[i][j-2][k]+
				x[i][j+1][k]+x[i][j+2][k]+x[i+1][j][k]+x[i+2][j][k]+
				x[i][j][k+1]+x[i][j][k-1])/npsDENOM;	
}

//physics computation
void physics(double z[][NY][NZ])
{
	int i,j,k;
	for(i=2;i<NX-2;i++)
		for(j=2;j<NY-2;j++)
		{
			z[i][j][NZ-1]=1.00;//getting 100% of radiation. lower parts get less and less
			for (k=NZ-2;k>=0;k--)
				z[i][j][k]=pow( (z[i][j][k+1] * Radiation_Factor ),Physics_Power);
		}	
}

