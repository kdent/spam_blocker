#include <stdlib.h>
#include <string.h>

#include <doc_analysis.h>
#include <doc_list.h>
#include <vocab_list.h>

#define TBLSIZE 9323

int vocab_list_init(VocabList *vlist);
VocabItem *vocab_item_init(char *word);

char **
word_list(VocabList *vlist)
{
    char **wlist;
    int j = 0;

    wlist = (char **)malloc(sizeof(char *) * vlist->item_count); 
    if (wlist == NULL) return NULL;
    memset(wlist, 0, sizeof(char *) * vlist->item_count);

    for (int i = 0; i < vlist->tbl_size; i++) {
        VocabItem *item = NULL;
        if ((item = vlist->table[i]) != NULL) {
            while (item != NULL) {
                wlist[j] = (char *)malloc(strlen(item->word) + 1);
                if (wlist[j] == NULL) return NULL;
                strcpy(wlist[j], item->word);
                j++;
                item = item->next;
            }
        }
    }

    return wlist;
}

void
word_list_free(int wlist_size, char **wlist)
{
    for (int i = 0; i < wlist_size; i++) {
        free(wlist[i]);
    }
    free(wlist);
}

VocabList *
vocab_list(DocList *doc_list)
{
    VocabList *vlist = NULL;
    DocAnalysis *doc = NULL;

    if (doc_list == NULL) return NULL;

    vlist = (VocabList *)malloc(sizeof(VocabList));
    if (vlist == NULL) return NULL;
    if (vocab_list_init(vlist) != 0) return NULL;

    for (doc = doc_list_first(doc_list); doc != NULL; doc = doc_list_next(doc_list))
    {
        for (char *str = str_list_first(doc->token_list); str != NULL; str = str_list_next(doc->token_list))
        {
            if (vocab_list_insert(vlist, str) != 0) return NULL;
        }
    }
    return vlist;
}

void
vocab_list_free(VocabList *vlist)
{
    for (int i = 0; i < vlist->tbl_size; i++) {
        if (vlist->table[i] != NULL) {
            VocabItem *item, *next;
            item = vlist->table[i];
            while (item != NULL) {
                next = item->next;
                free(item->word);
                free(item);
                item = next;
            }
        }
    }
    free(vlist->table);
    free(vlist);
}

int
vocab_list_init(VocabList *vlist)
{
    vlist->tbl_size = TBLSIZE;
    vlist->slot_count = 0;
    vlist->item_count = 0;
    vlist->table = (VocabItem **)malloc(sizeof(VocabItem) * TBLSIZE);
    if (vlist->table == NULL) return -1;
    for (int i = 0; i < vlist->tbl_size; i++) {
        vlist->table[i] = NULL;
    }
    return 0;
}

int
vocab_list_insert(VocabList *vlist, char *word)
{
    VocabItem *item = NULL;
    int hash;

    hash = hash_vocab_word(word);
    item = vlist->table[hash];

    /* This bucket might be in use by this word or another. */
    if (item != NULL) {
        item = vocab_list_lookup(vlist, word);
        if (item == NULL) {     /* This word does not exist yet. */
            VocabItem *list_item = vlist->table[hash];
            item = vocab_item_init(word);
            if (item == NULL) return -1;
            /* Find the end of the chained list. */
            while (list_item->next != NULL) {
                list_item = list_item->next;
            }
            list_item->next = item;
            vlist->item_count++;
        } else {
            item->count++;
        }
    } else {    /* Bucket is completely free. */
        vlist->item_count++;
        vlist->slot_count++;
        item = vocab_item_init(word);
        if (item == NULL) return -1;
        vlist->table[hash] = item;
    }

    return 0;
}

VocabItem *
vocab_item_init(char *word)
{
    VocabItem *item = (VocabItem *)malloc(sizeof(VocabItem));
    if (item == NULL) return NULL;
    item->word = (char *)malloc(strlen(word) + 1);
    if (item->word == NULL) return NULL;
    strcpy(item->word, word);
    item->count = 1;
    item->next = NULL;
    return item;
}

VocabItem *
vocab_list_lookup(VocabList *vlist, char *word)
{
    VocabItem *item = NULL;
    int hash;

    if (vlist == NULL) return NULL;
    if (word == NULL) return NULL;

    hash = hash_vocab_word(word);
    item = vlist->table[hash];
    if (item != NULL) {
        do {
            if (strcmp(item->word, word) == 0) {
                break;
            } else {
                item = item->next;
            }
        } while (item != NULL);
    }
    return item;
}

int
hash_vocab_word(const void *word)
{

    const char *ptr;
    int val;

    val = 0;
    ptr = word;
    while (*ptr != '\0') {
        int tmp;
        val = (val << 4) + (*ptr);
        if ((tmp = (val & 0xf0000000))) {
            val = val ^ (tmp >> 24);
            val = val ^ tmp;
        }
        ptr++;
    }

    return abs(val) % TBLSIZE;

}

