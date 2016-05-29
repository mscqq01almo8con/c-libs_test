#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
int main(void)
{int i,j,k,displ;
int a[2][2][2]={1,2,3,4,5,6,7,8};
int *b,addr;
int continuous[8]={ /*1st row x*/ 
			/*1st col y*/
				 /*1st col z*/ 1,
				 /*2nd col z*/ 2, 
		    /*1st row x*/ 
			/*2nd col y*/ 
				/*1st col z*/ 3,
				/*2nd col z*/ 4, 
		    /*2nd row x*/
			/*1st col y*/
				/*1st col z*/5,
				/*2nd col z*/6,
			/*2nd col y*/
				/*1st col z*/7,
				/*2nd col z*/8  };

for(i=0;i<2;i++)
	for(j=0;j<2;j++)
		for(k=0;k<2;k++)
			printf("a[%d][%d][%d]=%d\n",i,j,k,a[i][j][k]);

for(i=0;i<2;i++)
	for(j=0;j<2;j++)
		for(k=0;k<2;k++)
			printf("a[%d][%d][%d]=%d\n",i,j,k,*(*(*(a+i)+j)+k));

for(i=0;i<2;i++)
	for(j=0;j<2;j++) 
		for(k=0;k<2;k++)
		{
		 displ=i*(2*2)+j*(2)+k;
		 printf("b=%d \n",*(continuous+displ) );

		}
		
	
	b=&a[0];		
for(i=0;i<8;i++)
	printf("b=%d\n",*(b+i)/*or just: b[i]*/);
//typonontas merikous double tyxaious- setaro me to srand, paragw me to rand()%x
time_t ii;
srand((unsigned)time(&ii));
for(i=0;i<10;i++)printf("%.3lf\n",(rand()%(int)10E6)*10E-4);
return 0;
}
