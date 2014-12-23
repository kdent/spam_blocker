#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
        v[doc_count] = (int *)malloc(vlist->item_count * sizeof(int));
        memset(v[doc_count], 0, vlist->item_count);
        for (tok = str_list_first(doc_features->token_list); tok != NULL;
        tok = str_list_next(doc_features->token_list))
        {
            vocab_item = vocab_list_lookup(vlist, tok);
            v[doc_count][vocab_item->index] = 1;
        }
        doc_count++;
    }

    return v;
}
