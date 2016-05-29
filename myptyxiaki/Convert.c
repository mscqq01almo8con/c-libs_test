#include<stdio.h>
#include<stdlib.h>

typedef struct 
{
  unsigned char red,blue,green;
} PPMPixel;
typedef struct 
{
  int x, y;
  PPMPixel *data;
} PPMImage;

//special thanks to RpfElgueiras
#define RGB_COMPONENT_COLOR 255

static PPMImage *readPPM(const char *filename)
{
 char buff[16];
 PPMImage *img;
 FILE *fp;
 int c, rgb_comp_color;
 fp = fopen(filename, "rb");//open PPM file for reading
 if (!fp) 
 {
  fprintf(stderr, "Unable to open file '%s'\n", filename);
  exit(1);
 } 
 if (!fgets(buff, sizeof(buff), fp)) //read image format
 {
  perror(filename);
  exit(1);
 }

 if (buff[0] != 'P' || buff[1] != '6')   //check the image format
 {
  fprintf(stderr, "Invalid image format (must be 'P6')\n");
  exit(1);
 }

 img = (PPMImage *)malloc(sizeof(PPMImage)); //alloc memory form image
 if (!img) 
 {
 fprintf(stderr, "Unable to allocate memory\n");
 exit(1);
 }

 c = getc(fp);//check for comments
 while (c == '#') 
 {
  while (getc(fp) != '\n') ;
  c = getc(fp);
 }

 ungetc(c, fp);
 if (fscanf(fp, "%d %d", &img->x, &img->y) != 2)  //read image size information
 {
  fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
  exit(1);
 }

   
 if (fscanf(fp, "%d", &rgb_comp_color) != 1)  //read rgb component
 {
  fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
  exit(1);
 }
  
 if (rgb_comp_color!= RGB_COMPONENT_COLOR) //check rgb component depth 
 {
  fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
  exit(1);
 }

 while (fgetc(fp) != '\n') ;
 img->data = (PPMPixel*)malloc(3 * img->x * img->y);  //memory allocation for pixel data

 if (!img) 
 {
  fprintf(stderr, "Unable to allocate memory\n");
  exit(1);
 }

 if (fread(img->data, 3 * img->x, img->y, fp) != img->y)//read pixel data from file
 {
 fprintf(stderr, "Error loading image '%s'\n", filename);
 exit(1);
 }

 fclose(fp);
 return img;
}

void writePPM(const char *filename, PPMImage *img)
{
 unsigned char data[img->y*img->x][3];
 int X,Y;
 int p25=0,p50=0,p75=0,i,j,coord_print=1;
 double coord_factor;
 FILE *fp,*fp1;

 /*                   */
     coord_print=1;   
     coord_factor=1.0; 
 /*                   */

 fp = fopen("imgdata.dat", "wb"); //open file for output
 if (!fp)
 {
  fprintf(stderr, "Unable to open file '%s'\n", filename);
  exit(1);
 }
 fwrite(img->data, 3 * img->x, img->y, fp);// pixel data
 fclose(fp);
 printf("Press any key...");
 getchar();

 
 fp=fopen("imgdata.dat","r");
 if(fp==NULL);
 for(i=0;i<img->y*img->x;i++)
 {
  if(i==(int)img->y*(img->x-1)/4&&p25==0)
  {printf("\n 25 percent Complete");p25=1;}
  if(i==(int)img->y*(img->x-1)/2&&p50==0)
  { printf("\n 50 percent Complete");p50=1;}
  if(i==(int)3*img->y*(img->x-1)/4&&p75==0)
  { printf("\n 75 percent Complete");p75=1;}
  if(i==(int)img->y*img->x-1)
  { printf("\n100 percent Complete\n");}
  for(j=0;j<3;j++)
  {
   fread(&data[i][j],sizeof(data),1,fp);
  }
 }
 printf("\n");
 fclose(fp);

 if(coord_print!=0)printf("Space Coordinates will be printed on file, next to color coordinates");

 fp1=fopen("final_data.dat","w");

 X=0;Y=0; 

 for(i=0;i<img->y*img->x;i++)
  for(j=0;j<3;j++)
  {//---------------------------------------
  
   fprintf(fp1,"%lf ",(double)data[i][j]);
   if(j==3-1)
   {   //after we print the color coordinates, check if we want to write the space coordinates
    if(coord_print!=0)//====================
    {
     //Printing coordinates from 0 to max X and from 0 to max Y
     fprintf(fp1,"%lf %lf ",(double)(coord_factor*X),(double)(coord_factor*Y));
     X=X+1;//counting within in a row
     if(X==(img->x))
     {
      X=0;
      Y=Y+1;
     }
    }
    fputc('\n',fp1);//======================
   }
  }//---------------------------------------

  if(fp1!=NULL)printf("\nData File Print successful\n");
  fclose(fp1);
  
 
}

int main()
{ 
 FILE*fp;
 char image_name[30]="right.ppm";
  
 PPMImage *image;
 image = readPPM(image_name);
 writePPM(image_name,image);
 
 return 0;
}
