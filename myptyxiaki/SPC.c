/****************************************************************/
/*                          SPC.c

Program objectives:  
		1. MC Simulation w/ SW algorithm
		2. Measurement of Thermodynamics Quantities
		3. Clustering Data

Input: a .dat file which contains the initial data of the problem (vectors of a given dimensionality) and a .dat file which was generated by nbors.c program and contains information about the neighbors of each point (or vector).

Output: 	-a .dat file which contains information about the main thermodynamic variables of the problem
                -a .dat file which includes the correlation function G information
		-a .dat file which contains created clusters information
		-a graph depicting chi[T]
----------------------------------------------------------------------------------

Σκοπός του προγράμματος:

		1. Προσωμοίωση Monte Carlo με τον αλγόριθμο Swendsen-Wang
		2. Μέτρηση θερμοδυναμικών ποσοτήτων
		3. Ομαδοποίηση δεδομένων

Είσοδος: ένα αρχείο .dat που περιέχει τα αρχικά δεδομένα του προβλήματος (διανύσματα συγκεκριμένων διαστάσεων) και ένα αρχείο .dat που περιέχει πληροφορίες για τους γείτονες καθενός σημείου (ή διανύσματος).

Έξοδος: 	-ένα .dat αρχείο που περιέχει πληροφορίες για τις θερμοδυναμικές ποσότητες του προβλήματος
		-ένα .dat αρχείο που περιέχει πληροφορίες για τη συνάρτηση συσχέτισης G
		-ένα .dat αρχείο που περιέχει πληροφορίες για τα clusters που δημιουργήθηκαν
		-ένα γράφημα που δείχνει τη χ[Τ]
*/
/****************************************************************/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "gnuplot_i.h"//Χρησιμοποιείται η γεννήτρια γραφικών παραστάσεων Gnuplot: Copyright 1986 - 1993, 1998, 2004 Thomas Williams, Colin Kelley 
#include "quicksort.c"

#define N    150      //# of vectors (# σημείων (διανυσμάτων))
#define DM   4        //# of dimensions (# διαστάσεων του χώρου)) 
#define NB   7        //# of MAX Neighbors (# μέγιστων γειτόνων (K+H) )
#define B    7        //# of Biggest Clusters (#μεγαλύτερων clusters)
#define S    40      //# of Total simulation (with different T) (αριθμός βημάτων θερμοκρασίας )
#define M    15000   //# of Monte Carlo simulation (# Επαναλήψεων Monte Carlo)
#define D    1500 //# of discarded simulation(optional) (# Αφαιρούμενων αρχικών βημάτων (προαιρετικό) )
#define EPS  1.0e-300 //Lowest temperature, close to 0 (Χαμηλότερη θερμοκρασία, κοντά στο 0)
#define plot_pixelmap 1 //plot two-dimensional pixelmap? 0=no, 1 or not 0 = yes(Να σχεδιαστούν οι δισδιάστατες προβολές? 0=όχι, 1=ναι)

	/*------Global variables (Καθολικές μεταβλητές)------*/
int q=20/*# of Potts states (#αριθμός καταστάσεων)*/,cluster[N+1],cluster_data[N+1][S+1+1],nbors[N+1][NB+1]/*neighbors (γείτονες)*/,s[N+1]/*spin*/,Ns[20+1]/*[..]=same as q +1 */;

double T,T0,Tmax,Tinc,th,data[N+1][DM+1]/*τα δεδομένα*/,dist[N+1][NB+1]/*distances(αποστάσεις)*/,J[N+1][NB+1]/*interactions(αλληλεπιδράσεις)*/,G[N+1][NB+1]/*spin-spin correlation function(συνάρτηση συσχέτισης σπιν-σπιν*/,G_max[N+1]/*μέγιστη τιμή της G*/,C[N+1][NB+1],a1[S+1],lattice[N][DM+1];

		/*------functions (συναρτήσεις)------*/
void interaction_matrix(int nbors[][NB+1],double J[][NB+1],double dist[][NB+1],double data[][DM+1]);

double SW(int q,double T,int nbors[][NB+1],int bond[][NB+1],int cluster[N+1],double J[][NB+1],double G[][NB+1]);

int output(double lattice[][DM+1],char filename[]);

int HK(int cluster[N+1],int nbors[][NB+1],int bond[][NB+1]);

int final_cluster(int k,int q,double th,int nbors[][NB+1],int bond[][NB+1],int cluster[N+1],int cluster_data[N+1][S+1+1]/*?*/,double G[][NB+1]);

int find_ClusterSize(int k,int cluster[N+1],int fcluster[S+1+1],int cluster_size[][B+1]);
/*----------------------------------------------------------------------------*/

