#ifndef MAKE_VOCAB_LIST_H
#define MAKE_VOCAB_LIST_H

#include <vocab_item.h>

typedef struct _vocab_list {
    int tbl_size;       /* the size of the hash table */
    int size;           /* the number of words in the table */
    int cur_item;       /* current item when iterating the list */
    VocabItem **table;  /* the hash table */
} VocabList;

VocabList *vocab_list_init();
str_list *word_list(VocabList *);
void word_list_free(int, char **);
VocabList *vocab_list_from_docs(DocList *doc_list);
void vocab_list_free(VocabList *vlist);
size_t vocab_list_sizeof(VocabList *vlist);
int vocab_list_insert(VocabList *vlist, char *word);
VocabItem *vocab_list_lookup(VocabList *vlist, char *word);
int hash_vocab_word(const void *word);
VocabItem *vocab_list_first(VocabList *vlist);
VocabItem *vocab_list_next(VocabList *vlist);

#endif
