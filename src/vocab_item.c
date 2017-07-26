#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hashtbl.h>
#include <vocab_item.h>

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

/*
 * Determine if two VocabItem structures are the same word.
 */
int
vocab_item_match(const void *v1, const void *v2)
{
    return strncmp(((VocabItem *)v1)->word, ((VocabItem *)v2)->word, strlen(((VocabItem *)v1)->word));
}

/*
 * Generate a hash value for a given VocabItem word.
 */
int
vocab_item_hash(const void *v)
{

    const char *ptr;
    int val;

    val = 0;
    ptr = ((VocabItem *)v)->word;
    while (*ptr != '\0') {
        int tmp;
        val = (val << 4) + (*ptr);
        if ((tmp = (val & 0xf0000000))) {
            val = val ^ (tmp >> 24);
            val = val ^ tmp;
        }
        ptr++;
    }

    return abs(val) % VOCAB_ITEM_TBL_SIZE;

}

void
vocab_item_free(void *v) {
    free(((VocabItem *)v)->word);
    free(v);
    return;
}

