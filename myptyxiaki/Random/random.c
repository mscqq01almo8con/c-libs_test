#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include "gnuplot_i.h"

//The program gives a data file with vectors of form X=(a,b,c,d) for every row.(Το πρόγραμμα δίνει αρχείο με διανύσματα Χ=(α,β,γ,δ) σε κάθε σειρά)
#define value1 25
#define value2 40
#define value3 78

#define count1 20
#define count2 30
#define count3 50

/*output: 	20 points with values ~25+random number
 		30 points with values ~40+random number
		50 points with values ~78+random number
*/

#define rand_mag 15 /*(randomness magnitude)*/
int main(void)
{
 int i,j,value_frequency[(count1+count2+count3)],reading[400];double x[100],a[100];
 FILE *fp1=fopen("random.dat","w");
 gnuplot_ctrl*b;

 printf("\nPrinting number groups with a standard value plus a random value...\n");
 for(i=0;i<400;i++)reading[i]=0;
 for (i=0;i<100;i++){value_frequency[i]=0;x[i]=0.0;a[i]=0.0;}
 for (i=0;i<count1;i++)
 {
  fprintf(fp1,"%d %d %d %d\n",value1+(int)(rand_mag*drand48()),value1+(int)(rand_mag*drand48()),value1+(int)(rand_mag*drand48()),value1+(int)(rand_mag*drand48()));value_frequency[i]=0;
 }

 for (i=0;i<count2;i++)
 {
  fprintf(fp1,"%d %d %d %d\n",value2+(int)(rand_mag*drand48()),value2+(int)(rand_mag*drand48()),value2+(int)(rand_mag*drand48()),value2+(int)(rand_mag*drand48()));value_frequency[i]=0;
 }
 for (i=0;i<count3;i++)
 {
  fprintf(fp1,"%d %d %d %d\n",value3+(int)(rand_mag*drand48()),value3+(int)(rand_mag*drand48()),value3+(int)(rand_mag*drand48()),value3+(int)(rand_mag*drand48()));value_frequency[i]=0;
 }
 

 fclose(fp1);
 /*            					*/
 fp1=fopen("random.dat","r");
 for(i=0;i<400;i++)
 {
   fscanf(fp1,"%d",&reading[i]);
   for(j=0;j<100;j++)
   {
    if(reading[i]==j)
    {
     value_frequency[j]++;
     a[j]=(double)value_frequency[j];
    }
   }
 }
 fclose(fp1);

 x[0]=0.0;

 for(i=1;i<100;i++)x[i]=x[i-1]+1.0;
 b=gnuplot_init();
 gnuplot_setstyle(b,"impulses");
 gnuplot_set_xlabel(b,"value");
 gnuplot_set_ylabel(b,"frequency");

 gnuplot_cmd(b,"set terminal svg");
 gnuplot_cmd(b,"set output \"random.svg\"");
 //gnuplot_cmd(b,"set xrange [0:100]");//optional. sets the X-axis range.(προαιρετικό, θέτει το εύρος του άξονα Χ)
 
  
 gnuplot_plot_xy(b,x,a,100,"");//plotting current group curve (σχεδιάζουμε την καμπύλη της τρέχουσας ομάδας)
 //now we have all group curves in a single diagram.(έχουμε όλες τις καμπύλες cluster σε μία γραφική παράσταση)

 
 gnuplot_close(b);

 printf("\nPrint complete\n");
 return 0;
}
