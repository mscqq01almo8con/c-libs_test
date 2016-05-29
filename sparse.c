#include <stdio.h>
#include <stdlib.h>

#define N 10 //Max number of vertices in our graph.

typedef struct node *ptype;
struct node
{
 unsigned short int edge;
 unsigned short int x;
 unsigned short int y;
 unsigned short int spanning; //if an edge becomes a member of the spanning tree, we mark it, in order to clear out non-spanning tree vertices (cycles)
 ptype n,p,d,u;
};
typedef struct node node;

struct qnode
   {
     int data;
     struct qnode *link;
   };

//searches for a node (from a horizontal head(fixed i) it traverses the x axis (j) ).
int find_xy(ptype *headh,ptype *tailh,int j,int i,char param)
{
 node *aux;

 aux=NULL;
 if(j<N&&i<N)
 {
   aux=headh[i];
   if(aux==NULL)
    return 0;
   else
   {
    while(aux->n!=NULL&&aux->x<j)
    {
     aux=aux->n;
    }
    if(aux->x==j)
    {
     if(param=='s')		//found the edge, is it part of spanning tree?
      return aux->spanning;
     if(param=='e')		//found the edge, it is 1
      return aux->edge;
    }
    else return 0;//not found!
   }
 }
}

void initialize(node **a,node **b,node **c,node **d)
{
   *a=NULL;*b=NULL;*c=NULL;*d=NULL;
    if(*a==NULL&&*b==NULL&&*c==NULL&&*d==NULL);
}

//inserts a node in the right place on x axis.
void insertx(ptype *head,ptype *tail,int data,node *temp)
{
 ptype temp_h,temp_t,aux;
 temp_h=*head;
 temp_t=*tail;
 
 if(temp_h!=NULL)
 {
  if(temp->x < temp_h->x) 
  {
   temp->n=temp_h;
   temp_h=temp;
  }
  if(temp->x>temp_t->x)   
  {
   temp_t->n=temp;
   temp->p=temp_t;
   temp_t=temp;
  }
  else if((temp_h->x<temp->x)&&(temp->x<temp_t->x))
  {
   aux=*head;
   while(aux->n->x<temp->x)
    aux=aux->n;
   if(temp->x!=aux->n->x)
   {
    temp->n=aux->n;
    aux->n=temp;
    temp->p=aux;
   }
  }
 }
 else if(temp_h==NULL)
 {
  temp_h=temp;
  temp_t=temp;
 }
 
 *head=temp_h;
 *tail=temp_t;
}
//inserts a node in the right place on y axis.
void inserty(ptype *head,ptype *tail,int data,node *temp)
{
 ptype temp_h,temp_t,aux;
 temp_h=*head;
 temp_t=*tail;
 if(temp_h!=NULL)
 {
  if(temp->y < temp_h->y) 
  {
   temp->d=temp_h;
   temp_h=temp;
  }
  if(temp->y > temp_t->y)   
  {
   temp_t->d=temp;
   temp->d=temp_t;
   temp_t=temp;
  }
  else if((temp_h->y<temp->y)&&(temp->y<temp_t->y))
  {
   aux=*head;
   while(aux->d->y<temp->y)
    aux=aux->d;
   if(aux->d->y!=temp->y)
   temp->d=aux->d;
   aux->d=temp;
   temp->u=aux;
  }
 }
 else if(temp_h==NULL)
 {
  temp_h=temp;
  temp_t=temp;
 }
 *head=temp_h;
 *tail=temp_t;
}

