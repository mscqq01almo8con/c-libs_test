/*QUICK SORT και QUICK SORT 2*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int exchange(int *p,int *q)//Exchange elements(ανταλλαγή στοιχείων)
{
  int temp;
  
        temp=*p;
        *p=*q;
        *q=temp;
 return 0;
}

int exchange2(double *p2,double *q2)//Exchange elements(ανταλλαγή στοιχείων)
{
  int temp2;
  
        temp2=*p2;
        *p2=*q2;
        *q2=temp2;
 return 0;
}

int quicksort(double a[],int start,int end)
{
 int l,r;
 double pivot;
 if(start<end)/*if the array contains <2 elements then stop(αν έχουμε <2 στοιχεία, σταματάμε)*/
 { 
  l=start;
  r=end;
  pivot=a[start];/*choose 1st element for pivot(επιλογή 1ου στοιχείου για οδηγό)*/
  /*repeat until l>=r (επανάληψη μέχρι l>=r)*/
  while(l<r)
  {
   /*move index r*/
   while(a[r]>pivot)
   r=r-1;
   while(a[l]<=pivot && l<r)
   l=l+1;
   if(l<r)
   exchange2(&a[l],&a[r]);
  }
  /*exchange needed, because l=r (l=r->ανταλλαγή στοιχείων)*/
  exchange2(&a[start],&a[r]);
  quicksort(a,start,r-1);//recursion for left piece(αναδρομή σγια το αριστερό κομμάτι)
  quicksort(a,r+1,end);//recursion for right piece(αναδρομή σγια το δεξιό κομμάτι)
 }
 return 0;
}

int quicksort2(double a[],int a2[],int start,int end)
{
 int l,r;
 double pivot;
 if(start<end)/*if the array contains <2 elements then stop(αν έχουμε <2 στοιχεία, σταματάμε)*/
 { 
  l=start;
  r=end;
  pivot=a[start];/*choose 1st element for pivot(επιλογή 1ου στοιχείου για οδηγό)*/
  /*repeat until l>=r (επανάληψη μέχρι l>r=)*/
  while(l<r)
  {
   /*move index r*/
   while(a[r]>pivot)
   r=r-1;
   while(a[l]<=pivot && l<r)
   l=l+1;
   if(l<r)
   { 
    exchange2(&a[l],&a[r]);
    exchange(&a2[l],&a2[r]);
   }
  }
  /*exchange needed, because l=r (l=r->ανταλλαγή στοιχείων)*/
  { 
   exchange2(&a[start],&a[r]);
   exchange(&a2[start],&a2[r]);
  }
  quicksort2(a,a2,start,r-1);//recursion for left piece(αναδρομή σγια το αριστερό κομμάτι)
  quicksort2(a,a2,r+1,end);//recursion for right piece(αναδρομή σγια το δεξιό κομμάτι)
 }
 return 0;
}
