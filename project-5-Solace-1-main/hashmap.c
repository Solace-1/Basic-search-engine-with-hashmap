#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <string.h>

#include "hashmap.h"

#define HM_MAX_LOAD 0.75
struct hashmap* hm_create(int num_buckets){

  //create the hashmap
  struct hashmap * hm = malloc(sizeof(struct hashmap));
  
  //TODO the rest of the setup!
  hm->map = calloc(num_buckets, sizeof(struct llnode));

  hm->num_elements = 0;
  hm->num_buckets=num_buckets;
  for(int i=0; i < hm->num_buckets; i++) {
    //hm->map[i] = malloc(sizeof(struct llnode));
    hm->map[i] = NULL;
  }
  return hm;
}

struct llnode* hm_get_word(struct hashmap* hm, char* word){

    int h = hash_code(hm,word); //get hash
    struct llnode* cur = hm->map[h];
    if(hm->map[h] == NULL || cur == NULL){
        return NULL; 
    }
    if(!strcmp(cur->word, word)){//removed start loop to see if this fixes error. 
           
            return cur; //if it cant find the doc id in word, then it doesnt exist. 
    }
    while(cur->next != NULL && cur != NULL){
        if(!strcmp(cur->word, word)){
           return cur; 
        }
        cur = cur->next;
    }
    if(!strcmp(cur->word, word)){//check last value in ll
                { return cur; }
    }

    return NULL;
}


int hm_get(struct hashmap* hm, char* word, char* document_id){
    int h = hash_code(hm,word); //get hash
    struct llnode* cur = hm->map[h];
    struct lldoclist* doclist; 
    if(hm->map[h] == NULL || cur == NULL){
        return -1; 
    }
    while(cur != NULL){
        if(!strcmp(cur->word,word)){
            doclist = cur->docs; 
            while(doclist != NULL){
                if(!strcmp(doclist->document_id,document_id))
                { return doclist->num_occurrences;}
                doclist = doclist->next;
            }
        }
        cur = cur->next;
    }


    return -1;
}

void hash_table_insert(struct hashmap* hm, char* word, char* document_id, int num_occurrences){
  
  //check the load on the hashtable, if > max load, resize!
  int h = hash_code(hm,word); //get hash
  struct llnode* cur = hm->map[h];
  struct lldoclist* docid = NULL;
  if(hm_get_word(hm,word) == NULL){//word not in hashmap
            struct llnode* head = malloc(sizeof(struct llnode));
            struct lldoclist* docs = malloc(sizeof(struct lldoclist));
            head->word = strdup(word);
            docs->num_occurrences = num_occurrences;
            docs->document_id = strdup(document_id);
            docs->next = NULL;
            head->next = hm->map[h];
            head->docs = docs;
            head->df = 1; 
            hm->num_elements++;
            hm->map[h] = head; 
            return;
 }else if(hm_get(hm,word,document_id) == -1){//word is in hashmap but the document id isnt. 
        while(cur != NULL){
            if(!strcmp(cur->word,word)) { //if you get to the word, add document id to the doc id hashmap
                 struct lldoclist* docs = malloc(sizeof(struct lldoclist));
                 docs->num_occurrences = num_occurrences;
                 docs->document_id =strdup(document_id);
                 docs->next = cur->docs;
                 cur->docs = docs;
                 cur->df++; 
                 return; 
            }
            cur = cur-> next; 
        }
 }else{ // word  and doc id exist in hashmap
    while(cur != NULL){
        if(!strcmp(cur->word,word)){
            docid = cur->docs;
            while(docid != NULL){
                if(!strcmp(docid->document_id,document_id)){
                    docid->num_occurrences++; 
                    return;
                }
                docid = docid->next;
            }
        }

    
    cur = cur-> next;
    }
 }
}

void hm_remove(struct hashmap* hm, char* word){
    int h = hash_code(hm, word);
    struct llnode* n = hm->map[h];
    struct lldoclist* docs;

    if (n == NULL) {
        return;
    }

    if (!strcmp(n->word, word)) {//if the first word in the hash
        docs = n->docs;
        hm->map[h] = n->next;
        free_lldoclist(docs);
        free(n->word);
        free(n);
        hm->num_elements--;
        return;
    }
    while (n->next != NULL) {
        if (strcmp(n->next->word, word) == 0) {
            struct llnode* temp = n->next;
            n->next = temp->next;
            free_lldoclist(temp->docs);
            free(temp->word);
            free(temp);
                    hm->num_elements--;

            
            return;
        }
        n = n->next;
    }
}


void hm_destroy(struct hashmap* hm){

  for (int i = 0; i < hm->num_buckets; i++)
  {
    if( hm->map[i] != NULL){
        struct llnode * cur = hm->map[i];
        struct llnode * next = cur->next;
        while (cur != NULL) {
            next = cur->next;
            //hm_remove(hm,cur->word);

            free(cur->word);
            
            struct lldoclist* curDoc = cur->docs;
            struct lldoclist* prevDoc = curDoc;

            while(curDoc != NULL ){
                prevDoc = curDoc;
                curDoc = curDoc->next;
                free(prevDoc->document_id); //issue with freeing this more than once ???
                //free(prevDoc->num_occurrences);
                free(prevDoc);
                
            }
            //free(cur->docs);
            free(cur);
            cur = next;
            hm->num_elements--;//delete an element for each freed word

        }
    }
  }
  
  free(hm->map);
  free(hm);
}

int hash_code(struct hashmap* hm, char* word){
  double k = 0;
  double len = (double) strlen(word);//too afraid to turn into int
  
  for(int i = 0; i < len; i++){
    k += word[i];
  }

  return (int) ( k) % hm->num_buckets;
}

void free_lldoclist(struct lldoclist* docs) {
    struct lldoclist* cur = docs;
    while (cur != NULL) {
        struct lldoclist* next = cur->next;
        free(cur->document_id);
        free(cur);
        cur = next;
    }
}