#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <string.h>

 #include "hashmap.h"
 #include "search.h"


int N = 3; //N equals the amount of files. when we implement glob, we will set this to 0 and then add 1 for each file. 
    //extract words from d1.hardcoded for now. later, i'll need to fix this. 

bool first = true; //global variable to prevent search string from printing twice
bool first2 = true; //other global variable to prevent read_query from printing on initialization
int main(void){
    struct hashmap* hm;
    int buckets;
    //char w[20]; //word in c; 
    //char c[100];//phrase to search for
  //narahari que  stions:
  /*
  do i need to make sure user is inserting an int? yes
  do i need to remove conditional jump valgrind errors?
  where am i printing file.txts?
  */
    bool boo = true; //boolean to determine whether or not buckets is properly implemented
    int x;
    while(boo){
      printf("How many buckets?:\n");
      x = scanf("%d",&buckets);
    
      if(x == 1){//we need to check to see if scanned value is an int. if not, try again
    //printf("%d\n",buckets);
        if(buckets > 0){
          boo = false;
        }else{
          printf("ERROR: MUST INPUT INTEGER GREATER THAN 0. Try again.\n");
        }
      }else{
        printf("ERROR: Invalid Input\n");
        return 0;
      }
    }
    hm = training(buckets);
    removeStopWords(hm);
    //this needs to be in a loop
    bool b = true;
    while(b){
     b = processLineInput(hm);
     
     
    }
    
   
   hm_destroy(hm);
   return 0; 
}


//provides a score for each document


void read_query(struct hashmap* hm,char s[]){
  //step 1: get each word
  char word[20];
  int count = 0;
  char docs[3][10] = {"D1.txt" , "D2.txt", "D3.txt"};//REMEMBER TO FIX IF U EVER YOU GLOB
    double scores[N]; //score values of each txt file-> 1,2,3. 
    for(int i = 0; i < N; i++){
    scores[i] = 0; //initialize scores. 
    }
  //note- you have docs which has the name of each one. 
  //printf("%s",s);

  for(int i = 0; i < (int) strlen(s); i++){

    if(s[i] == ' ' || s[i] == '\0'){
      word[count] = '\0';
      count = 0; 

      if(hm_get_word(hm,word) != 0){//hm_get(hm,word,document_id)-how to get doc id??? 
            //step 1: go thru docs
            //step 2: if document_id == docs[i], then add numOccurences to scores[N]

        //CREATE WHILE LOOP TO GO THRU ALL OF THE NODES
        int h = hash_code(hm,word);
        struct llnode* cur = hm->map[h];
        while(cur != NULL){
          if(!strcmp(cur->word,word)){
          struct lldoclist* docid =  cur->docs;
          while(docid != NULL){
           for(int i =0; i < N; i++){
             if(!strcmp(docid->document_id,docs[i])){
                //printf("%d",docid->num_occurrences);
                scores[i] += rank(docid->num_occurrences,hm,word);
              }
            }
            docid = docid->next;
          }
          }
        cur = cur->next;
       }
      }
      
    } else{
      word[count] = s[i];
      count++;
    }
  }
  




  //hardcoded score adder: CHANGE IF GLOB
  //move to its own method later. 

  FILE *f = fopen("p5docs/search_scores.txt", "a");
  if(!first2){//prevents printing on first go
    if(scores[2] == 0 && scores[1] == 0 && scores [0] == 0){
      printf("None of the search words were found in the documents :(\n");
      fprintf(f, "None of the search words were found in the documents :(\n");
    }
    else if(scores[2] <= scores[1] && scores[1] <=scores[0]){ //2<1<0
      if(scores[0] != 0.0){
      printf("%s %f\n",docs[0],scores[0]);
      fprintf(f,"%s\t%f\n",docs[0],scores[0]);
      }
      if(scores[1] != 0.0){
      printf("%s %lf \n",docs[1],scores[1]);
      fprintf(f,"%s\t%f\n",docs[1],scores[1]);
      }
      if(scores[2] != 0.0){
      printf("%s %lf \n",docs[2],scores[2]);
      fprintf(f,"%s\t%f\n",docs[2],scores[2]);
      }
    }else if(scores[2] <= scores[0] && scores[0] <=scores[1]){ //2<0<1
      if(scores[1] != 0.0){
      printf("%s %f\n",docs[1],scores[1]);
      fprintf(f,"%s\t%f\n",docs[1],scores[1]);
      }
      if(scores[0] != 0.0){
      printf("%s %lf \n",docs[0],scores[0]);
      fprintf(f,"%s\t%f\n",docs[0],scores[0]);
      }
      if(scores[2] != 0.0){
      printf("%s %lf \n",docs[2],scores[2]);
      fprintf(f,"%s\t%f\n",docs[2],scores[2]);
      }
    }else if(scores[1] <= scores[2] && scores[2] <=scores[0]){ //1<2<0
     if(scores[0] != 0.0){
      printf("%s %f\n",docs[0],scores[0]);
      fprintf(f,"%s\t%f\n",docs[0],scores[0]);
      }
      if(scores[2] != 0.0){
      printf("%s %lf \n",docs[2],scores[2]);
      fprintf(f,"%s\t%f\n",docs[2],scores[2]);
      }
      if(scores[1] != 0.0){
      printf("%s %lf \n",docs[1],scores[1]);
      fprintf(f,"%s\t%f\n",docs[1],scores[1]);
      }
    }else if(scores[1] <= scores[0] && scores[0] <=scores[2]){ // 1<0<2
      if(scores[2] != 0.0){
      printf("%s %f\n",docs[2],scores[2]);
      fprintf(f,"%s\t%f\n",docs[2],scores[2]);
      }
      if(scores[0] != 0.0){
      printf("%s %lf \n",docs[0],scores[0]);
      fprintf(f,"%s\t%f\n",docs[0],scores[0]);
      }
      if(scores[1] != 0.0){
      printf("%s %lf \n",docs[1],scores[1]);
      fprintf(f,"%s\t%f\n",docs[1],scores[1]);
      }
    }else if(scores[0] <= scores[1] && scores[1] <=scores[2]){ // 0<1<2
      if(scores[2] != 0.0){
      printf("%s %f\n",docs[2],scores[2]);
      fprintf(f,"%s\t%f\n",docs[2],scores[2]);
      }
     if(scores[1] != 0.0){
      printf("%s %lf \n",docs[1],scores[1]);
      fprintf(f,"%s\t%f\n",docs[1],scores[1]);
      }
     if(scores[0] != 0.0){
      printf("%s %lf \n",docs[0],scores[0]);
      fprintf(f,"%s\t%f\n",docs[0],scores[0]);
      }
    }else if(scores[0] <= scores[2] && scores[2] <=scores[1]){ // 0<2<1
      if(scores[1] != 0.0){
      printf("%s %lf \n",docs[1],scores[1]);
      fprintf(f,"%s\t%f\n",docs[1],scores[1]);
      }
      if(scores[2] != 0.0){
      printf("%s %f\n",docs[2],scores[2]);
      fprintf(f,"%s\t%f\n",docs[2],scores[2]);
      }
      if(scores[0] != 0.0){
      printf("%s %lf \n",docs[0],scores[0]);
      fprintf(f,"%s\t%f\n",docs[0],scores[0]);
      }
    }
  }else{
    first2 = false;
  }
  
   fclose(f);
}