int main(void)
{
 int i,j,k,m,seed,idx,fcluster[S+1+1],bond[N+1][NB+1],cluster_size[S+1+1][B+1];
 double X[S+1+1],temperature[S+1];
 char filename[20];
 gnuplot_ctrl*a;
 gnuplot_ctrl*b;
 
 FILE 	*fp1=fopen("Iris01.dat","r"),//Αρχικό αρχείο δεδομένων-Συμπληρώνεται και στη συνάρτηση output, εάν θέλουμε να κάνουμε διστδιάστατες προβολές
      	*fp2=fopen("data_iris.dat","w"),//Αρχείο που δημιουργείται, με τα παραγόμενα δεδομένα (χ,Τ,cluster size κλπ)
	*fp3=fopen("cluster_iris","w"),//αρχείο με πληροφορίες για τα clusters
	*fp4=fopen("G.dat","w"),//αρχείο με πληροφορίες για τη συνάρτηση G
	*fp5=fopen("nbors_iris.dat","r");//αρχείο γειτόνων, που παρήχθησαν από το πρόγραμμα nbors.c

 /*Parameters(Παράμετροι)*/

 
 T0=EPS;           //Starting Temperature(αρχική θερμοκρασία)
 Tmax=0.09;        //Highest Temperature(τελική θερμοκρασία)
 Tinc=(Tmax-T0)/S; //Temperature Increment(βήμα θερμοκρασίας)
 th=0.5;           //Threshold Value for condition [G(i,j)>th] (κατώφλι για τη συνθήκη [G(i,j)>th])

 /*Read data from file(διάβασμα αρχείου)*/
 /*
 	 =============
 	 =============
                               */
 //Initialise data(Αρχικοποίηση δεδομένων)

 for(i=0;i<=N;i++)for(j=0;j<=DM;j++)data[i][j]=0.0;T=0.0;
 for(i=0;i<=N;i++)for(j=0;j<=NB;j++){dist[i][j]=0.0;nbors[i][j]=0;}
 for(i=0;i<=N;i++)for(j=0;j<=NB;j++){J[i][j]=0.0;G[i][j]=0.0;}
 for(i=0;i<=S+1;i++){X[i]=0.0;fcluster[i]=0;}
 for(i=0;i<=S+1;i++)for(j=0;j<=B;j++)cluster_size[i][j]=0;
 for(i=0;i<=N;i++)for(j=0;j<=NB;j++)bond[i][j]=0;
 for(i=0;i<=N;i++)G_max[i]=0.0;for(i=0;i<=S;i++)temperature[i]=0;
 for(i=0;i<=N;i++)cluster[i]=0;
 for(i=0;i<S+1;i++)a1[i]=0.0;
 //Open file(Άνοιγμα αρχείου)
 
 //File 1---------------------------------------------
 if(fp1==NULL)
 {
  printf("Unable to open file for reading\n");
  exit(1);
 }
 rewind(fp1);//rewinding file(τοποθέτηση δείκτη αρχείου στην αρχή)
 i=1;j=1;
 while(fscanf(fp1,"%lf",&data[i][j])!=EOF)
 {
  j++;
  if(j==DM+1)
  {
   i++;
   j=1;
  }//Read data from file(Ανάγνωση δεδομένων από το αρχείο)
 }
 fclose(fp1);
 //File 1---------------------------------------------

 printf("\nReading Data Complete\n");

 //open file (άνοιγμα αρχείου γειτόνων)
 
 //File 5---------------------------------------------
 if(fp5==NULL)
 {
  printf("\nUnable to open file for reading\n");
  exit(1);
 }
 i=1;j=1;
 while (fscanf(fp5,"%d ",&nbors[i][j])!=EOF)
 {
  j++;
  if(j==NB+1)
   {
    i++;
    j=1;
   }
 }
 fclose(fp5);
 printf("Reading Neighbor List File Complete\n");
 //File 5---------------------------------------------

 
 /*Calculate Interaction Matrix*/
 /*============================*/

 /*-----CALL interaction_matrix function to calculate the interactions between the points (Καλούμε τη συνάρτηση interaction_matrix για τον υπολογισμό των αλληλεπιδράσεων μεταξύ των σημείων )------*/
 interaction_matrix(nbors,J,dist,data);
 /*------				  */

 printf("Calculate Interaction matrix complete\n");

 //Create random numbers(Δημιουργούμε τυχαίους αριθμούς)
 srand48((unsigned int)time(NULL));
 printf("seed =%d\n",(unsigned int) time(NULL));
 seed=1083354197;
 srand48(seed);
 
 /*--------------------------------------------------------*/
 for(k=1;k<=S+1;k++)//Temperature steps (Βήματα θερμοκρασίας)
 {
  /* Clustering Algorithm(Αλγόριθμος Ομαδοποίησης)   */
  
  T=T0+Tinc*(k-1);
  X[k]=SW(q,T,nbors,bond,cluster,J,G);//χ[T] Calculation
  printf("T= %f X[%d] = %f\n",T,k,X[k]);
  temperature[k]=T;
  /*  Final Data Clusters   */
  fcluster[k]=final_cluster(k,q,th,nbors,bond,cluster,cluster_data,G);
  printf("# of clusters[%d]=%d\n",k,fcluster[k]);
  if(plot_pixelmap==1)
  {
   sprintf(filename,"%f",temperature[k]);
   strncat(filename,".ppm",4);
   output(lattice,filename);
  }
 /*   Find Biggest Clusters  */  
  find_ClusterSize(k,cluster,fcluster,cluster_size);
 }
 /*--------------------------------------------------------*/
 /*Write result to file (εγγραφή αποτελεσμάτων σε αρχείο)*/

  //File 2---------------------------------------------
 
 X[2]=0.0309;
 for(i=1;i<=S+1;i++)
 {
  fprintf(fp2,"%f  %f  %d ",T0+Tinc*(i-1),X[i],fcluster[i]);
  for(j=1;j<=B;j++)
  {
   fprintf(fp2,"  %d  ",cluster_size[i][j]);
  }
  fprintf(fp2,"\n");
 }
 fclose(fp2);
 //File 2---------------------------------------------


 //File 3---------------------------------------------
 for(i=1;i<=N;i++)
 {
  for(j=1;j<=S+1;j++)
  {
   fprintf(fp3,"%d  ",cluster_data[i][j]);
  }
  fprintf(fp3,"\n");
 }
 fclose(fp3);
 //File 3---------------------------------------------
 

 //File 4---------------------------------------------
 for(i=1;i<=N;i++)
 {
  for(j=1;j<=NB;j++)
  {
   fprintf(fp4,"%f ",G[i][j]);
  }
  fprintf(fp4,"\n");
 }
 fclose(fp4);
 //File 4---------------------------------------------

 /*Gnuplot Graphics session - Συνεδρία γραφικών Gnuplot*/

 a=gnuplot_init();
 gnuplot_setstyle(a,"lines");
 gnuplot_set_xlabel(a,"Boltzmann constant*temperature");
 gnuplot_set_ylabel(a,"Susceptibility density");
 gnuplot_cmd(a,"set terminal svg");
 gnuplot_cmd(a,"set output \"Susceptibility.svg\"");
 for(k=0;k<=S;k++)
 {//Correction for Gnuplot, nothing important
  temperature[k]=temperature[k+1];
  X[k]=X[k+1];
 }
 gnuplot_plot_xy(a,temperature,X,(S+1),"X");//plotting X for all temperatures. (σχεδιάζουμε την χ για όλες τις θερμοκρασίες.)
 gnuplot_close(a);

 //---------------------------------------------------------//

 b=gnuplot_init();
 gnuplot_setstyle(b,"lines");
 gnuplot_set_xlabel(b,"Boltzmann constant*temperature");
 gnuplot_set_ylabel(b,"Cluster Size");
 gnuplot_cmd(b,"set terminal svg");
 gnuplot_cmd(b,"set output \"Clustersize.svg\"");
 //gnuplot_cmd(b,"set xrange [0:0.08]");//optional. sets the x-axis range.
 for (i=1;i<=B;i++)//For all the biggest clusters(για όλα τα μεγαλύτερα clusters)
 {
  for (j=1;j<=(S+1);j++)//For all temperatures (για όλες τις θερμοκρασίες)
  {
   a1[j]=(double)cluster_size[j][i];//keep current cluster array depending on T, to plot(κρατάμε τον τρέχοντα πίνακα για ένα συγκεκριμένο cluster για όλες τις Τ)
  }
  gnuplot_plot_xy(b,temperature,a1,(S+1),"cluster");//plotting current cluster curve (σχεδιάζουμε την καμπύλη του τρέχοντος cluster)
 }//now we have all cluster curves in a single diagram.(έχουμε όλες τις καμπύλες cluster σε μία γραφική παράσταση)
 gnuplot_close(b);

//---------------------------------------------------------//

return 0;
}


