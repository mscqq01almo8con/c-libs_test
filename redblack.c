# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <time.h>
enum rbcolor {red,black};//make a "color" variable for better viewing

struct tnode {   //the typical tree node has a data field, a parent and 2 children.
 int data,anode,bnode;
 enum rbcolor color;
 struct tnode *rchild,*lchild,*parent;
};


//The rb_transplant function replaces node u with node v, rearranging the pointers correctly.
struct tnode *rb_transplant(struct tnode *T,struct tnode *u,struct tnode *v,struct tnode *nil)
{
 if ( u->parent==nil )            //U is the ROOT
  T=v;                          //replace u with v       
 else                           //u is not ROOT?
 {
  if ( u==(u)->parent->lchild )   //u is left child of its parent
   u->parent->lchild=v;         //replace u with v
  else                          //if u is right child of its parent
   u->parent->rchild=v;         //replace u with v
 }
 v->parent=u->parent;
 
 return T;
}

//inorder traversal of our tree. uses nil instead of NULL.
void inorder(struct tnode *T,struct tnode *nil)
{
 if( T != nil )
 {
  inorder( T->lchild,nil );                      //-left subtree recursion before print
  printf("\nNode: %d, 'A' node: %d, 'B' node:%d ",T->data,T->anode,T->bnode);             //-print tree (inorder)
  if( T->color==red )printf("color=red\n");      //-printf color, too
  else               printf("color=black\n");    //
  inorder( T->rchild,nil );                      //-right subtree recursion before print
 }
}

struct tnode* rotate_left(struct tnode *T,struct tnode *x,struct tnode *nil)
{
 struct tnode *y,*temp=(struct tnode*)malloc(sizeof(struct tnode));
 temp->parent=temp->lchild=temp->rchild=nil;
 y=temp;

 y=x->rchild;
 x->rchild=y->lchild;  
 if( y->lchild!=nil )
  y->lchild->parent=x;
  
 y->parent=x->parent;
 if(x->parent==nil)
  T=y;
 
 else
 {
  if     ( x==x->parent->lchild )
   x->parent->lchild=y;
  else if( x==x->parent->rchild )
   x->parent->rchild=y;
 }
 y->lchild=x;
 x->parent=y;
 
 return  T;
}


struct tnode* rotate_right(struct tnode *T,struct tnode *x,struct tnode *nil)
{
 struct tnode *y,*temp=(struct tnode*)malloc(sizeof(struct tnode));
 temp->parent=temp->lchild=temp->rchild=nil;
 y=temp;

 y=x->lchild;                                               
 x->lchild=y->rchild;                                  
 if( y->rchild!=nil )
  y->rchild->parent=x; //link x's parent to y.
  
 y->parent=(x)->parent;
 if(x->parent==nil)
  T=y;
 else 
 {
  if( x==x->parent->rchild )
   x->parent->rchild=y;
   
  if( x==x->parent->lchild )
   x->parent->lchild=y;
 }
 y->rchild=x;
 x->parent=y;

 return T;
}

// rb_insert_fixup restores the RB-properties of the tree, examining each of the 3 cases 
// and performing appropriate color changes and /or rotations, starting from the level 
// of the new node, and "climbing" up to the root of the tree.
struct tnode* rb_insert_fixup(struct tnode *T,struct tnode *z,struct tnode *nil)
{
 struct tnode *y;
 
