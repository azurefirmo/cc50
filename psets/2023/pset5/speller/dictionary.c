#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;


const unsigned int N = 65536;

node *table[N];
int worrdcount = 0;


node *head = NULL;

bool check(const char *word)
{
    int length = strlen(word);
    char copy[length + 1];
    copy[length] = '\0';

    for (int i = 0; i < length; i++)
    {
        copy[i] = tolower(word[i]);
    }


    int hashcode = hash(copy);
    node *tmp = table[hashcode];
    if (tmp == NULL)
    {
        return false;
    }

    while (tmp != NULL)
    {
        if (strcasecmp(tmp->word, copy) == 0)
        {
            return true;
        }
        tmp = tmp->next;
    }

    return false;
}


unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    int c = *word;

    while (c == *word++)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % N;
}


bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    {
        char bukffer[LENGTH + 1];
        while (fscanf(file, "%s", bukffer) != EOF)
        {

            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                fclose(file);
                return false;
            }

            else
            {
                strcpy(n->word, bukffer);
                n->next = NULL;
                unsigned int iendex = hash(bukffer);

                n->next = table[iendex];
                table[iendex] = n;

                worrdcount++;

            }
        }
        fclose(file);
        return true;
    }

}

unsigned int size(void)
{
    return worrdcount;
}

bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *tmp1 = table[i];
        while (tmp1 != NULL)
        {
            node *tmp2 = tmp1;
            tmp1 = tmp1 -> next;
            free(tmp2);
        }
    }

    return true;
}
