#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node 
{
    bool is_word;
    struct Node *children[27];
} Node;

unsigned int nwords = 0;

Node *root;
Node *pNodeBucket;

bool load(const char *dictionary)
{
    // size is hard coded for this example code (388k allocations used)
    pNodeBucket = calloc(400000, sizeof(Node));
    Node *pNextFreeNode = pNodeBucket;

    FILE* file = fopen(dictionary, "rb");  // "b" mode is important here
    if(file == false) 
    {
        return false;
    }

    // Get size of file: should check for error codes here
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read entire file: should check for read error here
    char *pBuffer = malloc(size + 1);
    fread(pBuffer, 1, size, file);
    fclose(file);

    // File data will not be nul terminated when read
    pBuffer[size] = '\0';

    root = pNextFreeNode++;
    Node* current = NULL;

    char *pParse = pBuffer;
    char cIndex = 0;
    while (*pParse)
    {
        current = root;
        for(; *pParse != '\n' && *pParse; ++pParse)
        {
            cIndex = *pParse - 'a';
            // avoid negative index? doesn't cause segmentation fault either way
            cIndex = cIndex < 0 ? 0 : cIndex + 1;
            if(current->children[cIndex] == NULL)
            {
                current->children[cIndex] = pNextFreeNode++;
            }
            current = current->children[cIndex];
        }

        current->is_word = true;
        nwords++;

        if (*pParse == '\n') 
        {
            ++pParse;
        }
    }

    printf("Total Words in Dictionary: %d\n", nwords);

    free(pBuffer);
    return true;    
}

void walk(Node *node, int *counter)
{
    if (node == NULL)
    {
        return;
    }

    if (node->is_word)
    {
        (*counter)++;
    }

    Node *n;
    for (int i = 0; i < 27; i++)
    {
        n = node->children[i];
        if (n != NULL)
        {
            walk(n, counter);
        }
    }
}

int main()
{
    // TODO: benchmark load
    load("large.txt");
    int count = 0;
    // TODO: benchmark walk
    walk(root, &count);
    printf("Words Found in Trie: %d\n", count);
    free(pNodeBucket);
    return 0;
}