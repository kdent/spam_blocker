#ifndef MAKE_VOCAB_LIST_H
#define MAKE_VOCAB_LIST_H

typedef struct _vocab_item {
    char *word;         /* the word */
    int count;          /* the number of times it occurs */
    int index;          /* this word's index in the list */
    struct _vocab_item *next;
} VocabItem;

typedef struct _vocab_list {
    int tbl_size;
    int slot_count;
    int item_count;
    VocabItem **table;
} VocabList;

char **word_list(VocabList *);
void word_list_free(int, char **);
VocabList *vocab_list_from_docs(DocList *doc_list);
void vocab_list_free(VocabList *vlist);
int vocab_list_insert(VocabList *vlist, char *word);
VocabItem *vocab_list_lookup(VocabList *vlist, char *word);
int hash_vocab_word(const void *word);

#endif