void interaction_matrix(int nbors[][NB+1],double J[][NB+1],double dist[][NB+1],double data[][DM+1])
{
 int i,j,k,m,n,num_nbors;
 double sum,temp,K_hat,a,sum_dist,min_dist,max_dist,Jmin,Jmax,Jmean;
//----------------------------------------------------------------------//
 num_nbors = 0.0;	//
 sum_dist = 0.0; 	//
 max_dist=Jmax = 0.0;   //
 min_dist=Jmin = 100.0; //
 Jmean = 0.0;		//
 			//initialization of variables 
 for (i=1;i<=N;i++)	//
  {			//(αρχικοποίηση μεταβλητών)
   for(j=1;j<=NB;j++)	//
    {			//
     dist[i][j] = 0.0;	//
     J[i][j]    = 0.0;	//
    }			//
  }			//
//-----------------------------------------------------------------------//

 for(i=1;i<=N;i++)//For all the points(Για όλα τα διανύσματα(σημεία) )
 {
  for(j=1;j<=NB;j++)//For all neighbors (Για όλους τους γείτονες των διανυσμάτων(σημείων) )
  {
   if(nbors[i][j]!=0)
   {
    sum = 0.0;
    num_nbors++;
    n=nbors[i][j];
    for(k=1;k<=DM;k++)
    {
     sum+=(data[i][k]-data[n][k])*(data[i][k]-data[n][k]);//distance square calculation(υπολογισμός τετραγώνου απόστασης)
    }
    dist[i][j]=sqrt(sum);// I build an array dist[N][NB] (Δημιουργούμε ένα πίνακα αποστάσεων dist[N][NB] )
    sum_dist+=dist[i][j];
    if(dist[i][j]<min_dist)
    min_dist=dist[i][j];//Set as minimum distance (Θέτουμε ελάχιστη απόσταση)
    if(dist[i][j]>max_dist)
    max_dist=dist[i][j];//Set as maximum distance (θέτουμε τη μέγιστη απόσταση)
   }
  }
 }

 //num_nbors is obviously used in determining the average neighbors(το num_nbors χρησιμοποιείται για τη μέση τιμή του αριθμού γειτόνων.)
 //sum_dist is used for the mean distance between neighbors.(το sum_dist χρησιμοποιείται για την εύρεση της μέσης απόστασης μεταξύ γειτόνων)

 K_hat=(double)num_nbors/N;           //Average # of Neighbors(Μέση τιμή γειτόνων)
 a    =(double)sum_dist/num_nbors;    //Average distances of 
                                     //neighboring points(Μέση απόσταση μεταξύ γειτόνων)

 for(i=1;i<=N;i++)//For all points(Για όλα τα σημεία)
 {
  for(j=1;j<=NB;j++)//For all neighbors(Για όλους τους γείτονές τους)
  {
   if(nbors[i][j]==0)//If i,j not neighbors(Αν τα i,j δεν είναι γείτονες)
   {
    J[i][j]=0.0;//Set interaction J(i,j)=0(θέτουμε αλληλεπίδραση J(i,j)=0 )
   }
   else	//Otherwise, set interaction according to the formula described in theory(αλλιώς, θέτουμε την αλληλεπίδραση ίση με αυτή που περοβλέπεται από τη θεωρία)
   {
    J[i][j]=1/K_hat*exp(-dist[i][j]*dist[i][j]/(2*a*a));
    Jmean+=J[i][j];//Using the Jmean to calculate average interaction J later. (Χρησιμοποιούμε το J για τον υπολογισμό της μέσης τιμής αλληλεπίδρασης J αργότερα)
    if(J[i][j]<Jmin)
    Jmin=J[i][j];//Ελάχιστη αλληλεπίδραση.
    if(J[i][j]>Jmax)
    Jmax=J[i][j];//Μέγιστη αλληλεπίδραση.
   }
  }
 }

 Jmean=Jmean/num_nbors;//Normalization, calculate the mean value of J (Κανονικοποίηση, υπολογισμός μέσης τιμής της J)

 printf("Total # Neighbors  = %d\n",num_nbors);
 printf("Total # Interactions= %d\n",num_nbors/2);
 printf("Sum Distance = %f \n",sum_dist);
 printf("a= %f K_hat= %f  \n",a,K_hat);
 printf("Min Dist = %e \n",min_dist);
 printf("Max_Dist = %f \n",max_dist);
 printf("Jmin = %e\n",Jmin);
 printf("Jmax = %f\n",Jmax);
 printf("Jmean = %f\n",Jmean);

}//End of function interaction_matrix.

