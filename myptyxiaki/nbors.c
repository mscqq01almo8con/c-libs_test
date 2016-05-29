/*

			<  nbors.c  >

Σκοπός του προγράμματος:  Βρίσκει τους Κ-κοντινότερους γείτονες
για το κάθε σημείο και βρίσκει και τυχόντες επιπρόσθετους γείτονες
οι οποίοι αντιστοιχούν στο Ελάχιστο Συνδετικό Δέντρο(MST). 

Είσοδος:                  Το αρχείο γειτόνων .dat που περιέχει τα δεδομένα των σημείων

Έξοδος:                   Το αρχείο .dat που περιέχει τους 'Κ' κοντινότερους γείτονες μαζί με τους 'Η' επιπρόσθετους γείτονες από τον αλγόριθμο του Kruskal για την εύρεση του MST

---------------------------------------------------------------------------------

Program objective:          Find the K-nearest neighbors for all the points(vectors) and
also any more potential neighbors by found by Minimal SPanning Tree (MST)
Input:                      A file formatted as .dat containing the input vector data
Output:                     A file formatted as .dat containing the 'K' nearest neighbors, plus the 'H' neighbors retrieved by Kruskal's algorithm for finding the MST/ 

*/

#include <stdio.h> 
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "quicksort.c" 

#define N  150 		  // # of Vectors (Αριθμός διανυσμάτων(δηλ. σημείων)  )
#define DM 4 		  // # of Dimensions (Αριθμός διστάσεων του κάθε σημείου)
#define K 5 		  // # of Mutual Neighbors (Αριθμός αμοιβαίων γειτόνων)
#define H 50  	          // # of Additional list for Neighbors (Αριθμός πρόσθετης
			  //   λίστας γειτόνων μέσω MST)
#define R 1000            //Buffer size (Μέγεθος buffer)

double data[N+1][DM+1],dist[N+1],dist1[R+1];

int nbors[N+1][K+H+1],graph[N+1],vtx[N+1],mark[R+1],elist[N+1],edge[2+1][N-1+1],idx[2+1][R+1];

int sort;      //Sort Neighbors List
int mst;       //Include Neighboring Points from MST (συμπεριλαμβάνοντας πρόσθετα σημεία μέσω του minimal spanning tree
int interval;  //Distance Interval Division (βήμα διαμέρισης απόστασης)


int find_neighbors(double data[][DM+1],int nbors[][K+H+1]);

int MST(int idx1[1+1],int idx2[1+1], double th_d,double th_u, double max_dist, double data[][DM+1], int graph[N+1], double dist1[R+1], int mark[R+1], int idx[][R+1], int edge[][N-1+1]);

