#include "mpi.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>// may have to be compiled with -lm
#include <omp.h>

/*	Editable variable definitions are the indented ones.		*/

				//	cartesian Process grid 2d sizes 
				//	CARTESIAN_X*CARTESIAN_Y --> must be equal to number of 
				//	processes we have defined to run our program.
				//	e.g. for 32 processes we set (CARTESIAN_X=4)*(CARTESIAN_Y=4) = 32 Processes
				#define CARTESIAN_X 4
				#define CARTESIAN_Y 4

				/*====DATA GRID DIMENSIONS====*/
				#define NX 64
				#define NY 64
				#define NZ 20

				#define REDUCE_FREQUENCY 100 //reduction frequency.
				#define SETREDUCE 0 //reduce (1) or not reduce (0) ?
				/*====we set this to 1 if we want to use OpenMP directives throughout our code.====*/
				#define USE_OpenMP 0

/*====LOCAL ARRAY DIMENSIONS====*/
#define L 2+(int)ceil((double)NX/(double)CARTESIAN_X)+2 
#define W 2+(int)ceil((double)NY/(double)CARTESIAN_Y)+2
#define H NZ

/*====defining each process's neighbors ====*/
#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

/*==== Horizontal 9-Point-Stencil and Vertical Physics constants ====*/
#define Physics_Power 10	
#define npsDENOM 14						//nine-point-stencil denominator 
#define Radiation_Factor 1-0.1 //each height z gets this percentage of radiation from upward neightbor
#define mass_overall 2.0E10 //defining a total mass here.
#define M 100			//total computation loop steps.

#define overall 0 //later we make a a vector of various timers.
#define sendrecv 1
#define indep 2
#define dep 3
#define waitrecv 4
#define waitsend 5
#define reduce 6
#define comp 0
#define comm 1
#define total 2
void setup();
void initHData(double x[][W][H],int rank);

//XY plane finite difference part, communication-independent
void XY_plane_finite_dif_indp(double x[][W][H]);

//XY plane finite difference part, communication-independent
void XY_plane_finite_dif_dep(double x[][W][H]);

//XY physics part, communication-independent
void physics_indp(double z[][W][H]);

//XY physics part, communication-dependent
void physics_dep(double z[][W][H]);

int periods[2]={1,1}, reorder=1, /*periodic grid of processes  -
		 															p[0,0] can talk to p[N-1,N-1] and so on.*/
		coords[2],					/* coordinates of a process, e.g. p[2,3] - 2nd row, 3rd col */
		count=3,						/* 3-dimensional Data grid. useful when commiting 3D data grid sections
		 										for communication between processes*/
		starts[3],					/* the origin upon which we select the datatype section we wish.*/
		subsizes[3],				/* length,width,and height of this datatype section*/
		bigsizes[3]={L,W,H};/*    >>   >>    >>   >>   of the whole 3D data array*/
	MPI_Comm myCartComm;	/*defining a Cartesian communicator for our Cartesian process grid.*/
	
	MPI_Datatype LEFT_segment_SEND,LEFT_segment_RCV,/*we send/receive well-defined datatype sections*/
				RIGHT_segment_SEND,RIGHT_segment_RCV,
				UP_segment_SEND,UP_segment_RCV,
				DOWN_segment_SEND,DOWN_segment_RCV;

