#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct node             // This is 1 way of making the cell struct.
{
 int data;
 struct node *next;
};

typedef struct nodetype *ptype; // This is another way.
struct nodetype
{
 int data2;
 ptype nextp;     
};
//typedef struct nodetype *ptype; /*  ->>   It is an error to put this after the struct. */

struct node *makelist(int x,int y)
{
 struct node *cell,*mylist;
	 cell=malloc(sizeof(struct node));
	 cell->data=x;
	 cell->next=NULL;
	 mylist=cell;
	 cell=malloc(sizeof(struct node));
	 cell->data=y;
	 cell->next=mylist;
	 mylist=cell;
 return mylist;
}

void printlist(struct node *p)//only works for this example
{
 if(p!=NULL)
 	printf("\n %d , %d \n",p->data,p->next->data);
 else printf("\n...empty list!\n");
}

void no_pointer_change(struct node *q)
{
 //CHANGES the VALUE in the address the pointer points to,
 //	(changes the list data contents) but NOT the ADDRESS 
 //	the pointer points to (address of the list).
 struct node *newpointer;//=malloc(sizeof(struct node));
	 newpointer=q;
	 newpointer->data++;
}

struct node *delete(struct node *p)
{
if(p)
	if(p->next)
   { free(p->next);//it only works if there
		free(p);}// are p->next and p!
    return NULL;
}

struct node *pointer_change_p(struct node *q)
{
	 q=delete(q);
	 q=makelist(3,4);
     return q;
}

struct node *pointer_change_double_p(struct node **q)
{
 *q=NULL;	 //with RETURN
 return *q;
}

void pointer_change_double_p2(struct node **q)
{
	 *q=makelist(6,7);	 //without RETURN
}

void pointers_to_infinity4(struct node *****p4)
{    ****p4=NULL;                 }
void pointers_to_infinity3(struct node ****p3)
{    pointers_to_infinity4(&p3);  }
void pointers_to_infinity2(struct node ***p2)
{    pointers_to_infinity3(&p2);  }
void pointers_to_infinity1(struct node **p1)
{    pointers_to_infinity2(&p1);  }



int main(void)
{
 struct node *list;
 char opt='l';
 int m[2][2]={{1,2},{3,4}};
 
 printf("0: Make list [1],[2].\n");
 list=makelist(1,2);//make list
 printlist(list);


     //changing list to NULL
/*1a*/printf("1a: Change list but not the list pointer.\n");
     no_pointer_change(list);//a pointer to list changes the list,
     printlist(list);	 // but not the pointer itself.
     //in other words, we point at the address the 'list' pointer contains
     
/*1b*/printf("1b: Change list : [3],[4].\n");
     list=pointer_change_p(list);//changing list to 3,4
     printlist(list);
     
     
/*1c*/printf("1c: Change list : [3],[4]->NULL.\n");
     list=pointer_change_double_p(&list);//changing list to NULL
                       	//a pointer to the address of list 
     printlist(list);  //pointer itself will be created
     //in other words, we point at the address the of the 'list' pointer itself
    
/*1d*/printf("1d: Change list : NULL->[6],[7].\n");
     pointer_change_double_p2(&list);//changing list to 6,7
     printlist(list);
    
/*1e*/printf("1e: Change list : NULL.\n");
     pointers_to_infinity1(&list);//a pointer to a pointer to a pointer to a pointer to list. what?
     printlist(list);
     
/*2b*/printf("2b: Pointer++ experiments:\n");
     int *ptr,i=15;
     ptr=&i;
     printf("Initial: [ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     ptr++; // Pointer moves to the next int position (as if it was an array)
     printf("ptr++:   [ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     ptr=&i;++ptr; // Pointer moves to the next int position (as if it was an array)
     printf("++ptr:   [ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     ptr=&i;++*ptr; // The value of ptr is incremented
     printf("++*ptr:  [ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     ptr=&i;++(*ptr); // The value of ptr is incremented
     printf("++(*ptr):[ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     ptr=&i;*ptr++; // Pointer moves to the next int position (as if it was an array). But returns the old content
     printf("*ptr++:  [ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     ptr=&i;(*ptr)++; // The value of ptr is incremented
     printf("(*ptr)++:[ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     ptr=&i;*(ptr)++; // Pointer moves to the next int position (as if it was an array). But returns the old content
     printf("*(ptr)++:[ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     ptr=&i;*++ptr; // Pointer moves to the next int position, and then get's accessed, with your code, segfault
     printf("*++ptr  :[ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     ptr=&i;*(++ptr); // Pointer moves to the next int position, and then get's accessed, with your code, segfault
     printf("*(++ptr):[ptr=%d],[*ptr=%d]\n",ptr,*ptr);
     
     printf("Summary: ptr++ = ++ptr = *(ptr)++ = *ptr++ --> \n\t\t --> p & *p give both the address\n");
     printf("         *ptr=++(*ptr)=(*ptr)++ --> \n\t\t --> p & *p give address and increment the value respectively\n");
     printf("         *++ptr=*(++ptr)--> \n\t\t --> p & *p give both the address, SEGFAULT possible\n");
     int ii=0;
     char *p=(char*)ii;p=(char*)malloc(sizeof(char));*p='o';
     
 //system("pause");
 return 0;
}
