#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char c;
    int num;
    struct Node *next;
} Node;

int find(char c, Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        if (curr->c == c) {
            curr->num += 1;
            return 1;
        }
        curr = curr->next; 
    }
    return 0;
}

void append(char c, Node **headRef) {

    // if (find(c, *headRef)) return;
    
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->c = c;
    newNode->num = 1;
    newNode->next = NULL;

    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }

    Node *temp = *headRef;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void printLL(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        // printf("%c : %d\n", curr->c, curr->num);
        if(curr->c == '\n'){
            printf("\\n, ");
        }
        else if (curr->c == '\t'){
            printf("\\t, ");
        }
        else{printf("%c, ",curr->c);}
        
        curr = curr->next;
    }
}

int main() {
    // char* test = "aab";
    FILE *file = fopen("main.c","r");
    Node *head = NULL;

    // int n = strlen(test);
    // for (int i = 0; i < n; i++) {
    //     append(test[i], &head); 
    // }
    char ch;
    while((ch = fgetc(file))!=EOF){
        append(ch, &head);
    }
    printLL(head);

    
    return 0;
}