int main(int argc,char *argv[])  {
	int  i,j,k;
	int numtasks					/* # of tasks. */, 	rank/*process rank*/, 
		source							/* any source neighbor process(Up,Down,Left,Right) */,
		dest								/* any destination neighbor process(Up,Down,Left,Right). */,
		tag=1, 							/*just setting tag=1 for all messages. (simplicity) */
		nbrs[4],					/*the neighboring processes (Up,Down,Left,Right)*/ 
		dims[2]={CARTESIAN_X,CARTESIAN_Y},/* we have a X*Y grid of processes ...
				... so: dims[0]->CARTESIAN_X,dims[1]->CARTESIAN_Y X=CARTESIAN_X, Y+CARTESIAN_Y */
		//periods[2]={1,1}, reorder=1, /*periodic grid of processes  -
		 //															p[0,0] can talk to p[N-1,N-1] and so on.*/
		//coords[2],					/* coordinates of a process, e.g. p[2,3] - 2nd row, 3rd col */
		//count=3,						/* 3-dimensional Data grid. useful when commiting 3D data grid sections
		// 										for communication between processes*/
	//	starts[3],					/* the origin upon which we select the datatype section we wish.*/
	//	subsizes[3],				/* length,width,and height of this datatype section*/
	//	bigsizes[3]={L,W,H},/*    >>   >>    >>   >>   of the whole 3D data array*/
		
		reductions=0; 			/* # of reductions performed.*/

	double	x[L][W][H],z[L][W][H],/*x-> 9 point stencil 3D data array,z-> physics 3D Data array*/
		mass_local,									/*total mass of the data*/
		mass_reduced,								/*the mass we compute after reduction*/
		T_init,T_final,t1=0.0,t2=0.0,t3=0.0,t4=0.0,t5=0.0,t6=0.0,t7=0.0,/*some timer checkpoints*/
		Tmax_communications,Tmax_computations,Tmax_overall,sendbuf[4],
		timer[7],Slowest_timer[4];//various timers here.
	MPI_Request reqs[8],sendreqs[4],recvreqs[4];	/*we need this request vector when we wait for communications to conclude*/
	MPI_Status stats[8];	/*every comm gives a status. not very important now*/
	
/////////////mpicartcom

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);//we'll use cartesian comm later.

	if (CARTESIAN_X*CARTESIAN_Y==numtasks)
	{
		if (numtasks >=4 && dims[0]>1 && dims[1]>1 )
		{	

			/*here we get initial time before computations/communications part.*/
			T_init=MPI_Wtime();
			setup();
			for (i=0;i<M;i++)
			{
			  t1=MPI_Wtime();

			  //Sending the datatype data sections
				MPI_Isend(&x[0][0][0],1,UP_segment_SEND   ,nbrs[UP],   
							tag,myCartComm,&reqs[UP]   );
				MPI_Isend(&x[0][0][0],1,DOWN_segment_SEND ,nbrs[DOWN], 
							tag,myCartComm,&reqs[DOWN] );
				MPI_Isend(&x[0][0][0],1,LEFT_segment_SEND ,nbrs[LEFT], 
							tag,myCartComm,&reqs[LEFT] );
				MPI_Isend(&x[0][0][0],1,RIGHT_segment_SEND,nbrs[RIGHT],
							tag,myCartComm,&reqs[RIGHT]);

			  //Receiving the datatype data sections in reverse order
				MPI_Irecv(&x[0][0][0],1,DOWN_segment_RCV, nbrs[DOWN], 
							tag,myCartComm,&reqs[DOWN+4] );
				MPI_Irecv(&x[0][0][0],1,UP_segment_RCV,   nbrs[UP],   
							tag,myCartComm,&reqs[UP+4]   );
				MPI_Irecv(&x[0][0][0],1,RIGHT_segment_RCV,nbrs[RIGHT],
							tag,myCartComm,&reqs[RIGHT+4]);
				MPI_Irecv(&x[0][0][0],1,LEFT_segment_RCV, nbrs[LEFT], 
							tag,myCartComm,&reqs[LEFT+4] );
				
				t2=MPI_Wtime();
				//independent computation while waiting to receive
				XY_plane_finite_dif_indp(x);
				physics_indp(z);
				
				t3=MPI_Wtime();			

				//waiting for receive communications
				MPI_Waitall(4,&reqs[4],MPI_STATUSES_IGNORE);
				t4=MPI_Wtime();			
				
				XY_plane_finite_dif_dep(x);//dependent communication after receiving
				physics_dep(z);
				
				t5=MPI_Wtime();		
				
				if(i%REDUCE_FREQUENCY==0 && SETREDUCE)
				{//after specified iterations, perform a reduction for total mass.
					MPI_Reduce (&mass_local,&mass_reduced,1,MPI_DOUBLE,
					MPI_SUM,0,myCartComm); reductions++;
				}
				
				t6=MPI_Wtime();	
				
				MPI_Waitall(4,reqs,MPI_STATUSES_IGNORE);//waiting for send communications
				t7=MPI_Wtime();	

			//determining which timer corresponds to what part.
				timer[sendrecv]	+=(t2-t1);
				timer[indep]		+=(t3-t2);
				timer[waitrecv]	+=(t4-t3);
				timer[dep]			+=(t5-t4);
				timer[reduce]		+=(t6-t5);
				timer[waitsend]	+=(t7-t6);	
				// timer[overall] ~= t7-t6+t6-t5+t5-t4+t4-t3+t3-t2+t2-t1 = t7-t1;
			}//endfor

			T_final=MPI_Wtime();//time after all communications and computations.
			timer[overall]=T_final-T_init;

			//prepare a sendbuffer to reduce the max timer values of interest.
			sendbuf[0]=timer[dep]+timer[indep];
			sendbuf[1]=timer[sendrecv]+timer[waitrecv]+timer[waitsend];
			sendbuf[2]=timer[overall];
			if(SETREDUCE)
				sendbuf[3]=timer[reduce];
				
			//find max times, to compute Tparallel = Tmax .
			MPI_Reduce (&sendbuf[0],&Slowest_timer[0],1,MPI_DOUBLE,MPI_MAX,0,myCartComm);
			MPI_Reduce (&sendbuf[1],&Slowest_timer[1],1,MPI_DOUBLE,MPI_MAX,0,myCartComm);
			MPI_Reduce (&sendbuf[2],&Slowest_timer[2],1,MPI_DOUBLE,MPI_MAX,0,myCartComm);
			if(SETREDUCE)
				MPI_Reduce (&sendbuf[3],&Slowest_timer[3],1,MPI_DOUBLE,MPI_MAX,0,myCartComm);
			
			if(!rank)
			{
				printf("\nMPI_Wtick() -> time resolution is: %.1e s = %.1e\
								 ms. \n",MPI_Wtick(),1E3*MPI_Wtick());
				printf("\n\t~----~\t\nCalculations maximum time:%.3lf ms\n\
				 				\rCommunications maximum time:%.3lf ms\n \
				 				\rOverall maximum time:%.3lf ms\n", 
				 				1E3*Slowest_timer[0],\
								1E3*Slowest_timer[1],\
								1E3*Slowest_timer[2]);
			if(SETREDUCE)
			{
				printf("\n# of Reductions = %d, T_reduction = %lf ms.\n",reductions,Slowest_timer[3]*1E3);
				printf("\nTotal mass is: %.2e kg.\n",mass_reduced);
			}
			 else
			 	printf("\nMass reductions are turned off.\n");
			}

		}//endif
		else
		  printf("Numtasks must be >=4, cartesian dimensions must be >=1. Terminating... "); 
		  
	}//endif 
	
	else
	{
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		if(!rank)printf("Must specify %d processors. Terminating..."
			,CARTESIAN_X*CARTESIAN_Y); 
	}
	MPI_Finalize();
	return 0;
}

