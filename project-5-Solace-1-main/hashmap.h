#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
        char* word;
        struct lldoclist* docs;
        //int num_occurrences; //removeme
        struct llnode* next;
        int df; //document frequency
};
struct lldoclist{
        char* document_id;
        int num_occurrences;
        struct lldoclist* next;

};

struct hashmap {
        struct llnode** map;
        int num_buckets;
        int num_elements;
};



struct hashmap* hm_create(int num_buckets);

int hm_get(struct hashmap* hm, char* word, char* document_id);
struct llnode* hm_get_word(struct hashmap* hm, char* word);
void free_lldoclist(struct lldoclist* docs);
void hash_table_insert(struct hashmap* hm, char* word, char* document_id, int num_occurrences);
void hm_remove(struct hashmap* hm, char* word);
void hm_destroy(struct hashmap* hm);
int hash_code(struct hashmap* hm, char* word);

#endif
