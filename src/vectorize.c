#include <stdlib.h>
#include <stdio.h>

#include <doc_features.h>
#include <doc_list.h>
#include <str_list.h>
#include <vectorize.h>
#include <vocab_list.h>

int **
vectorize_doc_list(VocabList *vlist, DocList *dlist)
{
    DocFeatures *doc_features;
    char *tok;
    int doc_count = 0;
    VocabItem *vocab_item;
    int **v = (int **)malloc(dlist->cur_size * sizeof(int *));
    if (v == NULL) return NULL;

    for (doc_features = doc_list_first(dlist); doc_features != NULL;
    doc_features = doc_list_next(dlist))
    {
        v[doc_count] = (int *)calloc(dlist->cur_size, sizeof(int *));
        for (tok = str_list_first(doc_features->token_list); tok != NULL;
        tok = str_list_next(doc_features->token_list))
        {
            v[doc_count] = (int *)calloc(vlist->item_count, sizeof(int));
            vocab_item = vocab_list_lookup(vlist, tok);
            v[doc_count++][vocab_item->index] = 1;
        }
    }

    return v;
}