int main(void)
{
 int i,j,k,idx;
 double mindist,maxdist,temp[K+H+1];

	//Άνοιγμα του "Ιris.dat" μόνο για ανάγνωση και του "nbors_iris.dat" για ανάγνωση και εγγραφή.
 FILE *fp1=fopen("Iris01.dat","r"),*fp2=fopen("nbors_iris.dat","w");

 sort=1;		/*sort=1 αν θέλουμε να ταξινομήσουμε τους γείτονες, αλλιώς sort=0*/
 mst=1;          /*mst=1 αν θέλουμε να βρούμε επιπρόσθετους γείτονες μέσω MST*/
 interval=50;

/*Read data from file(Διαβάζουμε τα δεδομένα από το αρχείο)*/

	/*=================*/
	/*=================*/

 for(i=0;i<=K+H;i++)
 temp[i]=0.0;

 if(fp1==NULL)//Check for file existence
 {
  printf("Unable to open file for reading\n");
  exit(1);
 }

 i=1;j=1;
/*We read the data, according to their dimensionality DM (Διαβάζουμε τα δεδομένα, λαμβάνοντας υπόψιν τη διάσταση DM του χώρου)*/
 while(fscanf(fp1,"%lf",&data[i][j])!=EOF)
 {
  j++;
  if(j==DM+1)
  {
   i++;
   j=1;
  }
 }
 printf("\nData reading complete\n");
 fclose(fp1);//Κλείσιμο αρχείου

/*Find Neighbors (Εύρεση γειτόνων)*/

	/*=================*/
	/*=================*/

 for(i=1;i<=N;i++)//For all points(για όλα τα σημεία)
 {
  for(j=1;j<=K+H;j++)//For all neighbors(για όλους τους γείτονές τους)
  {
   nbors[i][j]=0;//Initialize neighbors(αρχικοποίηση τιμών γειτόνων)
  }
 }
 printf("\nBegining to find neighbors\n"); 

 //Call "find_neighbors function"
 find_neighbors(data,nbors);
 //------------------------------

 printf("Find Neighbors Complete\n");

 /* Sort neighbors (Ταξινόμηση γειτόνων)*/
 /*=====================================*/

 for (i=1;i<=N;i++)//For all points(Για όλα τα σημεία)
 {
  for(j=1;j<=K+H;j++)//For all neighbors(Για όλους τους γείτονές τους)
  {
   temp[j]=nbors[i][j];//Relocate the nbors[i][j] in temp[j] variable
   nbors[i][j]=0;
  }
  if(sort==0)//If we won't sort the neighbors(Αν δε θα ταξινομήσω τους γείτονες)
  {
   idx=1;
   for(j=1;j<=K+H;j++)//For every neighbor of the current point(Για κάθε γείτονα του τρέχοντος σημείου)
   {
    if(temp[j]!=0)//if mutual neighbor (Αν είναι αμοιβαίος γείτονας)
    {
     nbors[i][idx]=temp[j];//then set temp[j] as the "idx-th" neighbor of current point (τότε θέτω ως "idx-ιοστό" γείτονα του i τον temp[j] ) 
     idx++;
    }
   }
  }

  if(sort==1)//If we'll sort the neighbors(Αν θέλουμε να ταξινομήσουμε τους γείτονες)
  {
   quicksort(temp,1,K+H);//Calling quicksort to sort the temp list of the current point (Καλούμε την quicksort που θα ταξινομήσοει τη λίστα temp του τρέχοντος σημείου)
   for(j=1;j<=K+H;j++)
   {
    if(temp[j]!=0)//if mutual neighbor (Αν είναι αμοιβαίος γείτονας)
    break;        //stop at current j (σταμάτα στο τρέχον j)
   }
   idx=1;
   for(k=j;k<=K+H;k++)//for k=j(where we stopped previously) (για k=j, όπου σταματήσαμε προηγουμένως)
   {
    nbors[i][idx]=(int)temp[k];//then set temp[k] as the "idx-th" neighbor of current point (τότε θέτω ως "idx-ιοστό" γείτονα του i τον temp[k] ) 
    idx++;
   }
   for(k=idx;k<=K+H;k++)
   {
    nbors[i][k]=0;
   }
  }
 }//loop  end

	
	/*=================*/
	/*=================*/


	/*Write result to file*/

 k=K;
 for(i=1;i<=N;i++)
 {
  for(j=K+1;j<=K+H;j++)
  {
   if(nbors[i][j]==0&&j>k)
   {
    k=j-1;
    break;//find which is the last k for which the point i has a neighbor(βρες το τελευταίο k για το οποίο έχει γείτονες το τρέχον σημείο i )
   }
   if(nbors[i][j]==0&&j<k)
   {
    break;
   }
  }
 }

 printf("MAX # of Neighbors is %d\n",k);
 printf("Begining to write neighbors file:");
 for(i=1;i<=N;i++)
 {
  for(j=1;j<=k;j++)
  {
   fprintf(fp2,"%d ",nbors[i][j]);//writing the neighbors for each point(γράφουμε τους γείτονες του κάθε σημείου)
  }
  fprintf(fp2,"\n");//start writing the neighbors for a new point(ξεκινούμε γράφουμε τους γείτονες ενός νέου σημείου)
 }
 fclose(fp2);//Close file (Κλείσιμο αρχείου)

 return 0;
}//End of function "main" (τέλος της main)


//================================================================//


/*Function to find neighbors (Συνάρτηση εύρεσης γειτόνων)*/


//================================================================//



