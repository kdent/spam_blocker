#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <doc_features.h>
#include <doc_list.h>
#include <str_list.h>
#include <vectorize.h>
#include <vocab_list.h>

/*
 * Projects words from a list of documents into a vector. The pointer to the
 * vector should be freed by calling vector_free().
 *
 * If successful, returns a pointer to a 2-dimensional array. If memory
 * cannot be allocated, it returns NULL.
 */
int **
vectorize_doc_list(VocabList *vlist, DocList *dlist)
{
    DocFeatures *doc_features;
    char *tok;
    int doc_count = 0;
    VocabItem *vocab_item;
    int **v = (int **)malloc(dlist->size * sizeof(int *));
    if (v == NULL) return NULL;

    for (doc_features = doc_list_first(dlist); doc_features != NULL;
    doc_features = doc_list_next(dlist))
    {
        v[doc_count] = (int *)malloc(vlist->size * sizeof(int));
        if (v[doc_count] == NULL) return NULL;
        memset(v[doc_count], 0, vlist->size);
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

void
vector_free(int **v, int nrows)
{
    int i;
    for (i = 0; i < nrows; i++) {
        free(v[i]);
    }
    free(v);
}
