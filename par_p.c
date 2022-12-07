#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define LEN 100
#define MESSAGESIZE 128

struct message_s {
    long type;
    char content[MESSAGESIZE];
};

struct Node {
    int value;
    struct Node *next;
};

struct List {
    int size;
    struct Node *head;
    struct Node *tail;
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

void insertNode(struct Node *node, struct List *list) { 
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
    /*
    this is super rough, but once the final insertion is made, initilzes the tail
    you don't know if any insertion uses the real, must go fetch it, only runs once
    */
    if(list->size == K) {
        struct Node *ptr = list->head; 
        
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

void readFile(char *workingstring) {
    int number = 0;
    
    struct Node *tmp = NULL;

    FILE *workingfile = fopen(workingstring, "r");
    if(workingfile == NULL) {
        printf("Error, file must exist.\n");
    }
    while(fscanf(workingfile, "%d\n", &number) != EOF) {
        if((list->size < K) && ((checkDupe(list, number)) == 0)) {
            tmp = createNode(number);
            insertNode(tmp, list);
        }
        else {
            replaceNode(list, number);
        }
    }
    fclose(workingfile);
}     

int main(int argc, char *argv[]) {
    K = atoi(argv[1]);
    if(K < 1 || K > 1000) {
        printf("Error, 1 <= K <= 1000.\n");
        return EXIT_FAILURE;
    }
    int count = 0, i = 0;
    
    struct dirent *dirstmp;
    DIR *dirtmp = opendir(argv[2]);
    if(dirtmp == NULL) {
        printf("Error, directory must exist.\n");
        return EXIT_FAILURE;
    }
    while((dirstmp = readdir(dirtmp)) != NULL) {
        if(!strcmp(dirstmp->d_name, ".") || !strcmp(dirstmp->d_name, "..")) { //filter
            continue;
        }
        count++;
    }
    closedir(dirtmp);

    pid_t pid;

    FILE *outfile = fopen(argv[3], "w");
    list = createList();
    
    struct dirent *dirs;
    DIR *dir = opendir(argv[2]);
    if(dir == NULL) {
        printf("Error, directory must exist.\n");
        return EXIT_FAILURE;
    }
    char *directory = argv[2];
    char workingstrings[count][LEN];
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

    struct message_s message;
    int message_queue_id;
    key_t key;

    if ((key = ftok("par_p.c", 1)) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((message_queue_id = msgget(key, IPC_CREAT|0666)) == -1) {
        perror("msgget");
        exit(1);
    }

    for(i = 0; i < count;i++) {
        pid = fork();
        if(pid < 0) { //error
            printf("Error, fork failed\n");
            break;
        }
        else if(pid == 0) { //child
            strcpy(message.content, "hello from child process");
            message.type = 1;

            if(msgsnd(message_queue_id, &message, MESSAGESIZE, 0) == -1) {
                perror("Error in msgsnd");
            }
            readFile(workingstrings[i]);
        }
        else { //parent handeling
            if (msgrcv(message_queue_id, &message, MESSAGESIZE, 0, 0) == -1) {
                perror("msgrcv");
                exit(1);
            }
            break;
        } 
    }

    wait(NULL);

    if(pid == 0) { //output the information on parent process
        printList(list, outfile);
        fclose(outfile);        
        closedir(dir);
        destroyList(list);
        
        printf("Successful print to: %s\n", argv[3]);

        return 0;
    }
    else { //kill child process
        msgctl(message_queue_id, IPC_RMID, NULL);
        fclose(outfile);
        closedir(dir);
        destroyList(list);
        kill(pid, SIGTERM);
    }
}
