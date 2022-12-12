//#ifndef SEARCH_H
#define SEARCH_H


struct hashmap* training(int s);
void read_query(struct hashmap* hm,char* s);
//void writeToFile(char* searchPhrase); 
double rank(int num, struct hashmap* hm,char* s);
int getDF(struct hashmap* hm,char* s);
double idfScore(struct hashmap* hm,char* s);
void removeStopWords(struct hashmap* hm);
bool processLineInput(struct hashmap* hm);