#include <stdio.h>
#include <stdlib.h>

#define FILENAME "params.h"

void main(int argc, char *argv[])
{
  int np,size;
  int mimax,mjmax,mkmax,mi,mj,mk;

  FILE *fp;

  if(argc != 3) {
    printf("Usage: %s size cpu\n",argv[0]);
    printf("size= SSMALL or SMALL or MID or LARGE\n");
    printf("cpu= n**2\n");
    exit(1);
  }

  np= atoi(argv[2]);

  if(np!=1 && np!=2 && np!=4 && np!=8 && np!=16 && np!=32 && np!=64){
    printf("Fatal Number of CPU\n");
    printf("2**n n= CPU\n");
    exit(1);
  }

  if (!strcmp(argv[1],"SSMALL") || !strcmp(argv[1], "ssmall"))
    size= 0;
  else if (!strcmp(argv[1],"SMALL") || !strcmp(argv[1], "small"))
    size= 1;
  else if (!strcmp(argv[1],"MID") || !strcmp(argv[1], "mid"))
    size= 2;
  else if (!strcmp(argv[1],"LARGE") || !strcmp(argv[1], "large"))
    size= 3;

  if(size == 0){
    mi= 33;
    mj= 33;
    mk= 65;
  } else if(size == 1){
    mi= 65;
    mj= 65;
    mk= 129;
  } else if(size == 2){    
    mi= 129;
    mj= 129;
    mk= 257;
  } else if(size == 3){    
    mi= 257;
    mj= 257;
    mk= 513;
  }
  mimax= (mi-1)/np + 3;
  mjmax= mj;
  mkmax= mk;

  fp= fopen(FILENAME,"w");
  if(fp == NULL){
    printf(" configuration file %s does not exist \n",FILENAME);
    exit(1);
  }
  fprintf(fp,"#define NCPU  %d\n",np);
  fprintf(fp,"#define MIMAX  %d\n",mimax);
  fprintf(fp,"#define MJMAX  %d\n",mjmax);
  fprintf(fp,"#define MKMAX  %d\n",mkmax);
  fprintf(fp,"#define MI  %d\n",mi);
  fprintf(fp,"#define MJ  %d\n",mj);
  fprintf(fp,"#define MK  %d\n",mk);
  fclose(fp);
}