 while (z->parent->color==red)
 {     
  if(z->parent==z->parent->parent->lchild)
  {
   y=z->parent->parent->rchild;
   if(y->color==red) //Case 1: z’s uncle y is red:
   {                 //so, since dad and uncle are both red, they have a black parent.
                     //which means we can just exchange grandfather's color with both dad and uncle and move up
                     //to the level of the, now red, grandparent.  
    z->parent->color=black;          //->paint dad black.
    y->color=black;                  //->paint uncle red.
    z->parent->parent->color=red;    //->paint grandfather red.
    z=z->parent->parent;             //->move up to the level of the grandfather.
   }
   else
   {
    if(z==z->parent->rchild)//Case 2: z’s uncle y is black and z is a right child
    {                       //since z is the right child, we are not yet in case 3. 
                            //But, we can get to case 3 by setting z as it's parent and then rotating left on z.
                                   
     z=z->parent;           //->set z as it's parent
     T=rotate_left(T,z,nil);//->rotating left on z.
    }            //Case 3: z’s uncle y is black and z is a left child.
                 //-exchanging color between z's father and grandfather and then rotating left on the grandfather,
                 // eliminates the violation.
    z->parent->color=black;                 //->z's parent is now black
    z->parent->parent->color=red;           //->z's grandfather is now red.
    T=rotate_right(T,z->parent->parent,nil);//-> left rotation on z's grandfather.
   } 
  }
  else //------------------The above same part, just symmetrical ( left <-> right )------------------
  {
   y=z->parent->parent->lchild;
   if(y->color==red)//Case 1: z’s uncle y is red
   {
    z->parent->color=black;
    y->color=black;
    z->parent->parent->color=red;
    z=z->parent->parent; 
   }
   else
   {
    if(z==z->parent->lchild)//Case 2: z’s uncle y is black and z is a left child
    {
     z=z->parent;
     T=rotate_right(T,z,nil);
    }
    z->parent->color=black;//Case 3: z’s uncle y is black and z is a right child
    z->parent->parent->color=red;
    T=rotate_left(T,z->parent->parent,nil);
   } 
  } 
            
 }
 T->color=black; //now that we're done with the fixup, we only
                 // have to paint (initially red) z as black.
 return T;
}
//insert does what classic binary trees insert also does, except that 
//in the end, it calls the insert-fixup function to restore any
//violations of the red black tree. (only rules 2 and 4 can appear)
struct tnode *insertRBNode(struct tnode *T,int key,struct tnode *nil,int anode,int bnode)
{
 struct tnode *y,*x,*z;
 y=nil;
 x = T;//-starting from the root, traverse the tree to find out 
       // where to put our new node.
 
 z = (struct tnode *) malloc(sizeof(struct tnode)); 
 z->data = key;
 z->anode= anode;
 z->bnode=bnode;
 z->lchild=nil;
 z->rchild=nil;
 z->color=red;      //now we have set up a new node for insertion.
 
 while(x!= nil)
 {  
  y=x;  
  if( z->data < x->data)
   x = x->lchild;
  else
   x = x->rchild;
 } 
 z->parent=y;
 if(y == nil)
  T=z;
 else
 {
  if (z->data < y->data)
   y->lchild=z;
  else
   y->rchild=z;
 }
 T=rb_insert_fixup(T,z,nil);   //-we've put a red node in our tree, now 
                               // we have to fix any potential violations.
 return (T);
}

//finds node with a given key as data.
struct tnode *findkey( struct tnode *y,int key,struct tnode *nil)
{
 struct tnode *x;
 x = y;
 while( x != nil)
 {
  if(x->data == key)
   return(x);
  else
  {
   if(x->data > key)
    x = x->lchild;
   else
    x = x ->rchild;
  }
 }
 
 return(nil);
}



// find sub-tree (rooted on x) minimum.it's used in delete 
// operation, when we wish to find the sucessor of a node.
struct tnode *tree_min(struct tnode *x,struct tnode *nil)
{
 while ( x->lchild!=nil )
  x=x->lchild;
 return (x);       
}

//rb_delete_fix checks for any violations of the rb rules and eliminates them 
//in the while loop, while it maintains the black height of each subtree.
//4 cases of rb violation are examined. 
struct tnode *rb_delete_fix(struct tnode *T,struct tnode *x,struct tnode *nil)
{
 struct tnode* w;//w is considered the sibling of x throught the while loop below.
 

 while ( x!=T && x->color==black )
 {
  if(x==x->parent->lchild)
  {
   w=x->parent->rchild;
   if(w->color==red)                     //Case 1: x’s sibling w is red
   {
    w->color=black;
    x->parent->color=red;
    T=rotate_left(T,x->parent,nil);
    w=x->parent->rchild;
   }
   if(w->lchild->color==black && w->rchild->color==black)
   {                                     //Case 2: x’s sibling w is black, and both of w’s children are black
    w->color=red;
    x=x->parent;
   }
   else 
   {
    if(w->rchild->color==black)      //Case 3: x’s sibling w is black, w’s left child is red, and w’s right child is black
    {
     w->lchild->color=black;
     w->color=red;
     T=rotate_right(T,w,nil);
     w=x->parent->rchild;
    }
    w->color=x->parent->color;        //Case 4: x’s sibling w is black, and w’s right child is red
    x->parent->color=black;
    w->rchild->color=black;
    T=rotate_left(T,x->parent,nil);
    x=T;
   }
  }
              
  else //------------- same as above, but symmetrical  ( right <-> left )--------------------
  {
   w=x->parent->lchild;
   if(w->color==red)                     //Case 1: x’s sibling w is red
   {
    w->color=black;
    x->parent->color=red;
    T=rotate_right(T,x->parent,nil);
    w=x->parent->lchild;
   }
   if(w->rchild->color==black && w->lchild->color==black)
   {                                    //Case 2: x’s sibling w is black, and both of w’s children are black
    w->color=red;
    x=x->parent;
   }
   else 
   {
    if(w->lchild->color==black)     //Case 3: x’s sibling w is black, w’s right child is red, and w’s left child is black
    {
     w->rchild->color=black;
     w->color=red;
     T=rotate_left(T,w,nil);
     w=x->parent->lchild;
    }
    w->color=x->parent->color;      //Case 4: x’s sibling w is black, and w’s left child is red
    x->parent->color=black;
    w->lchild->color=black;
    T=rotate_right(T,x->parent,nil);
    x=T;
   }
  }                 
 }//end of the while loop.
 x->color=black;
 