//deletes a node in x axis, if it exists
node *deletex(ptype *head,ptype *tail,int data)
{ 
 ptype aux,temp,temp2,temp_h,temp_t;
 temp_h=*head;
 temp_t=*tail;
 temp=NULL;
 if(temp_h==NULL)						//void list.
 {
  return NULL;
 }
 if(temp_h->x>data||data>temp_t->x)				//out of range
 {
  return NULL;
 }
 if(temp_h!=NULL)
 {
  if(temp_h->x==data) 						//delete 1st
  {  
   if (temp_h->n==NULL)
   {
    temp=temp_h;
    *head=NULL;*tail=NULL;temp2=NULL;
   }
   else
   {
    temp=temp_h;
    temp_h=temp_h->n;
    temp_h->p=NULL;
    *head=temp_h;
    temp2=temp_h->p;
   }
   printf("Inserted number was first in list.Deleted.\n");
  }
  else
  {
   aux=temp_h;
   while(aux->n->x<data&&aux->n->n!=NULL)
   {
    aux=aux->n; 
   }
   if(aux->n->n==NULL)
   {
    printf("I found the last item.\n");					//delete last
    temp=aux->n;
    temp_t=aux; 
    *tail=temp_t;
    aux->n=NULL;
    temp2=aux;
   }
   else
   {
    printf("Item somewhere in between the list.\n");			//delete in-between
    temp=aux->n;
    aux->n=aux->n->n;
    temp2=aux;
    aux->n->p=temp2;
   }
  }
 }
 return (temp);
}

//deletes a node in y axis, if it exists
node *deletey(ptype *head,ptype *tail,int data)
{ 
 ptype aux,temp,temp2,temp_h,temp_t;
 temp_h=*head;
 temp_t=*tail;
 temp=NULL;
 if(temp_h==NULL)						//void list
 {
  return NULL;
 }
 if(temp_h->y>data||data>temp_t->y)				//out of range
 {
  return NULL;
 }
 if(temp_h!=NULL)
 {
  if(temp_h->y==data) 						//deleting 1st unique node
  {  
   if (temp_h->d==NULL)
   {
    temp=temp_h;
    temp=NULL;
    *head=NULL;
    *tail=NULL;
    temp2=NULL;
    }
   else
   {
    temp=temp_h;
    temp_h=temp_h->d;
    temp_h->u=NULL;
    *head=temp_h;
    temp=NULL;
    temp2=temp_h->u;
   }
   printf("Inserted number was first in list.\n");			//delete 1st of many
  }
  else
  {
   aux=temp_h;
   while(aux->d->y<data&&aux->d->d!=NULL)
   {
    aux=aux->d; 
   }
   if(aux->d->d==NULL)
   {
    printf("Found the last item.\n");					//delete last node
    temp=aux->d;
    temp_t=aux;
    *tail=temp_t;
    temp=NULL;
    aux->d=NULL;
    temp2=aux;
   }
   else
   {
    printf("Item somewhere in between the list.\n");			//delete in between
    temp=aux->d;
    aux->d=aux->d->d;
    temp=NULL;
    temp2=aux;
    aux->d->u=temp2;
   }
  }
 }

 return (temp);
}

//make a new memory record/cell
ptype setup_cell(ptype temp,int j,int i)
{
 temp->x=j;
 temp->y=i;
 temp->edge=1;
 temp->spanning=0;
 initialize(&temp->n,&temp->u,&temp->p,&temp->d);
 return(temp);
}

//insert xy first creates a new memory record, then calls insert for both x,y to insert the record. 
void insertxy(ptype *headh,ptype *tailh,ptype *headv,ptype *tailv,int j,int i)
{
    struct node *temp;

    if(j<N&&i<N)
    {
     temp=malloc(sizeof(struct node));//ftaxnw kouti node k dinw times sta dedomena
     setup_cell(temp,j,i);
     insertx(headh,tailh,j,temp); 
     inserty(headv,tailv,i,temp);printf("yelp!"); 
    }
    else
     printf("\nNode outside acceptable bounds: MAX=%d\n",N);
}