void initHData(double x[][W][H],int rank)
{			//set up data values of size 'double' for our local array 'x'
	int i,j,k;
	srand((unsigned)rank);
	#if USE_OpenMP	//not important to parallelize.
	#pragma omp parallel for collapse(3)
	#endif
	for(i=0;i<L;i++){
		for(j=0;j<W;j++){
			for(k=0;k<H;k++){
				x[i][j][k]=(rand()%(int)10E6)*10E-4;	}}}
}

void XY_plane_finite_dif_indp(double x[][W][H])
{			//communication-independent finite-difference stencil computation
	int i,j,k;
	#if USE_OpenMP
	#pragma omp parallel for collapse(3)
	#endif
	for(i=4;i<L-2;i++){
		for(j=4;j<W-2;j++){
			for(k=1;k<H-1;k++)	  		{
				x[i][j][k]=( 4*x[i][j][k]+
				x[i-1][j][k]+x[i-2][j][k]+x[i][j-1][k]+x[i][j-2][k]+
				x[i][j+1][k]+x[i][j+2][k]+x[i+1][j][k]+x[i+2][j][k]+
				x[i][j][k+1]+x[i][j][k-1])/npsDENOM;	}}}
}

void XY_plane_finite_dif_dep(double x[][W][H])
{				//communication-dependent finite-difference stencil computation
	#if USE_OpenMP
	#pragma omp parallel
	#endif
	{
		int i,j,k;
		#if USE_OpenMP
		#pragma omp parallel for collapse(3)
		#endif	
		for(i=2;i<4;i++)
		{
			for(j=0;j<W-2;j++)
			{
				for(k=1;k<H-1;k++)
				{
					x[i][j][k]=( 4*x[i][j][k]+
					x[i-1][j][k]+x[i-2][j][k]+x[i][j-1][k]+x[i][j-2][k]+
					x[i][j+1][k]+x[i][j+2][k]+x[i+1][j][k]+x[i+2][j][k]+
					x[i][j][k+1]+x[i][j][k-1])/npsDENOM;
				}
			}
		}
		#if USE_OpenMP
		#pragma omp parallel for collapse(3)
		#endif	
		for(i=L-4;i<L-2;i++)
		{
			for(j=2;j<W-2;j++)
			{
				for(k=1;k<H-1;k++)
				{
					x[i][j][k]=( 4*x[i][j][k]+
					x[i-1][j][k]+x[i-2][j][k]+x[i][j-1][k]+x[i][j-2][k]+
					x[i][j+1][k]+x[i][j+2][k]+x[i+1][j][k]+x[i+2][j][k]+
					x[i][j][k+1]+x[i][j][k-1])/npsDENOM;	
				}
			}
		}
		#if USE_OpenMP
		#pragma omp parallel for collapse(3)
		#endif
		for(i=4;i<L-4;i++)
		{
			for(j=2;j<4;j++)
			{
				for(k=1;k<H-1;k++)
				{
					x[i][j][k]=( 4*x[i][j][k]+
					x[i-1][j][k]+x[i-2][j][k]+x[i][j-1][k]+x[i][j-2][k]+
					x[i][j+1][k]+x[i][j+2][k]+x[i+1][j][k]+x[i+2][j][k]+
					x[i][j][k+1]+x[i][j][k-1])/npsDENOM;	
				}
			}
		}
		#if USE_OpenMP
		#pragma omp parallel for collapse(3)
		#endif
		for(i=4;i<L-4;i++)
		{
			for(j=W-4;j<W-2;j++)
			{
				for(k=1;k<H-1;k++)
				{
					x[i][j][k]=( 4*x[i][j][k]+
					x[i-1][j][k]+x[i-2][j][k]+x[i][j-1][k]+x[i][j-2][k]+
					x[i][j+1][k]+x[i][j+2][k]+x[i+1][j][k]+x[i+2][j][k]+
					x[i][j][k+1]+x[i][j][k-1])/npsDENOM;	
				}
			}
		}

	}
}

