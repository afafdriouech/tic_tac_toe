#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./user.h"


int login(char *ch)
{
 FILE* f; 
 int a=strlen(ch)+1;
 char tampon[a];
 f=fopen("../user.txt","r"); 
 if (f!=NULL)
   {
      while(fgets(tampon,a,f)!=NULL)
      { 
        if(strcmp(tampon,ch)==0)
        return 1;
      }
   }
 return 0;
}