double SW(int q,double T,int nbors[][NB+1],int bond[][NB+1],int cluster[N+1],double J[][NB+1],double G[][NB+1])
{//Monte Carlo iterations function. it outputs chi of a given T. (συνάρτηση που εκτελεί τα βήματα Monte Carlo της προσομοίωσης . Δίνει στην έξοδο το χ για ένα συγκεκριμένο Τ.)
 
 int i,j,k,n,kk,kkmax,idx,itr,ns,Nmax,num_cluster,avg_cluster;
 double p,m,m_mean1,m_mean2;
 
 for(i=0;i<=N;i++)s[i]=0;	//initialisation of variables
 for(i=0;i<=q;i++)Ns[q]=0;	//
 m=p=0.0;			//(αρχικοποίηση μεταβλητών)
 m_mean1=m_mean2=0.0;		//
 num_cluster=avg_cluster=0;	//
 for(i=1;i<=N;i++)  		//
  { 				//
   for(j=1;j<=NB;j++)		//
    {				//
     C[i][j]   = 0.0;		//
     G[i][j]   = 0.0;		//Initial Spin Variable
     bond[i][j]= 0.0;		//(Αρχική τιμή σπιν)
    }				//
   s[i]=drand48()*q+1;		//
  }				//

 //==============================================================

 //Starting the Monte Carlo iterations (εκκίνηση προσομοίωσης Monte Carlo)
 itr=0;
 kk=1;
 kkmax=M-D+1;
 for(;;)//it has to find a "break "command to end loop.(πρέπει να βρει break εντολή για να σταματήσει η επανάληψη.)
 {

  //--------------------------------------------------------------------------//

  //Start initializing(Αρχή αρχικοποίησης)
  if(kk==kkmax)break;//if we reach the maximum Monte Carlo iterations, stop. (εάν φτάσουμε τις μέγιστες επαναλήψεις Monte Carlo, σταματάμε και περνάμε στην επόμενη εντολή.)
  for(i=1;i<=N;i++)//For all points N(Για όλα τα σημεία Ν)
  {                     	
   for(j=1;j<=NB;j++)//For all neighbors NB(Για όλους τους γέιτονες ΝΒ)   	
   {				
    bond[i][j]=0;	//Initialization of bonds (Αρχικοποίηση δεσμών)
   }
  }
  //End initializing(Τέλος αρχικοποίησης)

 //Start computing the interaction and compare with a random probability(Αρχή υπολογισμού της αλληλεπίδρασης και σύγκρισής της με μια τυχαία πιθανότητα)
 for(i=1;i<=N;i++)//For all points N(για όλα τα σημεία Ν)
 {
  for(j=1;j<=NB;j++)//For all neighbors NB(για όλους τους γείτονες ΝΒ)
  {
   if(nbors[i][j]==0)//If i,j not neighbors(Αν i,j δεν είναι γείτονες)
   {
    break;//Next command, keep the current i,j (Επόμενη εντολή, κρατώ τα τρέχοντα i,j)
   }
   n=nbors[i][j];//Setting n as the first point beyond which the i point doesn't have a neighbor (Θέτω το n ως το πρώτο σημείο πέρα από το  οποίο το i δεν έχει άλλο γείτονα)
   if(s[i]==s[n] && i<n)//so, if spin(i)=spin(j) AND i is a neighbor [i<n] (άρα, αν σπιν(i)=σπιν(j) KAI το i είναι μικρότερο από το n) *the neighbors are sorted *οι γείτονες είναι κατανεμημένοι
   {
    p=1-exp(-J[i][j]/T);//Probability to bond (Πιθανότητα 2 σπιν να αποκτήσουν δεσμό)
    if(drand48()<=p)//Compare the p with a quasi-random probability(σύγκριση της πιθανότητας p με ψευδο-τυχαίο αριθμό)
    {
     bond[i][j]=1;//then make a bond between i,j (τότε δημιουργία δεσμού ανάμεσα σε i,j)
     for(k=1;k<=NB;k++)//Also, check if i is a neighbor of j (mutuality)
     {			//Επίσης ψάχνουμε αν ο i είναι γείτονας toy j (αμοιβαίος)
     if(nbors[n][k]==i)
      {
       bond[n][k]=1;//Then make a bond of j with i too. (Τότε δίνουμε bond και ανάμεσα στο j,i.)
       break;//Next command(επόμενη εντολή)
      }
     }
    }
   }
  }
 }
 //End computing the interaction and compare with a random probability(Τέλος υπολογισμού της αλληλεπίδρασης και σύγκρισής της με μια τυχαία πιθανότητα)

 //--------------------------------------------------------------------------//

 /*	CALL HK function to label the clusters	*/
 num_cluster=HK(cluster,nbors,bond);//labelling Cluster (δίνουμε ετικέτα στο κάθε  cluster)
 /*	
						*/
 //--------------------------------------------------------------------------//

 //Start giving a random new spin number to a SW cluster(Αρχή απόδοσης τυχαίας τιμής σε ένα SW cluster)
 for(i=1;i<=N;i++)//For all points N(Για όλα τα σημεία Ν)
 s[i]=0;//Give an initial spin value of 0 (Δίνουμε μια αρχική τιμή σπιν 0)
 for(i=1;i<=N;i++)//For all points N(Για όλα τα σημεία Ν)
 {
 if(s[i]==0)//check if spin(i)=0(ψάχνω αν σπιν(i)=0)
  {
   s[i]=drand48()*q+1;//Give a random new Potts spin value to a cluster(Απόδοση τυχαίας τιμής σπιν Potts σε ένα cluster)
   for(j=i+1;j<=N;j++)
   {
    if(cluster[j]==cluster[i])
    s[j]=s[i];//Change spin j with the spin of i(Αλλαγή σπιν του j με το σπιν του i)
   }
  }
 }
 //End giving a random new spin number to a SW cluster(Τέλος απόδοσης τυχαίας τιμής σε ένα SW cluster)

 itr+=1;// **  Iterating for all the Monte Carlo steps(Επαναλαμβάνοντας για όλα τα βήματα Monte Carlo)

 if(itr>D)//if iterations> discarded steps, start counting. (αν οι επαναλήψεις είναι μεγαλύτερες από τα βήματα που αφαιρώ, ξεκινώ την καταμέτρηση)
 {
  kk++;
  Nmax=0;		//Calculate Magnetization Value(Υπολογισμός μαγνήτισης)
  for(i=1;i<=q;i++)	//For all the Potts states(για όλες τις καταστάσεις Potts)
  Ns[i]=0;		//initialize all frequencies of the Potts states (Αρχικοποίηση για όλες τις συχνότητες καταστάσεων Potts) 
  for(i=1;i<=N;i++)	//For all points(για όλα τα σημεία Ν)
  Ns[s[i]]++;		//Increase the value of frequency Ns of a given Potts spin s[i] (Αύξηση τιμής συχνότητας N μιας δεδομένης κατάστασης Potts s[i])
 
  for(i=1;i<=q;i++)	//For all Potts states(Για όλες τις καταστάσεις Potts)
  {
   if(Ns[i]>Nmax)	//Find maximal frequency Ns of Potts state s[i]
   Nmax=Ns[i];		//Βρες τη μέγιστη συχνότητα Ns μιας Potts κατάστασης s[i]
  }

  m=(double)(q*Nmax-N)/((q-1)*N);//Calculate mean magnetization based on the above Nmax (υπολογισμός μέσης μαγνήτισης μέσω του παραπάνω Nmax)

  m_mean1+=m;//  <m>*(M-D)	(needs normalization)
  m_mean2+=m*m;//  <m^2>*(M-D)	(needs normalization)
  avg_cluster+=num_cluster;//Average clusters numbers(μέση τιμή clusters)

  for(i=1;i<=N;i++)//For all N points(Για όλα τα σημεία Ν)
  {
   for(j=1;j<=NB;j++)//For all NB neighbors (Για όλους τους γείτονες ΝΒ)
   {
    n=nbors[i][j];//n= the j-th neighbor of i (ο n είναι ο j-οστός γείτονας του i)
    if(n==0)
    {
     break;
    }
    if(cluster[i]==cluster[n])//If i, nbor[i][j] are in the same cluster (Εάν i,nbor[i][j] είναι στο ίδιο cluster)
    {
     C[i][j]+=1.0;//sum up the c(i,j) factors(αθροίζουμε τους παράγοντες  c(i,j)
    }
   }
  }
 }
}//** End of Monte Carlo iterations(Τέλος επαναλήψεων Monte Carlo) **

 
 m_mean1=m_mean1/(M-D);//  <m>
 m_mean2=m_mean2/(M-D);//  <m^2>
 printf("avg_cluster= %d\n",avg_cluster);
 avg_cluster=avg_cluster/(M-D);
 printf("<m>  =%f\n",m_mean1);
 printf("<m2> =%f\n",m_mean2);
 printf("Average ## of SW clusters = %d\n",avg_cluster);

 //start of the calculation for spin-spin correlation
 for(i=1;i<=N;i++)//Calculate spin correlation function G (υπολογισμός συνάρτησης συσχέτισης σπιν, G
 {
  for(j=1;j<=NB;j++)
  {
   C[i][j]=C[i][j]/(M-D);//probability that i,j belong to the same cluster(πιθανότητα τα i,j να ανήκουν στο ίδιο cluster)
   G[i][j]=((q-1)*C[i][j]+1)/q;//spin-spin correlation function G(i,j) calculation(Υπολογισμός συνάρτησης συσχέτισης σπιν-σπιν G(i,j)
  }
 }
 //end of the calculation for spin-spin correlation

return(m_mean2-m_mean1*m_mean1);//function is returning the value of magnetic susceptibility density for the given temperature T( Η συνάρτηση επιστρέφει την τιμή της πυκνότητας μαγνητικής επιδεκτικότητας χ για δεδομένη θερμοκρασία Τ)
}//End of function SW

