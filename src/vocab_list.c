#include <stdlib.h>
#include <string.h>

#include <doc_features.h>
#include <doc_list.h>
#include <str_list.h>
#include <vocab_list.h>

#define TBLSIZE 9323

int vocab_list_init(VocabList *vlist);
VocabItem *vocab_item_init(char *word, int cur_index);
int vocab_item_index_cmp(const void *v1, const void *v2);

/*
 * Initialize a new VocabList.
 */
int
vocab_list_init(VocabList *vlist)
{
    int i;
    vlist->tbl_size = TBLSIZE;
    vlist->size = 0;
    vlist->table = (VocabItem **)malloc(sizeof(VocabItem) * TBLSIZE);
    if (vlist->table == NULL) return -1;
    for (i = 0; i < vlist->tbl_size; i++) {
        vlist->table[i] = NULL;
    }
    return 0;
}

/*
 * Create a new VocabList from a given DocList. A VocabList is a list of
 * VocabItem objects which is managed as a hash table for fast read access.
 *  In addition to the word other information such as number of occurences is
 * stored in the VocabItem. The returned VocabList should be freed by calling
 * vocab_list_free().
 *
 * If successful, vocab_list_from_docs() returns a pointer to a VocabList. If
 * there is an error allocating memory or the doc_list parameter is NULL, it
 * returns a NULL. In the case of a memory error, errno will be set.
 */
VocabList *
vocab_list_from_docs(DocList *doc_list)
{
    VocabList *vlist = NULL;
    DocFeatures *doc = NULL;
    char *str = NULL;

    if (doc_list == NULL) return NULL;

    vlist = (VocabList *)malloc(sizeof(VocabList));
    if (vlist == NULL) return NULL;
    if (vocab_list_init(vlist) != 0) return NULL;

    for (doc = doc_list_first(doc_list); doc != NULL; doc = doc_list_next(doc_list))
    {
        for (str = str_list_first(doc->token_list); str != NULL;
        str = str_list_next(doc->token_list))
        {
            if (vocab_list_insert(vlist, str) != 0) return NULL;
        }
    }
    return vlist;
}

/*
 * Free all the memory associated with a VocabList.
 */
void
vocab_list_free(VocabList *vlist)
{
    int i;
    for (i = 0; i < vlist->tbl_size; i++) {
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

/*
 * Get the set of words in their index order for a given VocabList.
 *
 * If successful word_list() returns a str_list of the individual
 * words in this collection. It returns NULL if there is a memory or
 * other error.
 */
str_list *
word_list(VocabList *vlist)
{
    VocabItem **new_list;
    VocabItem *item;
    str_list *word_list;
    int i, j;

    word_list = str_list_init();
    if (word_list == NULL) return NULL;
    new_list = (VocabItem **)calloc(vlist->size, sizeof(VocabItem *));
    if (new_list == NULL) return NULL;

    /*
     * Create a list of pointers to each VocabItem in vlist so it
     * can be sorted.
     */
    j = 0;
    for (i = 0; i < vlist->tbl_size; i++) {
        if ((item = vlist->table[i]) != NULL) {
            while (item != NULL) {
                new_list[j] = item;
                item = item->next;
                j++;
            }
        }
    }

    /*
     * Sort the VocabItems according to their index in the list.
     */
    qsort(new_list, vlist->size, sizeof(VocabItem *), vocab_item_index_cmp);

    /*
     * Build the str_list of words.
     */
    for (i = 0; i < vlist->size; i++) {
        str_list_add(word_list, new_list[i]->word);
    }

    free(new_list);
    return word_list;
}

int
vocab_item_index_cmp(const void *voc1, const void *voc2)
{
    int r, val1, val2;

    val1 = (*(VocabItem **)voc1)->index;
    val2 = (*(VocabItem **)voc2)->index;

    if (val1 < val2) {
        r = -1;
    } else if (val1 > val2) {
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

int
vocab_list_insert(VocabList *vlist, char *word)
{
    VocabItem *item = NULL;
    int hash;

    hash = hash_vocab_word(word);
    item = vlist->table[hash];

    /*
     * When adding to the list, there are three possible situations: 1. the
     * hash slot is already in use by a different word (hash collision),
     * 2. the hash slot is already in use by a previous instance of this same
     * word, or 3. the slot is completely available.
     */
    if (item != NULL) {     /* Slot is occupied. */
        item = vocab_list_lookup(vlist, word);
        if (item == NULL) {     /* This word does not exist yet. */
            VocabItem *list_item = vlist->table[hash];
            item = vocab_item_init(word, vlist->size);
            if (item == NULL) return -1;
            /* Find the end of the chained list. */
            while (list_item->next != NULL) {
                list_item = list_item->next;
            }
            list_item->next = item;
            vlist->size++;
        } else {    /* Same as existing word, increment its count. */
            item->count++;
        }
    } else {    /* Slot is completely available. */
        item = vocab_item_init(word, vlist->size);
        vlist->size++;
        if (item == NULL) return -1;
        vlist->table[hash] = item;
    }

    return 0;
}

/*
 * Create a new VocabItem structure initializing default values and copying
 * the supplied word into the structure.
 */
VocabItem *
vocab_item_init(char *word, int cur_index)
{
    VocabItem *item = (VocabItem *)malloc(sizeof(VocabItem));
    if (item == NULL) return NULL;
    if (word != NULL) {
        item->word = (char *)malloc(strlen(word) + 1);
        if (item->word == NULL) return NULL;
        strcpy(item->word, word);
    }
    item->count = 1;
    item->index = cur_index;
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

