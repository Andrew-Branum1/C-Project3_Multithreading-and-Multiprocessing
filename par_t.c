#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <pthread.h>
#include <assert.h>

#define LEN 100

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct Node {
    int value;
    char *file;
    struct Node *next;
};

struct List {
    int size;
    struct Node *head;
    struct Node *tail;
};
struct Argument {
    char *workingstring;
    int K;
};

struct List *list;
int K;

struct Node *createNode(int number) {
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        fprintf(stderr, "%s: Couldn't create memory for the node; %s\n", "linkedlist", strerror(errno));
        exit(-1);
    }
    node->value = number;
    node->next = NULL;
    return node;
}

struct Node *createFile(char *file) {
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        fprintf(stderr, "%s: Couldn't create memory for the node; %s\n", "linkedlist", strerror(errno));
        exit(-1);
    }
    node->file = file;
    node->next = NULL;
    return node;
}

void
Pthread_mutex_lock(pthread_mutex_t *m)
{
    int rc = pthread_mutex_lock(m);
    assert(rc == 0);
}
                                                                                
void
Pthread_mutex_unlock(pthread_mutex_t *m)
{
    int rc = pthread_mutex_unlock(m);
    assert(rc == 0);
}
                                                                                
void
Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 	
	       void *(*start_routine)(void*), void *arg)
{
    int rc = pthread_create(thread, attr, start_routine, arg);
    assert(rc == 0);
}

void
Pthread_join(pthread_t thread, void **value_ptr)
{
    int rc = pthread_join(thread, value_ptr);
    assert(rc == 0);
}

void insert(struct Node *node, struct List *list){

	struct Node *ptr = malloc(sizeof(struct Node));
	
	if(list->head==NULL){
	
		node->next = list->head;
		list->head = node;

		}	
	else{
		ptr = list->head;
	while(ptr->next != NULL	){
		ptr = ptr->next;
	}
		node->next = ptr->next;
		ptr->next = node;
	}
	}

struct List *createList() {
    struct List *list = malloc(sizeof(struct List));
    if (list == NULL) {
        fprintf(stderr, "%s: Couldn't create memory for the list; %s\n", "linkedlist", strerror(errno));
        exit(-1);
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void insertNode(struct Node *node, struct List *list, int K) { 
    if(list->head == NULL || (list->head->value < node->value)) {
        node->next = list->head;
        list->head = node;
        list->tail = node;
    }
    else {
        struct Node *ptr = list->head;

        while((ptr->next != NULL) && (ptr->next->value > node->value)) { //while next value is greater than current value, keep iterating
            ptr = ptr->next;
        }
            node->next = ptr->next; 
            ptr->next = node;
    }
    list->size++;
    if(list->size == K) { //this is super rough, but once the final insertion is made, initilzes the tail
        struct Node *ptr = list->head; //you don't know if any insertion uses the real, must go fetch it, only runs once
        
        while(ptr != NULL) {
            list->tail = ptr;
            ptr = ptr->next;
        }
    }  
}

int checkDupe(struct List *list, int number) {
    struct Node *ptr = list->head;
    int dupe = 0;

    while(ptr != NULL) {
        if(ptr->value == number) { //if stored value already exists in the list, skip insertion
            dupe = 1;
            break;
        }
        else {
            ptr = ptr->next;
        }
    }
    return dupe;
}

void replaceNode(struct List *list, int number) {
    struct Node *ptr = list->head;
    int tmp = 0;

    while(ptr != NULL) {
        if(list->tail->value > number) { //check end of list, if tail is bigger, no need to iterate through list
            break;
        }
        if(ptr->value == number) { //scanning for duplicates while iterating through list
            break;
        }
        if(ptr->value < number) { //replacing value of smaller node with larger value
            tmp = ptr->value;
            ptr->value = number;
            number = tmp;
        }
        else {
            ptr = ptr->next; 
        }
    }
}


void printList(struct List *list, FILE *outfile) {
    struct Node *ptr = list->head;

    while(ptr != NULL) {
        fprintf(outfile, "%d\n", ptr->value);
        ptr = ptr->next;
    }
}

void destroyList(struct List *list) {
    struct Node *ptr = list->head;
    struct Node *tmp;
    while(ptr != NULL) {
        tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
    free(list);
}



void *readDir(void *workingstrings)
{ 
     char *workingstring;
     workingstring = (char *)workingstrings;
     int number = 0;
     struct Node *tmp = NULL;
     FILE *workingfile = fopen(workingstring, "r");
        if(workingfile == NULL) {
            printf("Error, file must exist, workingstring %s\n", workingstring);
        }
      Pthread_mutex_lock(&lock);
      while(fscanf(workingfile, "%d\n", &number) != EOF) {
            if((list->size < K) && ((checkDupe(list, number)) == 0)) {
                tmp = createNode(number);
                insertNode(tmp, list, K);
            }
            else {
                replaceNode(list, number);
            }
        }
        Pthread_mutex_unlock(&lock);
        fclose(workingfile);
        pthread_exit(0); 
        return 0;
        }
        

int main(int argc, char *argv[]) {
    K = atoi(argv[1]);
    int count = 0, ret = 0, i =0;
    if(K < 1 || K > 1000) {
        printf("Error, 1 <= K <= 1000.\n");
        return EXIT_FAILURE;
    }
    
    struct dirent *dirs2;
    DIR *dir2 = opendir(argv[2]);
    if(dir2 == NULL) {
        printf("Error, directory must exist.\n");
        return EXIT_FAILURE;
    }
    
    while((dirs2 = readdir(dir2)) != NULL) {
        if(!strcmp(dirs2->d_name, ".") || !strcmp(dirs2->d_name, "..")) { //filter
            continue;
        }
        count++;
        }

    closedir(dir2);
    
    pthread_t thread[count];
    char workingstrings[count][LEN];
   
    FILE *outfile = fopen(argv[3], "w");

    list = createList();
    
    struct dirent *dirs;
    
    DIR *dir = opendir(argv[2]);
    if(dir == NULL) {
        printf("Error, directory must exist.\n");
        return EXIT_FAILURE;
    }

    char *directory = argv[2];
    char *workingstring = (char *) malloc(LEN); //initialize a working string of ample length
    
    
    while((dirs = readdir(dir)) != NULL) {
        if(!strcmp(dirs->d_name, ".") || !strcmp(dirs->d_name, "..")) { //filter
            continue;
        }
	
        strcpy(workingstring, directory); //copy directory value into working string
        char slash[LEN] = "/"; 
        strcat(slash, dirs->d_name); //create file name of /[name]
        strcat(workingstring, slash); //create directory name
	strcpy(workingstrings[i], workingstring);
        free(workingstring); //called malloc, need to free
        i++;  
    }
    char *a = (char *) malloc(LEN);
    for(i = 0; i < count;i++){
      strcpy(a, workingstrings[i]);//copies directory into char *a
      ret = pthread_create(&thread[i], NULL, readDir, (void *)workingstrings[i]);//creates thread with directory as argument to readDir
      if(ret!=0){
           printf("pthread_create failed in %d_th pass\n",i);
           exit(EXIT_FAILURE);        
       }
       }
    free(a);
   
    for(i = 0; i < count; i++){
    	ret = pthread_join(thread[i], NULL);// joins each thread back to the main thread after they complete
    	if(ret!=0){
               printf("pthread_join failed in %d_th pass\n",i);
               exit(EXIT_FAILURE);        
            }
    }
    printList(list, outfile);
    fclose(outfile);
    closedir(dir);
    destroyList(list);

    return 0;   
}