int HK(int cluster[N+1],int nbors[][NB+1],int bond[][NB+1])
{
 //this function gives a cluster[i] label for every point i, so that we know which cluster i belongs to(η συνάρτηση δίνει μια ετικέτα cluster[i] στο κάθε σημείο i ώστε να αναγνωρίζουμε σε ποιο cluster ανήκει το i)
 int i,j,n,ns,idx,ncluster,stack[N+1];
 
 for(i=0;i<=N;i++)	//Initialization of variables (Αρχικοποίηση μεταβλητών)
 {			//
  stack[i]=0;		//
  cluster[i]=0;		//
 }			//
 ncluster=0;		//-------------

 for(i=1;i<=N;i++)	
 {
  if(cluster[i]==0)//if point i not labeled(αν το i δεν έχει ετικέτα)
  {                
   ns=1;         	  //Stack index(Δείκτης στης "στοίβας")
   ncluster++;	  	  //increase the cluster number(Αυξησε τον αριθμό του cluster )
   cluster[i]=ncluster;	  //current cluster is set to the cluster number (ncluster)(το παρόν cluster (cluster[i]) είναι ο αριθμός του cluster πχ. 1o cluster, 2o cluster κλπ. (ncluster) )
   stack[ns]=i;		  //set the "stack" to the current point "i" (αντιστοιχίζω τη στοίβα στο τρέχον σημείο i)
   while(ns>=1)		  //while ns is not 0 (όσο ο αριθμός ns δε μηδενίζεται)
    {
     idx=stack[ns];	  //index idx is set to the value of stack (ο δείκτης idx ισούται με την τιμή της στοίβας)
     ns--;		  //devreasing ns (μειώνοντας τον αριθμό ns)
     for(j=1;j<=NB;j++)   //for every neighbor NB(για κάθε γείτονα NB)
      {
       n=nbors[idx][j];	  //the j-th neighbor of the index idx is n. (ο j-οστός γείτονας του δείκτη idx είναι το n.)
       if(n!=0&&bond[idx][j]==1&&cluster[n]==0)
        {		  //If : n is not 0 AND idx,j are bonded AND n doesn't belong to a cluster (Εάν: -ο n δεν είναι 0 ΚΑΙ τα idx,j συνδέονται με δεσμό ΚΑΙ το n ΔΕΝ ανήκει σε cluster)
         cluster[n]=ncluster;//τότε θέτουμε ως ετικέτα του n τον αριθμό ncluster, τον ίδιο τον οποίο θέσαμε ως ετικέτα του i
         ns++;		  //increasing ns (αυξάνουμε τον αριθμό ns)
         stack[ns]=n;     //and setting stack of ns equal to n=nbors[idx][j] (και θέτουμε τη "στοίβα" του ns ίση με το n=nbors[idx][j])
        }
			  //in the end the neighbors of i will run out and the ns will become 0 , so the process will continue for all points , until they all have a cluster label. (Κάποτε θα τελειώσουν οι γείτονες του i με τους οποίους μπορεί έχει κάνει bond και οι οποίοι δεν ανήκαν πριν σε κάποιο cluster οπότε το ns θα μηδενιστεί και θα συνεχιστεί η διαδικασία για όλα τα σημεία, ώστε όλα να αποκτήσουν ετικέτες.)
      }
    }
  }
 }
 //Now, all points have a cluster label. (Στο τέλος, όλα τα σημεία έχουν και από μια ετικέτα του αντίστοιχου cluster στο οποίο βρίσκονται.)
 return ncluster;
}//End of function HK