bool processLineInput(struct hashmap* hm){
    char c[100];
    if(!first){
      printf("Enter search string or X to exit\n");
    }else{
      first = false;
    }
    // Read a line of input
    // printf("%d",hm_get_word(hm,"computer")->df);
    // printf("\n%d",hm_get(hm,"computer","D1.txt"));

    if (fgets(c, 96, stdin) != NULL) {
      
        if(!strcmp(c,"X\n")){
        //
        return false;
        }
        for(int i = 0; i < 100; i ++){
          if(c[i] == '\n'){//add a char to the end of the input- allows user to stop at the last space when 
                            //going thru query
            c[i] = ' ';
            c[i+1] = 'c';
            c[i+2] = '\0';
            break;
          }
        }
        
        read_query(hm,c);// ->print out each word d1. PRINT RANKINGS IN ORDER!!!!! //uninitialized value in c. 

        return true; // Return if there is no input
    }
    return true; 
}

struct hashmap* training(int buckets){
    
    struct hashmap* hm = hm_create(buckets);
    char current[20], c; 

    FILE * stream = fopen("p5docs/D1.txt", "r");
    c = fgetc(stream);
    int i = 0;
    while (c != EOF)
    {
        if(c == ' ' || c == '\n' || c == '\0'){
         
         current[i] = '\0';//end line
         
         hash_table_insert(hm,current,"D1.txt",1);
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
               hash_table_insert(hm,current,"D1.txt",1);
              // printf("%s\n",current);
                i = 0;
          }


        }
    }
    fclose(stream);
    //    d2
    stream = fopen("p5docs/D2.txt", "r");
    c = fgetc(stream);
    i = 0;
    while (c != EOF)
    {
        if(c == ' ' || c == '\n' || c == '\0'){
         
         current[i] = '\0';//end line
         
         hash_table_insert(hm,current,"D2.txt",1);
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
               hash_table_insert(hm,current,"D2.txt",1);
               //printf("%s",current);
                i = 0;
          }


        }
    }
    //d3
     fclose(stream);
    stream = fopen("p5docs/D3.txt", "r");
    c = fgetc(stream);
    i = 0;
    while (c != EOF)
    {
        if(c == ' ' || c == '\n' || c == '\0'){
         
         current[i] = '\0';//end line
         
         hash_table_insert(hm,current,"D3.txt",1);
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
               hash_table_insert(hm,current,"D3.txt",1);
               //printf("%s",current);
                i = 0;
          }


        }
    }
    fclose(stream);

    return(hm);
}

int getDF(struct hashmap* hm,char* s){
    int h = hash_code(hm,s);//hash value of a specific word. 
  
      struct llnode* cur = hm->map[h];
      while(cur != NULL){
        if(!strcmp(cur->word,s)){
          return cur->df;
        }

        cur = cur->next;
      }
      return 0; 
  

}

double rank(int num, struct hashmap* hm,char* s){
  double d = (double) num;

  // printf("%d",num);
  // printf("%lf <-Score", d * idfScore(hm,s));
  return d * idfScore(hm,s);
  

}

double idfScore(struct hashmap* hm,char* s){
    int df = getDF(hm,s); //document frequency. 
   if(df == 0){
    return log((double) N);
   }else{
    return log((double) N / (double) df);
   }
}

void removeStopWords(struct hashmap* hm){//BROKEN
  int size = hm->num_buckets;
  for(int i = 0; i < size; i++){
    struct llnode* cur = hm->map[i];
    while(cur != NULL){
      if(idfScore(hm,cur->word) == 0){
        //printf("%s ",cur->word);
        hm_remove(hm,cur->word); //remove all instances of the word
        cur= hm->map[i];
      }
      cur = cur->next;
    }

  }
}