void physics_indp(double z[][W][H])
{//communication-independent physics computation
	int i,j,k;
	#if USE_OpenMP
	#pragma omp parallel for collapse(2)
	#endif
	for(i=4;i<L-4;i++)
	{
		for(j=4;j<W-4;j++)
		{
			z[i][j][H-1]=1.00;//getting 100% of radiation. lower parts get less
			for (k=H-2;k>=0;k--)
				z[i][j][k]=pow( (z[i][j][k+1] * Radiation_Factor ),Physics_Power);
		}	
	}

}

void physics_dep(double z[][W][H])
{    		//communication-dependent physics computation
	int i,j,k;
	#if USE_OpenMP
	#pragma omp parallel
	#endif
	{
		#if USE_OpenMP
		#pragma omp parallel for collapse(2)
		#endif
		for(i=2;i<4;i++)
		{
			for(j=2;j<W-2;j++)
			{
				z[i][j][H-1]=1.00;
				for (k=H-2;k>=0;k--)
					z[i][j][k]=pow( (z[i][j][k+1] * Radiation_Factor ),Physics_Power);
			}
		}
		#if USE_OpenMP
		#pragma omp parallel for collapse(2)
		#endif		
		for(i=L-4;i<L-2;i++)
		{
			for(j=2;j<W-2;j++)
			{
				z[i][j][H-1]=1.00;
				for (k=H-2;k>=0;k--)
					z[i][j][k]=pow( (z[i][j][k+1] * Radiation_Factor ),Physics_Power);
			}
		}
		#if USE_OpenMP
		#pragma omp parallel for collapse(2)
		#endif				
		for(i=4;i<L-4;i++)
		{
			for(j=2;j<4;j++)
			{
				z[i][j][H-1]=1.00;
				for (k=H-2;k>=0;k--)
					z[i][j][k]=pow( (z[i][j][k+1] * Radiation_Factor ),Physics_Power);
			}			
		}
		#if USE_OpenMP
		#pragma omp parallel for collapse(2)
		#endif
		for(i=4;i<L-4;i++)
		{
			for(j=W-4;j<W-2;j++)
			{
			
				z[i][j][H-1]=1.00;
				for (k=H-2;k>=0;k--)
					z[i][j][k]=pow( (z[i][j][k+1] * Radiation_Factor ),Physics_Power);
			}		
		}

	}
}