int find_neighbors(double data[][DM+1],int nbors[][K+H+1])
{
 int i,j,ii,jj,m,n,k,idx1[1+1],idx2[1+1],graph[N+1],vtx[N+1],mark[R+1],elist[N+1],edge[2+1][N-1+1],idx[2+1][R+1];
 double sum,th_d,th_u,inc,max_dist,temp,dist[N+1],dist1[R+1];

 printf("\nInitializing distances\n");
 
 for(i=0;i<=N;i++)//Initializing values(Αρχικοποίηση τιμών)
 {
  dist[i]=0.0;
  vtx[i]=0;
 }

 /*Find K-Nearest Neighbors(Εύρεση Κ-εγγύτερων γειτόνων)*/

 sum=max_dist=0.0;//Initialisation of sum and max distance(Αρχικοποίηση αθροίσματος και μέγιστης απόστασης.)
 printf("Computing distances between vectors\n");

 for(i=1;i<=N;i++)
 {
          //Progress message (Μύνημα προόδου)
  if(i==(int)N/4)printf("\n25 percent complete\n");
  if(i==(int)N/2)printf("\n50 percent complete\n");
  if(i==(int)3*N/4)printf("\n75 percent complete\n");
  if(i==(int)N)printf("\n100 percent complete\n");

  temp=0.0;//Initialization for temp(Αρχικοποίηση για την temp)
  for(j=1;j<=N;j++)//For all points (Για όλα τα σημεία)
  {
   for(k=1;k<=DM;k++)//For all the attributes[or vector coefficients] (για όλες τις ιδιότητες [ή τις συντεταγμένες του διανύσματος]
   {
    sum+=(data[i][k]-data[j][k])*(data[i][k]-data[j][k]);//Calculate distance squared (Υπολογισμός τετραγώνου απόστασης)
   }

   dist[j]=sqrt(sum);//Square root of the squared distance (Τετ. ρίζα του τετραγώνου της απόστασης)
   if(dist[j]>max_dist)
   {
    max_dist=dist[j];//Find the maximum distance (Εύρεση της μέγιστης απόστασης)
   }
   vtx[j]=j;
   sum=0.0;
  }//loop end

  quicksort2(dist,vtx,1,N);
  if(vtx[1]!=i)
  {
   for(j=2;j<=N;j++)
   {
    if(vtx[j]==i)
    {
     temp=vtx[1];
     vtx[1]=vtx[j];
     vtx[j]=temp;
     break;
    }
   }
  }
 for(j=1;j<=K;j++)
 nbors[i][j]=vtx[j+1];
 }
 printf("Max distance=%f\n",max_dist);


	/*=================*/
	/*=================*/

 //Finding the K-mutual neighbors(Βρίσκουμε τους Κ-κοινούς γείτονες)
 for(i=1;i<=N;i++)
 {
  for(j=1;j<=K;j++)
  {
   m=nbors[i][j];
   for(k=1;k<=K;k++)
   {
    if(i==nbors[m][k])
    {
     break;
    }
   }
   if(k==K+1)
   {
    nbors[i][j]=0;
   }
  }
 }
 /*Find Minimum Spanning Tree with Kruskal's Algorithm (Εύρεση MST με τον αλγόριθμο του Kruskal*/
 if(mst==1)
 {
  printf(" MST is 'ON'  \n"); 
  printf("Memory Allocation Complete\n");
  for(i=0;i<=N-1;i++)
  {
   graph[i]=0;
   elist[i]=0;
   edge[0][i]=0;
   edge[1][i]=0;
   edge[2][i]=0;
  }
  graph[N]=0;
  elist[N]=0;

  inc=max_dist/(double)interval;
  printf("inc = %f\n",inc);
  idx1[1]=1;idx2[1]=1;
  for(k=1;k<=interval;k++)
  {
   m=0;
   th_d=inc*(k-1);
   th_u=inc*k;
   MST(idx1,idx2,th_d,th_u,max_dist,data,graph,dist1,mark,idx,edge);
   printf("# Cluster Idx = %d #Edges = %d\n",idx1[1],idx2[1]-1);
   printf("MST %d complete \n\n",k);

   for(i=1;i<=N;i++)
   {
    if(graph[i]!=1)
    {
     m++;
    }
   }
   //While there exists a graph[i]=0 m is increasing and graph is unconnected.(Όσο υπάρχει graph[i]=0 το m αυξάνεται και ο γράφος δεν είναι ακόμα συνδεδεμένος)
   //When all graph[i] are >0 then we have a connected graph (Όταν όλα τα graph[i]>0 τότε έχουμε ένα συνδεδεμένο γράφο
   printf("Unconnected= %d data\n",m);
   if(m==0)//Graph are connected (MST found) (Έχω συνδεδεμένο γράφο)
   {
    printf("Connected graph found\n");
    break;
   }
  }
  printf("idx2 = %d N-1 = %d\n",idx2[1],N-1);

 /*Find empty place in Array nbors(Εύρεση άδειας θέσης στους γείτονες)*/

	/*=================*/
	/*=================*/

 
  for(i=1;i<=N;i++)
  {
   for(j=1;j<=K+H;j++)
   {
    if(nbors[i][j]==0)
    {
     elist[i]=j;
     break;
    }
   }
  }
  
  for(i=1;i<=N-1;i++)//Update list of Neighbors
  {
   if(i==(int)N/4)printf("25 percent complete\n");
   if(i==(int)N/2)printf("50 percent complete\n");
   if(i==(int)3*N/4)printf("75 percent complete\n");
   if(i==(int)N-1)printf("100 percent complete\n");
   
   m=edge[1][i];
   n=edge[2][i];
   for(j=1;j<=K+H;j++)
   {
    if(nbors[m][j]==n)//Neighbors in the List(Γείτονες στη λίστα)
    {
     break;
    }
   }
   if(j==K+H+1)//Neighbors not in the List(Γείτονες εκτός λίστας)
   {
    ii=elist[m];
    nbors[m][ii]=n;
    for(k=ii+1;k<=K+H;k++)
    {
     if(nbors[m][k]==0)
     {
      elist[m]=k;
      break;
     }
    }
    if(k==K+H+1)
    {
     printf("Set larger array for neighbors !\n");
     exit(1);
    }
    jj=elist[n];
    nbors[n][jj]=m;
    for(k=jj+1;k<=K+H;k++)
    {
     if(nbors[n][k]==0)
     {
      elist[n]=k;
      break;
     }
    }
    if(k==K+H+1)
    {
     printf("Set larger array for neighbors! \n");
     exit(1);
    }
   }
  }
 }

 return 0;
}