//delete xy calls delete for the x,y axis. deletes after the pointers are rearranged
void deletexy(ptype *headh,ptype *tailh,ptype *headv,ptype *tailv,int j,int i)
{
    node *temp;

    temp=NULL;
    if(j<N&&i<N)
    {
     printf("Deleting node...");
     if(!headh||!headv)
      printf("Not found.\n");
     else
     {
      temp=deletex(headh,tailh,j);
      temp=deletey(headv,tailv,i);
      if(temp!=NULL)free(temp);
     }
    }
    else
    printf("Node outside acceptable bounds: MAX=%d",N);
}

//add a value to the queue
struct qnode *addqueue(struct qnode *p,int val)
{
 struct qnode *temp;
 if(p == NULL)
 {
  p = (struct qnode *) malloc(sizeof(struct qnode)); /* insert the new node first node*/
  p->data = val;
  p->link=NULL;
 }
 else
 {
  temp= p;
  while(temp->link != NULL)
  {
   temp = temp->link;
  }
  temp->link = (struct qnode*)malloc(sizeof(struct qnode));
  temp = temp->link;
  temp->data = val;
  temp->link = NULL;
}
return(p);
}

//delete a value from a queue
struct qnode *deleteq(struct qnode *p,int *val)
{  
 struct qnode *temp;

 *val = p->data;
 temp = p;
 p = p->link;
 free(temp);
 return(p);
}

/*we visit every vertex (change the value of visited[i]) and enqueue all nodes(children) it connects to (breadth first search), then we dequeue and visit  each of the children, finding its own children, 'breadth first search'-way. we repeat this procedure until we've visited every vertex and we can't enqueue anything more. we keep track of parents and distance from the root*/
void bfs(ptype *headh,ptype *tailh,int n,struct qnode **p,int i,int j,int visited[],int parent[],int distance[])
{
 int y,k,x;
 ptype aux;

 aux=*headh;
 if(visited[i]==0)printf("\nBFS node: %d(root)\n",i);//first unvisited vertex gets to be the root.
 *p = addqueue(*p,i);
 do{
    *p=deleteq(*p,&x);			//dequeue all the children and find their own children
     if(visited[x] == 0)
     {
      visited[x] = 1;
      for(k=0;k<n;k++)		// checking every child
      {
       if( (find_xy(headh,tailh,x,k,'e')==1) && (visited[k] == 0) )// found a new child?
       {
        *p=addqueue(*p,k); 		//add child to queue. 
	if(parent[k]==-1) 
        {
	 printf("\nBFS node: %d\n",k);//if i haven't mentioned it before, print the current vertex.
         parent[k]=x;  		//setting vertex's father
         distance[k]=distance[x]+1; //... and distance from root.
         aux=headh[k]; 
         if(visited[k]==0)printf("\nParent of %d is: %d,distance:%d \n",k,parent[k],distance[k]);
         while(aux->n!=NULL&&aux->x<x)
          aux=aux->n;
         if ( aux->x==x&&x!=k)
         {
          aux->spanning=1;//add edge of current vertex and its parent to spanning tree 
         }
         aux=headh[x];
         while(aux->n!=NULL&&aux->x<k)
          aux=aux->n;
         if ( aux->x==k&&k!=x)
         { 
          aux->spanning=1;//add edge of current vertex and its parent to spanning tree (symmetric edge)
         }
        }
       }//end if
      } //end for
     }  //end if
   }while((*p) != NULL);
}

