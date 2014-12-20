#ifndef DOC_LIST_H
#define DOC_LIST_H

#include <doc_features.h>

#define DOC_LIST_SIZE 4

typedef struct _doc_list {
    int cur_size;
    int max_size;
    int cur_item;
    DocFeatures **list;
} DocList;

DocList *doc_list_from_files(int class_label, str_list *file_list);
DocList *doc_list_init();
int doc_list_add(DocList *dlist, DocFeatures *doc);
DocFeatures *doc_list_first(DocList *dlist);
DocFeatures *doc_list_next(DocList *dlist);
void doc_list_free(DocList *dlist);

#endif