void setup()
{			/*setting up our cartesian topology.*/
			MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,reorder,&myCartComm);
			MPI_Comm_rank(myCartComm,&rank);
			MPI_Cart_coords(myCartComm, rank, 2, coords);
			MPI_Cart_shift(myCartComm,0,1,&nbrs[UP],&nbrs[DOWN]);
			MPI_Cart_shift(myCartComm,1,1,&nbrs[LEFT],&nbrs[RIGHT]);

			//distribute total mass fractions to processes.
			mass_local=mass_overall/(double)(CARTESIAN_X*CARTESIAN_Y);
	
			initHData(x,rank);/*initiating our Data Array x[L][W][H]. */
			/*vertical computations (z[L][W][H]) depend on the clear sky and
			 extend downwards. no need to initialize.*/
			
			if(!rank)
			{
				printf("\nLocal Data Array (w/ extra outer parts):");				
				printf("Length L=%d, Width W=%d, Height H=%d\n",L,W,H);
			}

			starts[0]=2;  starts[1]=2;  starts[2]=0;//send-to-nbors[L]-datatype
			subsizes[0]=L-2-2;subsizes[1]=2;subsizes[2]=H;
			MPI_Type_create_subarray(count, bigsizes, subsizes, 
				starts,MPI_ORDER_C, MPI_DOUBLE, &LEFT_segment_SEND);
			MPI_Type_commit(&LEFT_segment_SEND);

			starts[0]=2;  starts[1]=0;  starts[2]=0;//receive-from-nbors[L]-datatype 	
			subsizes[0]=L-2-2;subsizes[1]=2;subsizes[2]=H;
			MPI_Type_create_subarray(count, bigsizes, subsizes, 
				starts,MPI_ORDER_C, MPI_DOUBLE, &LEFT_segment_RCV);
			MPI_Type_commit(&LEFT_segment_RCV);

			starts[0]=2;  starts[1]=W-4;  starts[2]=0;//send-to-nbors[R]-datatype 
			subsizes[0]=L-4;subsizes[1]=2;subsizes[2]=H;
			MPI_Type_create_subarray(count, bigsizes, subsizes, 
				starts,MPI_ORDER_C, MPI_DOUBLE, &RIGHT_segment_SEND);
			MPI_Type_commit(&RIGHT_segment_SEND);

			starts[0]=2;  starts[1]=W-2;  starts[2]=0;//receive-from-nbors[R]datatype 	
			subsizes[0]=L-4;subsizes[1]=2;subsizes[2]=H;
			MPI_Type_create_subarray(count, bigsizes, subsizes, 
				starts,MPI_ORDER_C, MPI_DOUBLE, &RIGHT_segment_RCV);
			MPI_Type_commit(&RIGHT_segment_RCV);

			starts[0]=2;  starts[1]=2;  starts[2]=0;//send-to-nbors[U]-datatype
			subsizes[0]=2;subsizes[1]=W-4;subsizes[2]=H;
			MPI_Type_create_subarray(count, bigsizes, subsizes, 
				starts,MPI_ORDER_C, MPI_DOUBLE, &UP_segment_SEND);
			MPI_Type_commit(&UP_segment_SEND);

			starts[0]=0;  starts[1]=2;  starts[2]=0;//receive-from-nbors[U]-datatype	
			subsizes[0]=2;subsizes[1]=W-4;subsizes[2]=H;
			MPI_Type_create_subarray(count, bigsizes, subsizes, 
				starts,MPI_ORDER_C, MPI_DOUBLE, &UP_segment_RCV);
			MPI_Type_commit(&UP_segment_RCV);

			starts[0]=L-4;  starts[1]=2;  starts[2]=0;//send-to-nbors[D]-datatype
			subsizes[0]=2;subsizes[1]=W-4;subsizes[2]=H;
			MPI_Type_create_subarray(count, bigsizes, subsizes, 
				starts,MPI_ORDER_C, MPI_DOUBLE, &DOWN_segment_SEND);
			MPI_Type_commit(&DOWN_segment_SEND);

			starts[0]=L-2;  starts[1]=2;  starts[2]=0;//receive-from-nbors[D]-datatype	
			subsizes[0]=2;subsizes[1]=W-4;subsizes[2]=H;
			MPI_Type_create_subarray(count, bigsizes, subsizes, 
				starts,MPI_ORDER_C, MPI_DOUBLE, &DOWN_segment_RCV);
			MPI_Type_commit(&DOWN_segment_RCV);
}

