# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <time.h>


typedef struct mystruct {   //the typical tree node has a data field, a parent and 2 children.
int a,b;
char c;

}stct;
int main(void){
    stct mystruct;
    char mychar[17]="zavarakatranemia\n", *temp;
    char * stringArray[4]={"a","bb","ccc","dddd"};
    char ** ptrToStrArray[3]={stringArray,stringArray+1,stringArray+2};
    char *** ptrToPtrToStrArray=ptrToStrArray;
    
    printf("\nsize of struct is: %d (expecting 12=4+4+1+(filling 3 more bytes due to memory alignment.)\n",sizeof(mystruct));
    printf("\nsize of struct is: %d (expecting 4 (it's an address))\n",sizeof(&mystruct));    
    printf("\nsize of struct is: %d (expecting 4 (it's an address))\n",sizeof(&mychar));        
    printf("\nsize of struct is: %d (expecting 17)\n",sizeof(mychar));        
    
    printf("\n mychar+1=%d, &mychar+1=%d \n",mychar+1 - (int)(&mychar),mychar+1 - (int)(&mychar));        
    printf("\n sizeof stringArray=%d,\n",sizeof(stringArray));
    puts(*ptrToStrArray[1]);
    puts(**ptrToPtrToStrArray);
    
    printf("\nsizeof \"\" = %d,\n",sizeof(""));
    system("pause");
    
    return 0;
}
