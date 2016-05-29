#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <ctype.h>
char buffer[256];
void fflushnew(char *c){while((*c = getchar()) != '\n' && *c != EOF);}
struct tm *get_date(char string[])//takes a string and creates a date in tm format (time.h).
{
 char days[3],months[3],years[5],final[256]; 
 struct tm *date;
 int i=0;
 
 printf("\n[System: Get date from string - start.]\n");   
 date=malloc(sizeof(struct tm));//-------------------------- first, we make a memory location to put the date in.
 
//---------------------------------------------------------- Now according to space positions in stringm we 
//---------------------------------------------------------- get the date, month & year pieces of the string

 while(string[i]!=' ')                               //reading the letters of days
 {
  days[i]=string[i];
  i++;
 }
 i++;
 while(string[i]!=' ')                               //reading the letters of months
 {
  months[i-3]=string[i];
  i++;
 }
 i++;
 while(string[i]!='\0')                              //reading the letters of years
 { 
  years[i-6]=string[i];
  i++;
 }
  days[2]='\0';  months[2]='\0';   years[4]='\0';    //ending the strings
  
  //now we make the new date
  date->tm_year=atoi(years)-1900; 
  date->tm_mon=atoi(months)-1; //1os minas: 0 , teleytaios:11
  date->tm_mday=atoi(days); //1i mera: 1, teleytaia:0
  date->tm_hour=0;
  date->tm_min=0;
  date->tm_sec=1;
  date->tm_isdst=-1;
 
  if(mktime(date)==-1)
   printf("Error getting time.\n");//we end program if we couldn't find a proper date.
   
  printf("\n[System: Get date from string - end.]\n");   
   
 return date;  
      
}

struct tm *other_day(struct tm *tday,int x)
{
  struct tm *xday;
  
  xday=malloc(sizeof(struct tm));
  
  printf("\n[System: Calculate another date - start.]\n");    
  
  if(x<0)
   printf("Successfully found %d days before \n",x);
  else
   printf("Successfully found %d days after \n",x);
   
  strftime (buffer,15, "%a %d %m %Y", tday);
  puts(buffer);
  printf(" which is: ");
  /* Convert it to local time representation. */
   
  xday->tm_year=tday->tm_year;
  xday->tm_mon=tday->tm_mon;
  xday->tm_mday=(tday->tm_mday)+x;
  xday->tm_sec=1;
  xday->tm_min=0;
  xday->tm_hour=0;
  xday->tm_isdst=-1;

  if(mktime(xday)==-1)
  {
   printf("Error getting time.\n");
   exit(0);
  }
    
  strftime (buffer,15, "%a %d %m %Y", xday); 
  puts(buffer);
  
  printf("\n[System: Calculate another date - end.]\n");   
  
  return(xday);
}

static FILE *open_file ( char *file, char *mode )
{
  FILE *fp = fopen ( file, mode );

  if ( fp == NULL ) {
    perror ( "Unable to open file" );
    exit ( -1 );
  }

  return fp;
}