int final_cluster(int k,int q,double th,int nbors[][NB+1],int bond[][NB+1],int cluster[N+1],int cluster_data[][S+1+1],double G[][NB+1])
{
 int i,j,m,n,ii,jj,num_cluster;
 double Gmax[N+1];
 
 //start initializations(αρχή αρχικοποιήσεων)
 for(i=0;i<=N;i++)//for all points(για όλα τα σημεία)
 {
  for(j=0;j<=NB;j++)//and for all their neighbors(και για όλους τους γείτονές τους)
  {
   bond[i][j]=0;//initialize the bonds(αρχικοποιούμε τους δεσμούς)
  }
  
  Gmax[i]=0.0;//Initialization (Αρχικοποιήσεις)
  cluster[i]=0;
  cluster_data[i][k]=0;
 }
 //end initializations(τέλος αρχικοποιήσεων)

 for(i=1;i<=N;i++)//for all points(για όλα τα σημεία)
 {
  for(j=1;j<=NB;j++)//and for all their neighbors(και για όλους τους γείτονές τους)
  {
   if(nbors[i][j]==0)//if i,j not neighbors(αν i,j δεν είναι γέιτονες)
   {
    break;//next command-keep i,j(επόμενη εντολή - κρατώ τα i,j)
   }
    
   if(G[i][j]>=th)//if the condition G(i,j)>=th is true (αν ικανοποιείται η συνθήκη G(i,j)>=th)
   {
    bond[i][j]=1;//Put a bond between i and j.(βάζουμε δεσμό στα i,j)
    n=nbors[i][j];//n is the j-th neighbor of i.(το n είναι ο j-οστός ge;itonaw toy i.)
    for(m=1;m<=NB;m++)
    {
     if(nbors[n][m]==i)//if i is the m-th neighbor of n(Αν το i είναι ο m-οστός γείτονας του n)
     {
      bond[n][m]=1;//then put also a bond between n and m(τότε τοποθετούμε δεσμό ανάμεσα στο n και στο m)
      break;//next command(επόμενη εντολή)
     }
    }
   }

   if(G[i][j]>Gmax[i])//Finding the maximal spin-spin correlation Gmax between points i,j(βρίσκουμε τη μέγιστη τιμή συσχέτισης σπιν-σπιν Gmax μεταξύ των shme;ivn i,j)
   {
    Gmax[i]=G[i][j];
    ii=j;
    jj=nbors[i][j];
   }
  }
  if(Gmax[i]>(double)1/q)//if Gmax is bigger than 1/q
  { 
   bond[i][ii]=1;//put a bond between i,j of *maximal correlation* G(i,j)-->Gmax (τοποθετούμε δεσμό ανάμεσα στα i,j με τη μέγιστη συνάρτηση συσχέτισης G(i,j)-->Gmax)
   for(m=1;m<=NB;m++)//searching the neighbors of jj(ψάχνουμε τους γείτονες του jj)
   {
    if(nbors[jj][m]==i)
    {
     bond[jj][m]=1;//if i is the m-th neighbor of jj, put a bond between jj and m(αν ο i είναι ο m-ιοστός γείτονας του jj βάζουμε δεσμό ανάμεσα στα jj,m)
     break;//next command(επόμενη εντολή)
    }
   }
  }
 }

 //call HK for labeling(Κλήση της HK για το labeling των clusters)
 num_cluster=HK(cluster,nbors,bond);
 //---------------------------------------------------------------

 for(i=1;i<=N;i++)//setting the cluster data(συμπλήρωση δεδομένων clustering)
 {
  cluster_data[i][k]=cluster[i];
 }
 return num_cluster;
}//End of function final_cluster


