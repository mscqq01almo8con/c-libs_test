//#include "mpi.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

typedef struct xouxou *ii;
struct xouxou{
	int aa;
};

struct axaxa{
	int aa;
}*m,n,o;

main(){
	int *a,b[2][2][2];
	printf("ceil(%lf)=%d\n",2.001,(int)ceil(2.001));
	a=(int*)b;
	b[1][1][1]=93;
	//printf("\n%d",a[1][1][1]);
	int rowsize=2*2;//without sizeof(double)
	int colsize=2;
	int height=1;
	printf("\n%d",*(a+1*rowsize+1*colsize+1*height ) );

int av,*bv,**cv,***dv;

m=(struct axaxa*)malloc(sizeof(struct axaxa));
m->aa=8;
	av=193;

	bv=&av;

	cv=&bv;

	dv=&cv;
struct xouxou baba;
ii iii;
	***dv=287;iii=0;

	printf("\nav=%d,***dv=%d",av,***dv);
	while(1)
	{++iii;iii->aa=0;}

}