int add_entry(char *entries,char *extra ,char *str_cost,int ask )
{
 struct tm *tday,*dday,*loctime,*temp;
 
 FILE *fp_ent=open_file(entries,"a"),*fp_extra=open_file(extra,"a"),*fp_cost=open_file(str_cost,"r");
 int i,num,count1,count2,count3;
 double temp_cost=0.0,cost=0.0;
 char line[21]="00000000000000000000",cdate[15],ddate[15],type,dim[20],w[7],har,status,del,hol,buy,rec,mor,sat,order='N';
 
 printf("\n[System: Add entries/calculate costs - start.]\n");     
 time_t curtime;
 curtime = time (NULL);
 loctime = localtime (&curtime);
 
  printf("Today's date is:"); 
 fputs (asctime (loctime), stdout);
               
 tday=other_day(loctime,1);//find courier's date.
                 
 strftime (cdate,15, "%a %d %m %Y", tday);
 

 if((cdate[0]=='S'||cdate[0]=='s')&&(cdate[1]=='U'||cdate[1]=='u')&&(cdate[2]=='N'||cdate[2]=='n'))
 {
  tday=other_day(tday,1);
  strftime (cdate,15, "%a %d %m %Y", tday );
  printf ("Next workday after Sunday:");puts(cdate);
 }
 
 dday=other_day(tday,1);//find next workday delivery date.
 strftime (ddate,15, "%a %d %m %Y",dday );  
 
 if((ddate[0]=='S'||ddate[0]=='s')&&(ddate[1]=='U'||ddate[1]=='u')&&(ddate[2]=='N'||ddate[2]=='n'))
 {
  dday=other_day(tday,2);
  strftime (ddate,15, "%a %d %m %Y", dday );
 }
 
 if(cdate[14]=='\0')//
 printf ("Courier date:");    puts(cdate);
 printf ("Delivery date before saturday case examine:");               
 puts(ddate);
                 
 printf("\nInsert delivery code number:");
 scanf("%d%*c",&num); 
 
 printf("\nSetting courier date (format:Www dd mm yyyy):%s",cdate);
 printf("\nSetting delivery date (format:Www dd mm yyyy):%s",ddate);
   
 printf("\nInsert packet type ('F'older,'B'ox):");
 scanf("%c%*c",&type);
 while(type!='F'&&type!='f'&&type!='B'&&type!='b')
 {
  printf("\nIncorrect input. \nInsert packet type ('F'older,'B'ox):");
  scanf("%c%*c",&type);
 }
 printf("\nInsert dimensions (format:xxx.x,yyy.y,zzz.z) (se cm):");//------
 gets(dim);
 i=0;count1=0;count2=0;count3=0;
 while(dim[i]!='\0')
 {
   if(dim[i]=='.')
    count1++;
   if (dim[i]==',')
    count2++;
   if(isdigit(dim[i]))
    count3++;
   i++;
 }
 while(count2!=2||count1!=3||count3!=12)
 {
  printf("\nIncorrect input.\nInsert dimensions (format:xxx.x,yyy.y,zzz.z) (cm):");
  gets(dim);  
   i=0;count1=0;count2=0;count3=0; 
   while(dim[i]!='\0')
   {
   if(dim[i]=='.')
    count1++;
   if (dim[i]==',')
    count2++;
   if(isdigit(dim[i]))
    count3++;
     i++;  
   }       
                             
 }
 
 printf("\nInsert weight (format:www.ww) (kg):");//------
 gets(w);
 
 i=0;count1=0;count2=0;count3=0;
 while(w[i]!='\0')
 {
   if(w[i]=='.')
    count1++;
   
   if(isdigit(w[i]))
    count2++;
    
   i++;  
 }
 while(count1!=1||count2!=5)
 {
  printf("\nIncorrect input.\nInsert weight (format:www.ww) (kg):\n");
  gets(w);
 
  i=0;count1=0;count2=0;count3=0;
  while(w[i]!='\0')
  {
    if(w[i]=='.')
     count1++;
   
    if(isdigit(w[i]))
     count2++;
    
    i++;  
  }                                 
 }
 
 printf("\nInsert Hard-reached area delivery cost (format:'Y'es,'N'o):");
 scanf("%c%*c",&har);
 while(har!='Y'&&har!='y'&&har!='N'&&har!='n')
 {
  printf("\nIncorrect input.\nInsert Hard-reached area delivery cost (format:'Y'es,'N'o):");
  scanf("%c%*c",&har);
 }
 //-----------------
 printf("\nStatus Complete (format:'Y'es,'N'o):");
 scanf("%c%*c",&status); 
 while(status!='Y'&&status!='y'&&status!='N'&&status!='n')
 {
  printf("\nIncorrect input.\nStatus Complete (format:'Y'es,'N'o):");
  scanf("%c%*c",&status);
 }
 
 printf("\nBuy (format:'Y'es,'N'o):");
   scanf("%c%*c",&buy); 
 while(buy!='Y'&&buy!='y'&&buy!='N'&&buy!='n')
 {
  printf("\nIncorrect input.\nBuy (format:'Y'es,'N'o):");
   scanf("%c%*c",&buy);
 }
 
 printf("\nSend receipt back to sender (format:'Y'es,'N'o):");
   scanf("%c%*c",&rec); 
 while(rec!='Y'&&rec!='y'&&rec!='N'&&rec!='n')
 {
  printf("\nIncorrect input.\nSend receipt back to sender (format:'Y'es,'N'o):");
   scanf("%c%*c",&rec);
 }
 
 printf("\nDelivery (format:'Y'es,'N'o):");
   scanf("%c%*c",&del); 
 while(del!='Y'&&del!='y'&&del!='N'&&del!='n')
 {
  printf("\nIncorrect input.\nDelivery (format:'Y'es,'N'o):");
   scanf("%c%*c",&del);
 }
 
 printf("\nHoliday delivery (format:'Y'es,'N'o):");
 scanf("%c%*c",&hol);
 while(hol!='Y'&&hol!='y'&&hol!='N'&&hol=='n')
 {
  printf("\nIncorrect input.\nHoliday delivery (format:'Y'es,'N'o):");
   scanf("%c%*c",&hol);
 }
  
 printf("\nDelivery in the morning (format:'Y'es,'N'o):");
   scanf("%c%*c",&mor); 
 while(mor!='Y'&&mor!='y'&&mor!='N'&&mor!='n')
 {
  printf("\nIncorrect input.\nDelivery in the morning (format:'Y'es,'N'o):");
  scanf("%c%*c",&mor);
 }
 
 if((har=='Y'||har=='y')&&(mor=='Y'||mor=='y'))
 {
  printf("\n------------------------------------------\nCan't deliver package in hard reached areas in the morning. Defaulting in midday/afternoon delivery.\n------------------------------------------\n");
  mor=='N';
 }
 
 printf("\nSaturday delivery (format:'Y'es,'N'o):");
  scanf("%c%*c",&sat);
  while(sat!='Y'&&sat!='y'&&sat!='N'&&sat=='n')
 {
  printf("\nIncorrect input.\nSaturday delivery (format:'Y'es,'N'o):");
   scanf("%c%*c",&sat);
 }  
   
 if((ddate[0]=='S'||ddate[0]=='s')&&(ddate[1]=='A'||ddate[1]=='a')&&(ddate[2]=='T'||ddate[2]=='t'))
 {
  if((sat=='Y'||sat=='y')&&(har=='Y'||har=='y'))
  {
   ddate[14]='\0';
   strftime (buffer,256, "%a %d %m %Y.\n", other_day(get_date(&ddate[4]),2) );   
   sat=='N';
   printf("\nCan't deliver in  hard reached areas on Saturday. Must Be a workday. Setting delivery date on ");
   puts(buffer);
   for(i=0;i<15;i++)
    ddate[i]=buffer[i];
   ddate[14]='\0';
  }
 
 }
 else sat='N';
 
 printf("______________________________________");
 
 fgets( line, sizeof line, fp_cost );
 i=0;
 while (line[i]!='\n')
 {
  buffer[i]=line[i+9];
  i++;
 }
 temp_cost=atof(buffer);
 cost+=temp_cost;
 printf("\nInitial cost:                                       %9.2lf",cost);
 

 if(del=='Y'||del=='y')//KOSTOS ANTIKATAVOLIS
 {
  fgets( line, sizeof line, fp_cost );
  i=0;
  while (line[i]!='\n')
  {
   buffer[i]=line[i+9];
   i++;
  }
  temp_cost=atof(buffer);
  cost+=temp_cost;
  printf("\n+Delivery cost                                      %9.2lf",cost);
 }
 
 if(hol=='Y'||hol=='y')//KOSTOS Paradosis se argies
 {
  fgets( line, sizeof line, fp_cost );
  i=0;
  while (line[i]!='\n')
  {
   buffer[i]=line[i+9];
   i++;
  }
  temp_cost=atof(buffer);
  cost+=temp_cost;
  printf("\n+Deliver during holidays:                           %9.2f",cost);
 }
 
 if(buy=='Y'||buy=='y')//KOSTOS Agoras
 {
  fgets( line, sizeof line, fp_cost );
  i=0;
  while (line[i]!='\n')
  {
   buffer[i]=line[i+9];
   i++;
  }
  temp_cost=atof(buffer);
  cost+=temp_cost;
  printf("\n+Buy:                                               %9.2f",cost);
 }
 
 if(rec=='Y'||rec=='y')//KOSTOS epistrofis apodeiktikou ston apostolea
 {
  fgets( line, sizeof line, fp_cost );
  i=0;
  while (line[i]!='\n')
  {
   buffer[i]=line[i+9];
   i++;
  }
  temp_cost=atof(buffer);
  cost+=temp_cost;
  printf("\n+Send receipt back to sender:                       %9.2f",cost);
 }
 
 if(mor=='Y'||mor=='y')//KOSTOS prwinis paradosis
 {
  fgets( line, sizeof line, fp_cost );
  i=0;
  while (line[i]!='\n')
  {
   buffer[i]=line[i+9];
   i++;
  }
  temp_cost=atof(buffer);
  cost+=temp_cost;
  printf("\n+Morning delivery cost :                            %9.2f",cost);
 }
 
 if(sat=='Y'||sat=='y')//KOSTOS savvato
 {
  fgets( line, sizeof line, fp_cost );
  i=0;
  while (line[i]!='\n')
  {
   buffer[i]=line[i+9];
   i++;
  }
  temp_cost=atof(buffer);
  cost+=temp_cost;
  printf("\n+Saturday delivery (not for hard reached areas):    %9.2f",cost);
 }
 
 if(har=='Y'||har=='y')//KOSTOS dysprositi per
 {
  fgets( line, sizeof line, fp_cost );
  i=0;
  while (line[i]!='\n')
  {
   buffer[i]=line[i+9];
   i++;
  }
  temp_cost=atof(buffer);
  cost+=temp_cost;
  printf("\n+Hard reached area cost:                            %9.2f",cost);
 }
 
 printf("\n                                      ->Total Cost: %9.2f\n",cost);
 
 if (ask==0)
 {
  fprintf(fp_ent,"%8d %14s %14s %1c %17s %6s %1c %9.2f %1c\n",num,cdate,ddate,type,dim,w,har,cost,status);
  fprintf(fp_extra,"%8d %1c %1c %1c %1c %1c %1c %1c\n",num,del,hol,buy,rec,mor,sat,har);
  
 }
 else
 {
  printf("\nThe cost is: %lf.  Place the order?(Y,N):",cost);
  scanf("%c",&order);
  if(order=='Y'||order=='y')
  {
    fprintf(fp_ent,"%8d %14s %14s %1c %17s %6s %1c %6.2lf %1c\n",num,cdate,ddate,type,dim,w,har,cost,status);
    fprintf(fp_extra,"%8d %1c %1c %1c %1c %1c %1c %1c\n",num,del,hol,buy,rec,mor,sat,har);
    printf("\nOrder registered.");
  }
  else
   printf("\nOrder not registered.");
 }
  
 fclose(fp_cost);
 fclose(fp_ent);
 fclose(fp_extra);
 //fflushnew(&order);
     printf("\n[System: Add entries/calculate costs - end.]\n");     
}