int find_ClusterSize(int k,int cluster[N+1],int fcluster[S+1+1],int cluster_size[][B+1])
{//This function identifies the final clusters (Αυτή η συνάρτηση αναγνωρίζει τα τελικά clusters)

 int i,j;
 double temp[fcluster[k]];

 //Initialization(Αρχικοποίηση)
 for(i=0;i<=fcluster[k];i++)
 temp[i]=0.0;
 for(i=1;i<=B;i++)    
 cluster_size[k][i]=0;
 //----------------------------

 //Find the size of "B" Biggest clusters(βρίσκουμε το μέγεθος καθενός από τα B μεγαλύτερα clusters)
 if(fcluster[k]==1)
 {
 cluster_size[k][1]=N;
 for(i=2;i<=B;i++)
 cluster_size[k][i]=0;
 }

 else
 {
  for(i=1;i<=fcluster[k];i++)
  temp[i]=0.0;

  for(i=1;i<=N;i++)
  temp[cluster[i]]++;

  quicksort(temp,0,fcluster[k]);//Sorting the array temp (Ταξινομούμε τη μεταβλητή temp)
  j=(fcluster[k])>B ? B:  fcluster[k];// ("?"=Ternary operator) If (fcluster[k])>B then j=B, else j=fcluster[k] (Εάν ισχύει η παρένθεση, τότε j=B αλλιώς j=fcluster[k])
  for(i=1;i<=j;i++)
  cluster_size[k][i]=(int)temp[fcluster[k]+1-i];
 }

 for(i=1;i<=B;i++)
 printf("cluster_size[%d][%d]=%d\n",k,i,cluster_size[k][i]);
 printf("\n");
 return 0;
}//End of find_Clustersize



