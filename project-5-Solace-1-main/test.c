

#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

int main(void){
   struct hashmap* hm = hm_create(100);
    char current[20], c; 

    //extract words from d1. 
    FILE * stream = fopen("D1.txt", "r");
    c = fgetc(stream);
    int i = 0;
    while (c != EOF)
    {
        if(c == ' ' || c == '\n' || c == '\0'){
         
         current[i] = '\0';//end line
         
         hm_put(hm,current,"D1",1);
        // printf("%s\n",current);

         c = fgetc(stream);
         //reset current
         i = 0;
        }else{
        current[i] = c; 
        //printf ("%c", current[i]);
        c = fgetc(stream);
        i++;   
           //add last string.
          if(c == EOF){
               current[i] = '\0';
               hm_put(hm,current,"D1",1);
              // printf("%s\n",current);
                i = 0;
          }


        }
    }
    fclose(stream);
    //    d2
    stream = fopen("D2.txt", "r");
    c = fgetc(stream);
    i = 0;
    while (c != EOF)
    {
        if(c == ' ' || c == '\n' || c == '\0'){
         
         current[i] = '\0';//end line
         
         hm_put(hm,current,"D2",1);
         //printf("%s",current);
         c = fgetc(stream);
         //reset current
         i = 0;
        }else{
        current[i] = c; 
        c = fgetc(stream);
        i++;   
           //add last string.
          if(c == EOF){
               current[i] = '\0';
               hm_put(hm,current,"D2",1);
               //printf("%s",current);
                i = 0;
          }


        }
    }
    //d3
     fclose(stream);
    stream = fopen("D3.txt", "r");
    c = fgetc(stream);
    i = 0;
    while (c != EOF)
    {
        if(c == ' ' || c == '\n' || c == '\0'){
         
         current[i] = '\0';//end line
         
         hm_put(hm,current,"D3",1);
         //printf("%s",current);
         c = fgetc(stream);
         //reset current
         i = 0;
        }else{
        current[i] = c; 
        //printf ("%c", current[i]);
        c = fgetc(stream);
        i++;   
           //add last string.
          if(c == EOF){
               current[i] = '\0';
               hm_put(hm,current,"D3",1);
               //printf("%s",current);
                i = 0;
          }


        }
    }
     fclose(stream);
    //printf("please run");
   
    //INSERT RUN CODE HERE
    //part 1b) remove stop words

    removeStopWords(hm);
    







    hm_destroy(hm);

   return 0; 
}