int edit_cost(char *cost ,int edit )
{
 FILE *fp_cost;
 double simp,del,hol,buy,rec,mor,sat,har; 
 char line[40];
 int i=0;
 
 if(edit==1)
 {
  fp_cost=open_file(cost,"w");
  
  printf("\nInsert simple sent cost: ");
  scanf("%lf",&simp);
  printf("\nInsert delivery cost: ");
  scanf("%lf",&del);
  printf("\nInsert holidays cost: ");
  scanf("%lf",&hol);
  printf("\nInsert buy cost: ");
  scanf("%lf",&buy);
  printf("\nInsert receipt cost: ");
  scanf("%lf",&rec);
  printf("\nInsert morning delivery cost: ");
  scanf("%lf",&mor);
  printf("\nInsert Saturday delivery cost: ");
  scanf("%lf",&sat);
  printf("\nInsert Hard-reached area delivery cost: ");
  scanf("%lf",&har);
  fprintf(fp_cost,"Simp snt:%9.2lf\nDelv cst:%9.2lf\nHol cost:%9.2lf\nBuy cost:%9.2lf\nRecp cst:%9.2lf\nMor cost:%9.2lf\nSat cost:%9.2lf\nHard cst:%9.2lf\n",simp,del,hol,buy,rec,mor,sat,har);
  //fflushnew(&har);
 }                 
 else
 {
  fp_cost=open_file(cost,"r");   
  while(fgets( line, sizeof line, fp_cost )!= NULL)
    puts(line); 

 }
 fclose(fp_cost);
}