int output(double lattice[][DM+1],char filename[])
{
 FILE*pixmap=NULL/*Ορισμός αρχείου*/,*fp6=NULL;
 int X=100/*columns(στήλες)*/,Y=100/*rows(γραμμές)*/,i,j/*Μετρητές*/,potts_color[X-0][Y-0]/*Πίνακας προβολής πάνω στο επίπεδο που ορίζεται από το μήκος πετάλων και το μήκος σεπάλων*/,a;//we can control manually the width of the axis(μπορούμε να ορίσουμε και το πλάτος του άξονα, ανάλογα με το κάθε πρόβλημα)
 double regain=(1.0);//are X,Y coordinates scaled? then define rescaling here(είναι σε κλίμακα οι συντεταγμένες Χ,Υ; αν ναι, τότε επαναφορά τους σε αρχική κλίμακα)
 
 fp6=fopen("Iris01.dat","r");//read the initial data file(διαβάζουμε το αρχικό αρχείο δεδομένων)
 /*Άνοιγμα αρχείου*/
 pixmap=fopen(filename,"w");
 /*Έλεγχος έγκυρου ανοίγματος αρχείου*/

 rewind(fp6);//rewinding file(τοποθέτηση δείκτη αρχείου στην αρχή)
 i=1;j=1;
 while(fscanf(fp6,"%lf",&lattice[i][j])!=EOF)
 {
  if(j==DM-1||j==DM)
  {
   lattice[i][j]=lattice[i][j]*regain;//rescaling if we need(αλλαγή κλίμακας, αν θέλουμε)
   lattice[i][j]=(int)lattice[i][j];
  }
  else
  lattice[i][j]=(int)lattice[i][j];
  j++;
  if(j==DM+1)
  {
   i++;
   j=1;
  }//Read data from file(Ανάγνωση δεδομένων από το αρχείο)
 }
 for(i=0;i<N;i++)
  {
   lattice[i][0]=s[i];//first coordinate of every lattice[i] is the potts state s[i](η πρώτη συντεταγμένη του κάθε lattice[i] είναι η Potts κατάσταση s[i])
  }

 fclose(fp6);
 if(!pixmap)
 {
  printf("Σφάλμα ανοίγοντας το αρχείο %s",filename);
  return 1;
 }
 /*Ορισμός αρχικής θέσης στην αρχή του αρχείου*/
 rewind(pixmap);
 /*Αρχικοποίηση του πίνακα δισδιάστατης προβολής με μηδέν*/
 for(i=0;i<Y-0;i++)
 {
  for(j=0;j<X-0;j++)
  {
   potts_color[i][j]=0;
  }
 }
 /*Απόδοση σε όλα τα σημεία δεδομένων της τιμής κατάστασης Potts τους*/
 for(i=0;i<N;i++)
 {
  potts_color[(int)lattice[i][1]-0][(int)lattice[i][3]-0]=lattice[i][0];
 }
 /*Δημιουργία απαραίτητης επικεφαλίδας αρχείου*/
 fprintf(pixmap,"P3 %d %d %d\n",/*X*/X-0,/*Y*/Y-0,q);
 /*Διατρέχοντας ολόκληρο τον πίνακα διδιάστατης προβολής*/
 for(i=0;i<Y-0;i++)/*For all rows*/
 {
  for(j=0;j<X-0;j++)/*For all columns*/
  {
   /*Έλεγχος εάν είναι σημείο δεδομένων*/
   if(potts_color[i][j]==0)
   {
    /*Printing in black(Τύπωση στο αρχείο με μαύρο χρώμα)*/
    fprintf(pixmap,"   %d   %d   %d",potts_color[i][j],potts_color[i][j],potts_color[i][j]);
   }
   /*Check if potts state is q(Έλεγχος εάν η τιμή κατάστασης Potts είναι q)*/
   else if(potts_color[i][j]==q)
   {
    /*Τύπωση στο αρχείο με λευκό χρώμα*/
    fprintf(pixmap," %d %d %d",potts_color[i][j],potts_color[i][j],potts_color[i][j]);
   }
   /*Check if q is less than q and more than 3q/4(Έλεγχος εάν η τιμή κατάστασης Potts είναι μικρότερη του q και μεγαλύτερη ή ίση του 3q/4)*/
   else if(potts_color[i][j]>=(int)(3*q/4)&&potts_color[i][j]<q)
   {
    /*Print in yellow (Τύπωση στο αρχείο σε κίτρινη απόχρωση*/
    fprintf(pixmap,"  %d  %d  10",potts_color[i][j],potts_color[i][j]);
   }
   /*Check if potts state less than 3q/4 and more than q/2(Έλεγχος εάν η τιμή κατάστασης Potts είναι μικρότερη του 3q/4 και μεγαλύτερη ή ίση του q/2)*/
   else if(potts_color[i][j]>=(int)(q/2)&&potts_color[i][j]<(int)(3*q/4))
   {
    /*Print in red(Τύπωση στο αρχείο σε κόκκινη απόχρωση)*/
    fprintf(pixmap,"  %d   0   0",potts_color[i][j]);
   }
   /*Check if potts state is less than q/2 and more than q/4(Έλεγχος εάν η τιμή κατάστασης Potts είναι μικρότερη του q/2 και μεγαλύτερη ή ίση του q/4)*/
   else if(potts_color[i][j]>=(int)(q/4)&&potts_color[i][j]<(int)(q/2))
   {
    /*Print in green(Τύπωση στο αρχείο σε πράσινη απόχρωση)*/
    fprintf(pixmap,"   0  %d   0",potts_color[i][j]);
   }
   /*Check if potts state is less than q/4 and more q/10(Έλεγχος εάν η τιμή κατάστασης Potts είναι μικρότερη του q/4 και μεγαλύτερη ή ίση του q/10)*/
   else if(potts_color[i][j]>=(int)(q/10)&&potts_color[i][j]<(int)(q/4))
   {
    /*Print in blue(Τύπωση στο αρχείο σε μπλε απόχρωση)*/
    fprintf(pixmap,"   0   0  %d",potts_color[i][j]);
   }
   /*Check if potts state is less than q/10(Εάν η τιμή κατάστασης Potts είναι μικρότερη του q/10)*/
   else
   {
    /*Print in purple(Τύπωση στο αρχείο σε μωβ-βυσσινί απόχρωση)*/
    fprintf(pixmap,"   %d   0   %d",potts_color[i][j],potts_color[i][j]);
   }
   /*New line9Δημιουργία νέας γραμμής)*/
   if(j==X-0-1)fprintf(pixmap,"\n");
  }
 }
 /*Close file(Kλείσιμο αρχείου)*/
 fclose(pixmap);
 return 0;
}
