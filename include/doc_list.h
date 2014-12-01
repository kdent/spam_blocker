#ifndef DOC_LIST_H
#define DOC_LIST_H

#include <doc_analysis.h>

#define DOC_LIST_SIZE 4

typedef struct _doc_list {
    int cur_size;
    int max_size;
    int cur_item;
    DocAnalysis **list;
} DocList;

DocList *doc_list(int filec, char **file_list);
DocList *doc_list_init();
int doc_list_add(DocList *dlist, DocAnalysis *doc);
DocAnalysis *doc_list_first(DocList *dlist);
DocAnalysis *doc_list_next(DocList *dlist);
void doc_list_free(DocList *dlist);

#endif