 return T;
}


struct tnode *deleteRBNode(struct tnode *T,int key,struct tnode *nil)
{
 struct tnode *x,*y,*z;enum rbcolor y_original;

 z=findkey(T,key,nil);//searching for the key we wish to delete.
 y=z;                 //we keep z in y and...
 y_original=y->color; //...also keep the initial color.
 if( z->lchild==nil )
 {
  x=z->rchild;
  T=rb_transplant(T,z,z->rchild,nil);                  
 }
 else
 {
  if( z->rchild==nil )
  {
   x=z->lchild;
   T=rb_transplant(T,z,z->lchild,nil);  
  }
  else
  {
   y=tree_min(z->rchild,nil);//looking for y, successor of z.          
   y_original=y->color;
   x=y->rchild;              //putting x in y's previous position.
   if(y->parent==z)
    x->parent=y;
   else
   {
    T=rb_transplant(T,y,y->rchild,nil);      
    y->rchild=z->rchild;
    y->rchild->parent=y;
   }
   T=rb_transplant(T,z,y,nil);      
   y->lchild=z->lchild;
   y->lchild->parent=y;
   y->color=z->color;
  }
 }
 if( y_original==black )   //-if y was black before and we removed it, it means
                           // that it may be replaced by a red node which could bring 
                           // a red black rule violation.
 T=rb_delete_fix(T,x,nil); //so we call rb_delete_fix to fix any potential violation
                           //that has been caused by the deletion.
 return T;
}
/*
int main(void)
{
 int i,j,k=1;
 char ch='$';
 struct tnode *root = NULL,*nil,*x,*y;

 nil=(struct tnode*)malloc(sizeof(struct tnode));//nil is the sentinel node.
 nil->color=black;
 nil->data=99999;              //setting a "highest" value for sentinel
 nil->rchild=nil->lchild=NULL;
 root=nil;                     //root initially point to sentinel.
 printf("\nInsert, Delete, Print (inorder), Group insert, Quit: ");
 while(ch!='q'&&ch!='Q')//a menu. 
 {
  
  scanf("%c",&ch);
  if ( ch=='i'||ch=='I' )//insert.
  {
   printf("\nInsert node number:");
   scanf("%d",&i);
   if(findkey(root,i,nil)==nil)//choosing not to insert copies of same number.
   root=insertRBNode(root,i,nil);
   else
   printf("\n%d already exists!\n",i);
  }
  if( ch=='d'||ch=='D' )//delete.
  {
   printf("\nDelete node number:");
   scanf("%d",&i);
   if(root!=NULL&&root!=nil&&findkey(root,i,nil)!=nil)
   root=deleteRBNode(root,i,nil);
   else printf("\nNothing to delete!\n");
  }
  if( ch=='p'||ch=='P' )//printf inorder
  {
   if(root!=NULL&&root!=nil)
    inorder(root,nil);
   else 
    printf("\nEmpty tree.");
  }
  if( ch=='g'||ch=='G' )//group insert + delete.
  {
   printf("\n [ Inserting 20 numbers: ]\n");
   for(i=1;i<=20;i++)
   {
    printf("------------------");
    root=insertRBNode(root,i,nil);
    inorder(root,nil);
   } 
   printf("\n [ Deleting the 20 numbers: ]\n");
  
   for(i=20;i>=1;i--)
   {
    if(root!=NULL&&root!=nil)
    {
     printf("------------------");
     root=deleteRBNode(root,i,nil);
     if (root!=NULL && root!= nil)
      inorder(root,nil);
     else 
      printf("\nEmpty tree.\n");
    }
   }
  
   
  }
 printf("\nInsert, Delete, Print (inorder), Group insert, Quit: ");
 }//end of menu.
 
 system("pause"); // (for windows)
 return 0;
}*/