//================================================================//


/*	  Function to find Minimal Spanning Tree []MST 
	(Συνάρτηση εύρεσης Ελάχιστου Συνδετικού Δέντρου[MST]) 	*/


//================================================================//

/*

		 Kruskal's Algorithm:

1.) Suppose we have a group of vertices v:V(G) where G is the graph(edges and vertices)
2.) We define a set S(v)<-{v} which contains all vertices v.
3.) We initialize a priority Q that contains all edges of graph G,using the weights as keys.
4.) A<-{} , A will ultimately contain the edges of the MST
5.) While A has less than n-1 edges do
6.) Let set S(v) contain v and S(u) contain u
7.) If S(v)!=S(u) then 
	-Add edge (u,v) to A
	-Merge S(v) and S(u)into one set i.e.union
8.) Return A

*/

int MST(int idx1[2]/*Graph Nodes (Κόμβοι γράφου)*/,int idx2[2]/*Graph Nodes (Κόμβοι γράφου)*/, double th_d,double th_u, double max_dist, double data[][DM+1], int graph[N+1], double dist1[R+1]/*Graph edge weights (Βάρη ακμών γράφου)*/, int mark[R+1], int idx[][R+1], int edge[][N-1+1])
{
 int i,j,k,m,ii,jj,I,J/*Counters (Μετρητές)*/,set,min;
 double sum,temp;
 
 for(i=0;i<=R;i++) //Initialization of variables (Αρχικοποίηση μεταβλητών)
 {
  mark[i]=i;
  dist1[i]=0.0;
  idx[1][i]=0;
  idx[2][i]=0;
 }

 m=1;
 sum=temp=0.0;	  //-------------------------------------------------------

 printf("Cluster Idx = %d # Edges = %d\n",idx1[1],idx2[1]-1);
 for(i=1;i<=N;i++)
 {
  for(j=i;j<=N;j++)
  {
   for(k=1;k<=DM;k++)
   {
    sum+=(data[i][k]-data[j][k])*(data[i][k]-data[j][k]);
   }
   temp=sqrt(sum);
   sum=0.0;
   if(temp>=th_d&&temp<th_u&&i!=j)
   {
    dist1[m]=temp;
    idx[1][m]=i;
    idx[2][m]=j;
    m++;
   }
  }
  if(m>R)break;
 }

 printf("Buffer=%d  R=%d\n",m,R);
 printf("th_d=%f  th_u=%f\n",th_d,th_u);

 if(m>R)//Check Buffer (έλεγχος buffer (προσωρινης βοηθητικής μνήμης ) )
 {
  printf("Use Larger Array or Decrease Threshold Increment\n");
  exit(1);
 }

 for(i=m;i<=R;i++)
 dist1[i]=max_dist;

 quicksort2(dist1,mark,1,R);// 

//-----			Starting Loop			-----//
 for(i=1;i<=m-1;i++)
 {
  ii=idx[1][mark[i]];
  jj=idx[2][mark[i]];
  
  if(graph[ii]==0&&graph[jj]==0)
  {
   graph[ii]=idx1[1];
   graph[jj]=idx1[1];
   edge[1][idx2[1]]=ii;
   edge[2][idx2[1]]=jj;
   idx1[1]++;
   idx2[1]++;
  }
  else if(graph[ii]==0&&graph[jj]!=0)
  {
   graph[ii]=graph[jj];
   edge[1][idx2[1]]=ii;
   edge[2][idx2[1]]=jj;
   idx2[1]++;
  }
  else if(graph[ii]!=0&&graph[jj]==0)
  {
   graph[jj]=graph[ii];
   edge[1][idx2[1]]=ii;
   edge[2][idx2[1]]=jj;
   idx2[1]++;
  }
  else if(graph[ii]!=graph[jj])
  {
   if(graph[ii]>graph[jj])
   {
    min=graph[jj];
    set=graph[ii];
   }
   else
   {
    min=graph[ii];
    set=graph[jj];
   }
   for(j=1;j<=N;j++)
   {
    if(graph[j]==set)
    graph[j]=min;
   }
   edge[1][idx2[1]]=ii;
   edge[2][idx2[1]]=jj;
   idx2[1]++;
  }
  else
  {
   continue;
  }
 }
//-----			Ending Loop			-----//
 return 0;//End of MST (Τέλος MST)
}
 