int main(void)
{
  ptype headh[N],headv[N],tailh[N],tailv[N],temp;
  struct qnode *p=NULL;
  int visited[N],parent[N],distance[N],number,item_x,item_y,data,i,j,root,a,b;
  char choice='*',del_cycles='n';


  for(i=0;i<N;i++)
  {
   initialize(&headh[i],&headv[i],&tailh[i],&tailv[i]);
   visited[i]=0;
   distance[i]=0;
   parent[i]=-1;
  }
  while(choice!='q'&&choice!='Q')
  {
   printf("insert, delete, print, find(x,y), breadth-first-search, cycles, quit\n");
   scanf("%c",&choice);
   if(choice=='i')
   {
                  
    printf("insert node coordinates:");
    scanf("%d %d",&j,&i);
                  insertxy(&headh[i],&tailh[i],&headv[j],&tailv[j],j,i);
       if(i!=j)   insertxy(&headh[j],&tailh[j],&headv[i],&tailv[i],i,j);// its symmetric edge (1-4) <-> (4-1)
              
   /* for (i=1;i<=9;i++)
      for(j=i+1;j<9;j++)
      {
                     
                  insertxy(&headh[i],&tailh[i],&headv[j],&tailv[j],j,i);
       if(i!=j)   insertxy(&headh[j],&tailh[j],&headv[i],&tailv[i],i,j);// its symmetric edge (1-4) <-> (4-1)
      }
    */
   }
   if(choice=='d')
   {
    printf("mark node for deletion:");
    scanf("%d %d",&j,&i);
    		 deletexy(&headh[i],&tailh[i],&headv[j],&tailv[j],j,i);
     if(i!=j)    deletexy(&headh[j],&tailh[j],&headv[i],&tailv[i],i,j);// its symmetric edge (1-4) <-> (4-1)
   }
   else if(choice=='p')
   {
    printf("\n %s\n","Printing adjacency matrix:\n");
    for(i=0;i<N;i++)
    { 
     for(j=0;j<N;j++)
      printf("%d ",find_xy(&headh[0],&tailh[0],j,i,'e'));
     printf("\n");   
    }
   }
   else if(choice=='f')
   {
    printf("To search for a node, insert node coordinates:");
    scanf("%d %d",&j,&i);
    if(i>=N||j>=N||i<0||j<0)
    {
     printf("Please provide coordinates within acceptable bounds(MAX=%d):",N);
    }
    printf("\nLink between %d,%d is: %d\n",j,i,find_xy(&headh[0],&tailh[0],j,i,'e'));
    printf("Its visited value is:%d\n",find_xy(&headh[0],&tailh[0],j,i,'s') );
   }
   else if(choice=='b')
   {//temp=headh[0];temp->visited=1;
   printf("\nBefore bfs search, select the root:");
   scanf("%d",&root);
    for(i=root; i<N; i++)
    {
     if(i==N)i=0;
     if(visited[i] ==0&&headh[i]!=NULL)
      bfs(&headh[0],&tailh[0],N,&p,i,0,visited,parent,distance);    
      
      
    }
    for(i=0;i<N;i++)
     visited[i]=0;
   }
   else if(choice=='c')
   {/*
    Whichever node has a 'spanning' value not equal to 1, doesn't belong to spanning tree, therefore it forms a cycle.
    */
    printf("Cycles will be displayed. Also delete cycles?(y/n)\n");
    scanf(" %c",&del_cycles);
    for(i=0;i<N;i++)
     for(j=0;j<i;j++)			//i, not N, because we don't need the cycle duplicates (symmetric ones at adj. matrix)
     {
      if(  find_xy(&headh[0],&tailh[0],j,i,'e')==1 &&
           find_xy(&headh[0],&tailh[0],j,i,'s')!=1   )
      {
       printf("\n Edge (%d,%d) forms a cycle.  \n",j,i); 
       printf("\n Tracking common parent of %d , %d back:",j,i);
       a=j,b=i;//keeping i,j to display them later
       while (a!=b&&a!=root&&b!=root)
       {
        b=parent[b];
        //if(a==b) break;
        a=parent[a];
        printf("[%d]",b);
        printf("[%d]",a);
       }
       /*while (a!=b&&b!=root)
       {
        b=parent[b];
        printf("[%d]",b);
       }
       while (a!=b&&a!=root)
       {
        a=parent[a];
        printf("[%d]",a);
       }*/
       printf("\n\n----The cycle is formed between: %d %d and common ancestor %d .----\n\n",i,j,a);
      }
     }
   }
 }
 return 0;
}
