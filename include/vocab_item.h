#ifndef MAKE_VOCAB_ITEM_H
#define MAKE_VOCAB_ITEM_H

#define VOCAB_ITEM_TBL_SIZE 9323

typedef struct _vocab_item {
    char *word;         /* the word */
    int count;          /* the number of times it occurs */
    int index;          /* this word's index in the list */
    struct _vocab_item *next;
} VocabItem;

VocabItem *vocab_item_init(char *word, int cur_index);
int vocab_item_match(const void *v1, const void *v2);
int vocab_item_hash(const void *v);
void vocab_item_free(void *v);

#endif
