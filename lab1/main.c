#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TABLE_SIZE 257

typedef struct CharNode
{
    unsigned char c;
    struct CharNode *next;
} CharNode;

typedef struct HashNode
{
    unsigned char c;
    int count;
    struct HashNode *next;
} HashNode;

static unsigned int hashChar(unsigned char c)
{
    return c % TABLE_SIZE;
}

static void printChar(unsigned char c)
{
    if (c == '\n')
        printf("\\n, ");
    else if (c == '\t')
        printf("\\t, ");
    else if (c == '\r')
        printf("\\r, ");
    else if (c == ' ')
        printf("[space], ");
    else if (isprint(c))
        printf("%c, ", c);
    else
        printf("\\x%02X, ", c);
}

static CharNode *createCharNode(unsigned char c)
{
    CharNode *node = (CharNode *)malloc(sizeof(CharNode));
    if (node == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    node->c = c;
    node->next = NULL;
    return node;
}

static void appendChar(CharNode **headRef, CharNode **tailRef, unsigned char c)
{
    CharNode *node = createCharNode(c);

    if (*headRef == NULL)
    {
        *headRef = node;
        *tailRef = node;
        return;
    }

    (*tailRef)->next = node;
    *tailRef = node;
}

static void hashUpdate(HashNode *table[], unsigned char c)
{
    unsigned int idx = hashChar(c);
    HashNode *curr = table[idx];

    while (curr != NULL)
    {
        if (curr->c == c)
        {
            curr->count++;
            return;
        }
        curr = curr->next;
    }

    curr = (HashNode *)malloc(sizeof(HashNode));
    if (curr == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    curr->c = c;
    curr->count = 1;
    curr->next = table[idx];
    table[idx] = curr;
}

static void buildHashFromCharList(CharNode *charHead, HashNode *table[])
{
    CharNode *curr = charHead;
    while (curr != NULL)
    {
        hashUpdate(table, curr->c);
        curr = curr->next;
    }
}

static void printHashTable(HashNode *table[])
{
    int i;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        HashNode *curr = table[i];
        while (curr != NULL)
        {
            printChar(curr->c);
            // printf(" : %d\n", curr->count);
            curr = curr->next;
        }
    }
}

static void freeCharList(CharNode *head)
{
    CharNode *curr = head;
    while (curr != NULL)
    {
        CharNode *next = curr->next;
        free(curr);
        curr = next;
    }
}

static void freeHashTable(HashNode *table[])
{
    int i;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        HashNode *curr = table[i];
        while (curr != NULL)
        {
            HashNode *next = curr->next;
            free(curr);
            curr = next;
        }
    }
}

int main(void)
{
    char FILE_NAME[] = "main.c";

    FILE *file = fopen(FILE_NAME, "r");
    CharNode *charHead = NULL;
    CharNode *charTail = NULL;
    HashNode *table[TABLE_SIZE] = {0};
    int ch;

    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file: %s\n", FILE_NAME);
        return 1;
    }

    while ((ch = fgetc(file)) != EOF)
    {
        appendChar(&charHead, &charTail, (unsigned char)ch);
    }

    fclose(file);

    buildHashFromCharList(charHead, table);

    printHashTable(table);
    freeHashTable(table);
    freeCharList(charHead);

    return 0;
}
