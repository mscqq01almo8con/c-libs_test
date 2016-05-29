#include <stdio.h>
#include <stdlib.h>
#include ".\rbtree.h"
#define N 10 //Max number of vertices in our graph.
int root=-1;
void fflushnew(char *c);
typedef struct cEdge *pEdge;
typedef struct cEdge {int node_a,node_b;pEdge link;} cycleEdge;

struct qnode
   {
     int data,anode,bnode;     
     struct qnode *link;  
   };
typedef struct node *ptype;
struct node
{
 unsigned short int edge;
 unsigned short int x;
 unsigned short int y;
 unsigned short int spanning; //if an edge becomes a member of 
 //the spanning tree, we mark it, in order to clear out non-spanning tree vertices (cycles)
 ptype n,p,d,u;
};

struct nodetype
{
	ptype toEdges;
	int parent;
	int visited;
	int distance;
};

//searches for a node (from a horizontal head(fixed i) it traverses the x axis (j) ).
void reset_xy(struct nodetype head[],struct nodetype tail[],int j,int i)
{
 ptype aux;
 ptype headh=head[i].toEdges;
 ptype tailh=tail[i].toEdges;
 aux=NULL;
 if(j<N&&i<N)
 {
   aux=headh;
   if(aux==NULL)
    return ;
   else
   {
    while(aux->n!=NULL && aux->x<j)
     aux=aux->n;
    if(aux->x==j)
    	aux->spanning=0;
   }
 }
}
//searches for a node (from a horizontal head(fixed i) it traverses the x axis (j) ).
int find_xy(struct nodetype head[],struct nodetype tail[],int j,int i,char param)
{
 ptype aux;
 ptype headh=head[i].toEdges;
 ptype tailh=tail[i].toEdges;
 aux=NULL;
 if(j<N&&i<N)
 {
   aux=headh;
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





void insertx(ptype *head,ptype *tail,int data,struct node* temp)
{
 ptype temp_h,temp_t,aux;
 temp_h=*head;
 temp_t=*tail;
 
 if(temp_h!=NULL)
 {
  if(temp->x < temp_h->x) 
  {
   temp->n=temp_h;
   temp_h->p=temp;
   temp_h=temp;
  }
  if(temp->x > temp_t->x)   
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
void inserty(ptype *head,ptype *tail,int data,struct node* temp)
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
   temp->u=temp_t;
   temp_t=temp;
  }
  else if((temp_h->y<temp->y)&&(temp->y<temp_t->y))
  {
   aux=*head;
   while(aux->d->y<temp->y)
    aux=aux->d;
   if(aux->d->y!=temp->y)
   {
		 temp->d=aux->d;
		 aux->d=temp;
		 temp->u=aux;
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


void initialize(struct node **a,struct node **b,struct node **c,struct node **d)
{*a=NULL;*b=NULL;*c=NULL;*d=NULL;
}


//setup newly made memory record/cell
ptype setup_cell(struct node *temp,int j,int i)
{
 temp->x=j;
 temp->y=i;
 temp->edge=1;
 temp->spanning=0;
 initialize(&temp->n,&temp->u,&temp->p,&temp->d);
// if(temp->n||temp->u||temp->p||temp->d)
  // printf("\nNOT init!\n");
 return(temp);
}

//insert xy first creates a new memory record, then calls insert for both x,y to insert the record. 
void insertxy(ptype *headh,ptype *tailh,ptype *headv,ptype *tailv,int j,int i)
{
    struct node *temp;
     if(j<N&&i<N){
     temp=(struct node*)malloc(sizeof(struct node));//ftaxnw kouti node k dinw times sta dedomena
     setup_cell(temp,j,i);
     insertx(headh,tailh,j,temp); 
     inserty(headv,tailv,i,temp);
     printf("\nTemp: j=%d,i=%d\n",temp->x,temp->y);
    
    }
    else     printf("\nNode outside acceptable bounds: MAX=%d\n",N);
}

void printall(struct nodetype headh[],struct nodetype tailh[],char option)
{
	int i,j,k;
	if(option=='e' || option=='s')
	{
		printf("\n");
		for(i=0;i<N;i++)
		{		
			for(j=0;j<N;j++)
			{
				k=find_xy(headh,tailh,j,i,option);
				printf(" %d",k); 
			}
			printf("\n");
		}
		printf("\n");
	}
	else printf("\nWrong Option!\n");
}

void insertrandom(struct nodetype *lists[4],int n)
{
	struct nodetype *headh=(struct nodetype*)lists[0],
									*tailh=(struct nodetype*)lists[1],
									*headv=(struct nodetype*)lists[2],
									*tailv=(struct nodetype*)lists[3];
	int i,j,k;
	srand(time(NULL));
	
	for(k=0;k<n;k++)
	{
		i=rand()%N;j=rand()%N;
		insertxy(&(headh[i].toEdges),&(tailh[i].toEdges),&(headv[j].toEdges),&(tailv[j].toEdges),j,i);
		if(i!=j)  	
			insertxy(&(headh[j].toEdges),&(tailh[j].toEdges),&(headv[i].toEdges),&(tailv[i].toEdges),i,j);
	}
}

//deletes a node in x axis, if it exists
struct node *deletex(ptype *head,ptype *tail,int data)
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
struct node *deletey(ptype *head,ptype *tail,int data)
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

//delete xy calls delete for the x,y axis. deletes after the pointers are rearranged
void deletexy(ptype *headh,ptype *tailh,ptype *headv,ptype *tailv,int j,int i)
{
    struct node *temp;

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
      if(temp!=NULL)
      	free(temp);
     }
    }
    else
    printf("Node outside acceptable bounds: MAX=%d",N);
}

void deleteall(struct nodetype *lists[4])
{
	struct nodetype *headh=(struct nodetype*)lists[0],
									*tailh=(struct nodetype*)lists[1],
									*headv=(struct nodetype*)lists[2],
									*tailv=(struct nodetype*)lists[3];
	int i,j,k;
	for(k=0;k<N;k++)
		for(j=0;j<N;j++)
		{					
			for(i=0;i<N;i++)
			{
				deletexy(&(headh[i].toEdges),&(tailh[i].toEdges),&(headv[j].toEdges),&(tailv[j].toEdges),j,i);
				if(i!=j)  	
					deletexy(&(headh[j].toEdges),&(tailh[j].toEdges),&(headv[i].toEdges),&(tailv[i].toEdges),i,j);
			}
		}
}

//add a value to the queue
struct qnode *enqueue(struct qnode *p,int val)
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
struct qnode *dequeue(struct qnode *p,int *val)
{  
 struct qnode *temp;
 
	 if(p)
		{
		 *val = p->data;
		 temp = p;
		 p = p->link;
		 free(temp);}
	 else
	 	p=NULL;
 return(p);
}


void bfs(int s,struct nodetype *lists[4])
{
	struct nodetype *headh=(struct nodetype*)lists[0],*tailh=(struct nodetype*)lists[1],
									*headv=(struct nodetype*)lists[2],*tailv=(struct nodetype*)lists[3];
	int currentNode,i,j,k,child;
	struct nodetype source=headh[s];
	struct qnode *q=NULL;
	ptype aux ;
	root=s;
	
    for(i=0;i<N;i++)	//initialize all nodes.
	{
		headh[i].visited=0;
		headh[i].distance=-1;
		headh[i].parent=-1;
	}
	
	source.visited=1;//set source node: source parent is itself.
	source.distance=0;
	source.parent=s;//!!!!careful here! or else you'll have e.g. headh[-1]=ERROR
	q=enqueue(q,s);
	printf("= Check1 =");

	while(q)//while i have neighbors
	{
		q=dequeue(q,&currentNode);
		aux=headh[currentNode].toEdges;
		printf("= Check2 =");
//		aux points to the y row (is the parent) and we'll search on to it for x's (children)
		if(aux)
		{	
				while (aux)
				{
                    printf("= Check3 =");
					child=aux->x;
					printf("= Check4 =");
					if(headh[child].visited==0)
					{
						headh[child].visited=1;
						headh[child].distance=headh[currentNode].distance+1;
						headh[child].parent=currentNode;
						q=enqueue(q,child);
					}
					aux=aux->n;
				}
				headh[currentNode].visited=2;
				
				if(headh[currentNode].parent!=-1)
				{
					aux=headh[currentNode].toEdges;
				
					while (aux)
					{
						if (aux->x==headh[currentNode].parent)
								aux->spanning=1;
						aux=aux->n;
					}
					aux=headh[headh[currentNode].parent].toEdges;
					while (aux)
					{
						if (aux->x==currentNode)
								aux->spanning=1;
						aux=aux->n;
					}
					printf("= Check7 =");
				}//last if (parent setups)
		}//prev if (i has adjacent nodes )
	}//while dequeue i ( Q -> i )
}//end of BFS.


void initall(struct nodetype headh[],struct nodetype tailh[],struct nodetype headv[],struct nodetype tailv[])
{
	int i;
	for(i=0;i<N;i++)
		{
		 initialize( &(headh[i].toEdges),&(headv[i].toEdges),&(tailh[i].toEdges),&(tailv[i].toEdges) );
		 headh[i].visited=0;
		 headh[i].distance=0;
		 headh[i].parent=-1;
		 tailh[i].visited=0;
		 tailh[i].distance=0;
		 tailh[i].parent=-1;
		 headv[i].visited=0;
		 headv[i].distance=0;
		 headv[i].parent=-1;
		 tailv[i].visited=0;
		 tailv[i].distance=0;
		 tailv[i].parent=-1;
		}
}
void insertall(struct nodetype *lists[4])
{
	struct nodetype *headh=(struct nodetype*)lists[0],
									*tailh=(struct nodetype*)lists[1],
									*headv=(struct nodetype*)lists[2],
									*tailv=(struct nodetype*)lists[3];
	int i,j,k;
	for(k=0;k<N;k++)
		for(j=0;j<N;j++)
		{					
			for(i=0;i<N;i++)
			{
				insertxy(&(headh[i].toEdges),&(tailh[i].toEdges),&(headv[j].toEdges),&(tailv[j].toEdges),j,i);
				if(i!=j)  	
					insertxy(&(headh[j].toEdges),&(tailh[j].toEdges),&(headv[i].toEdges),&(tailv[i].toEdges),i,j);
			}
		}
}

void insert(struct nodetype *lists[4],int i,int j)
{
	struct nodetype *headh=(struct nodetype*)lists[0],
									*tailh=(struct nodetype*)lists[1],
									*headv=(struct nodetype*)lists[2],
									*tailv=(struct nodetype*)lists[3];
	insertxy(&(headh[i].toEdges),&(tailh[i].toEdges),&(headv[j].toEdges),&(tailv[j].toEdges),j,i);
	if(i!=j)  	
		insertxy(&(headh[j].toEdges),&(tailh[j].toEdges),&(headv[i].toEdges),&(tailv[i].toEdges),i,j);
}



void delete(struct nodetype *lists[4],int i,int j)
{
	struct nodetype *headh=(struct nodetype*)lists[0],
									*tailh=(struct nodetype*)lists[1],
									*headv=(struct nodetype*)lists[2],
									*tailv=(struct nodetype*)lists[3];
	deletexy(&(headh[i].toEdges),&(tailh[i].toEdges),&(headv[j].toEdges),&(tailv[j].toEdges),j,i);
	if(i!=j)  	
		deletexy(&(headh[j].toEdges),&(tailh[j].toEdges),&(headv[i].toEdges),&(tailv[i].toEdges),i,j);
}

void reset(struct nodetype *lists[])
{
	int i,j,k;
	struct nodetype *headh=(struct nodetype*)lists[0],*tailh=(struct nodetype*)lists[1],
									*headv=(struct nodetype*)lists[2],*tailv=(struct nodetype*)lists[3];
		for(i=0;i<N;i++)
		{
			headh[i].visited=0;
			headh[i].parent=-1;
			headh[i].distance=-1;
			for(j=0;j<N;j++)
			{
				reset_xy(headh,tailh,i,j);
				reset_xy(headh,tailh,j,i);
			}
		}
		root=-1;
}

void cycles(struct nodetype *lists[])
{
	struct nodetype *headh=(struct nodetype*)lists[0],*tailh=(struct nodetype*)lists[1],
		*headv=(struct nodetype*)lists[2],*tailv=(struct nodetype*)lists[3];
	int i,j,k;
	if(root==-1)return;
//	struct nodetype *cyclex,*cycley;cyclex=malloc(sizeof(struct nodetype));cycley=malloc(sizeof(struct nodetype));
 //   cyclex->toEdges=NULL;cyclex->visited=0;cyclex->parent=0;cyclex->distance=0;
//	cycley->toEdges=NULL;cycley->visited=0;cycley->parent=0;cycley->distance=0;
//	struct nodetype *cycleEdges[2]={cyclex,cycley};
	int *cycles_i,*cycles_j,count=0,l=0,cycNode;
    struct qnode *cyclesQueue_i=NULL,*cyclesQueue_j=NULL;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		if(find_xy(headh,tailh,j,i,'e')&&!find_xy(headh,tailh,j,i,'s'))//& not in C(circles)
		//find path to root
		//while(k!=root) add edge(i,k=parent(i))->C
		//while(k!=root) add edge(j,k=parent(i))->C
		//remove cut
		{}
	}
 
}

void setunion(void *a,void *b,void *c)
{
/*


*/	

}

int main(void)
{
char s[1],c='?';
	int i=0,j,k;
	struct nodetype headh[N],headv[N],tailh[N],tailv[N],
					*lists[4]={headh,headv,tailh,tailv};
		struct tnode *root,*nil;
	initall(headh,tailh,headv,tailv);

	s[0]=c;

	while (c!='q')
	{
			printf("Sparse array menu\n(i-insert,d-delete,e-printedges,q-quit\n \
			\ra-insertall,x-deleteall,b-bfs,s-printspanning): ");
			c=getchar();	
			switch(c)
			{
				case 'i':{printf("inserting:\n");
									//reset(lists);
									fflushnew(&c);
									i=-1;j=-1;
									scanf("%d %d",&i,&j);					
									if(i>=0&&i<N&&j>=0&&j<N)
										insert(lists,i,j);}break;
				case 'a':{printf("inserting all:\n");
									reset(lists);
									//insertall(lists);
									insert(lists,1,2);
									insert(lists,1,4);
									insert(lists,2,3);
                                    insert(lists,3,4);
                                    insert(lists,2,5);
                                    insert(lists,4,7);
                                    insert(lists,3,5);
                                    insert(lists,3,7);
                                    insert(lists,3,6);
                                    insert(lists,5,6);
                                    insert(lists,6,7);
								printf("\nsizeof(lists)=%d\n",sizeof(lists));
				/*					nil=(struct tnode*)malloc(sizeof(struct tnode));//nil is the sentinel node.
                                     nil->color=black;
                                     nil->data=9999999;              //setting a "highest" value for sentinel
                                     nil->rchild=nil->lchild=NULL;
                                     root=nil;
                                    root=insertRBNode(root,1,nil,1,2);
									inorder(root,nil);*/
                                    }break;
				case 'd':{printf("deleting:\n");
									fflushnew(&c);
									i=-1;j=-1;
									scanf("%d %d",&i,&j);					
									if(i>=0&&i<N&&j>=0&&j<N)
										delete(lists,i,j);}break;
				case 'x':{printf("deleting all:\n");
									reset(lists);
									deleteall(lists);}break;		
				case 'e':{printf("printing edges:\n");
									printall(headh,tailh,'e');}break;
				case 's':{printf("printing spanning:\n");									
									printall(headh,tailh,'s');}break;
				case 'c':{printf("cycles:\n");
									cycles(lists);}break;
				case 'b':{printf("bfs:\n");
                                   
                                    printf("\nBreadth-First search - > select source node:");
                                    int a=scanf("%d",&i);
                                    if(i>=0&&i<N)
                                        bfs(i,lists);
                                        
                                    printf("Did spanning tree.");
                                    }break;
				case 'q':{printf("quitting.\n");}break;
				default : printf("\nPlease provide correct input- you typed <%c>\n",c);
				
			}
			if(c!='q')
			//snippet instead of fflush(stdin) that actually WORKS.
				fflushnew(&c);
	}
	getchar();
	return 0;
}



void fflushnew(char *c){while((*c = getchar()) != '\n' && *c != EOF);}