void incomplete(char *entries)
{
 char line[100];
 int i=0;
 FILE *fp_entries=fopen(entries,"r");
 
 printf("\n[System: Print Incomplete entries - start.]\n");  
 printf("--------------------------------------------------------------------------------");
 printf("\n                           ~~  Incomplete orders:  ~~\n");    
 printf("--------------------------------------------------------------------------------\n");
 while(fgets(line,100,fp_entries)!=NULL)//||!feof(print_line))
 {
  if(line[78]!='Y'&&line[78]!='y')
  {
   puts(line);
  }
  i++;
 }
 fclose(fp_entries);   
 
 printf("--------------------------------------------------------------------------------\n");
 printf("\n[System: Print Incomplete entries - end.]\n");       
}


void search_entry(char *entries)
{
 int i,k,found=0;
 char test[20],line[200],num[20],zeros[20]="000000000000";
 FILE *fp_entries,*fp_last=NULL;
 
 printf("\n[System: Print Entry Status - start.]\n");
  
 if((fp_entries=fopen(entries,"r"))==NULL)
  printf("\nEntries file doesn't exist!\n");
 else
 {
  printf("\nEnter order number:");

  gets (num);//AM
  printf("Searching for entry No. %s...\n",num);
  
  i=0;
  while(i<8-strlen(num))
  {
   zeros[i]=' ';//00000
   i++;
  }
  zeros[i]='\0';
  strcat(zeros,num);
  strcpy(num,zeros);
  
  while(fgets(line,200, fp_entries)!=NULL)
  {
   k=0;
   while (k<8)
   {
    test[k]=line[k];
    k++;
   }
   test[k]='\0';
   k++;
   
   if(strcmp(test,num)==0)
   {
    found=1; 
    break;
   }
   else fp_last=fp_entries;
  }
  if (found==1)
  
  {
   printf("--------------------------------------------------------------------------------");
   printf("\n-Entry no. %s found! Displaying entry elements:\n",num);
   printf("--------------------------------------------------------------------------------\n");
   puts(line); 
   printf("--------------------------------------------------------------------------------\n");
   if(fp_last)fgets(line,200, fp_last);//read a buffered line
  }
  else 
  {
   printf("--------------------------------------------------------------------------------");
   printf("\n-Couldn't find entry no. %s\n",num);
   printf("--------------------------------------------------------------------------------\n");
  }
 
 fclose (fp_entries);
 }     
 
 printf("\n[System: Print Entry Status - end.]\n");
}

void profits(char *entries)
{
 struct tm *yday,*loctime,*tday;
 char buffer2[20],line[200],cost_sum[10],code[9];
 double sum=0.0;
 int num=0,i;
 FILE * fp_entries=fopen(entries,"r");
 
 printf("\n[System: Print all y-day profits - start]\n");
 time_t curtime;
 curtime = time (NULL);
 loctime = localtime (&curtime);
 printf("Current time is:");//Today is
 fputs (asctime (loctime), stdout);
 yday=other_day(loctime,-1);
                
 strftime (buffer,256, "%d %m %Y\n", yday);  
                
 printf("Yday was:");//Today is
 buffer[10]='\0';
 puts(buffer);  
                         
 printf("\n|________________|__________|\n");
 printf("\n| Matching Entry |   Code   |\n");
 printf("\n|================|==========|\n");
 
 while(!feof(fp_entries)&& fgets(line,29,fp_entries)!=NULL)//||!feof(print_line))
 {
  for(i=0;i<8;i++)
   code[i]=line[i];code[8]='\0';
   
  fgets(buffer2,10+1,fp_entries);
  fgets(line,30+1,fp_entries);
  fgets(cost_sum,10+1,fp_entries);
  fgets(line,3,fp_entries);
  
  if(!strcmp(buffer,buffer2)&&(line[0]=='y'||line[0]=='Y') )
  {
   num++;
   printf("|%16d| %8s |\n",num,code);
   sum+=atof(cost_sum);
  }
 }
 
 printf("Total cost:%f",sum);

 fclose(fp_entries);  
  
 printf("\n[System: Print all y-day profits - end.]\n");
 
}


int main ( void )
{
  FILE *fcost,*fp_extra,*fp_entries;
  int ask,cost_exists;
  char i='0',ar_ext[8], ch='&',entries[13]="Entries.txt",cost[9]="Cost.txt",extra[10]="Extra.txt";
  
  
  fcost=fopen(cost,"r");
  if(fcost==NULL)
  {
   printf("Couldn't find %s\nThe file will now be created. Please insert the different cost values, before advancing to menu.\n",cost);
   edit_cost(cost,1);
  }
  else 
  {
   printf("Cost file detected. Advancing to menu.\n",cost);
   fclose(fcost);
  }
    
  
  
  
  
  while(ch!='8')
  {
  printf("\n1. New entry\n2. Calculate cost\n3. Print entry status\n4. Print non delivered entries\n5. Profits\n6. Services cost\n7. Update services cost\n8. Quit\n Your input:");
   ch=getchar();
   printf("\n**  Your input was: %c  ** \n",ch);

   switch (ch)
   {
    case '1': {
               ask=0;
               add_entry(entries,extra,cost,ask);
               
              }break;//don't ask.
              
    case '2': {
               ask=1;
               add_entry(entries,extra,cost,ask);
               //fflushnew(&ch);
              }break;//ask.
              
    case '3': 
               {fflushnew(&ch);search_entry(entries);
               }
               break;
              
    case '4':  {incomplete(entries);
    					 }
               break;
                  
    case '5':  {profits(entries);}
               break;
    
    case '6':  {edit_cost(cost,0);}
               break;//view cost file
    
    case '7':  {edit_cost(cost,1);fflushnew(&ch);}
               break;//edit cost file
    
    case '8':  break;//quit
    
    default: printf("Please enter correct input (1-8)");
   }
   
   if(ch!='8')fflushnew(&ch);
  }

  system ("pause");
  return 0;
}
