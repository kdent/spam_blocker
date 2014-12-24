#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <doc_features.h>
#include <doc_list.h>
#include <str_list.h>
#include <vectorize.h>
#include <vocab_list.h>

/*
 * Converts the features in each document in dlist into a list of vectors where
 * each document vector is a row in a matrix of documents.  The pointer to the
 * vector should be freed by calling vector_free().
 *
 * If successful, vectorize_doc_list returns a pointer to a 2-dimensional
 * array. If memory cannot be allocated, it returns NULL.
 */
int **
vectorize_doc_list(VocabList *vlist, DocList *dlist)
{
    DocFeatures *doc_features;
    char *tok;
    int i;
    int n_features, embedded_digit_col, camel_case_col;
    VocabItem *vocab_item;
    int **v = NULL;

    v = (int **)malloc(dlist->size * sizeof(int *));
    if (v == NULL) return NULL;

    n_features = vlist->size + 2;  /* Two features in addition to the words. */
    embedded_digit_col = vlist->size;
    camel_case_col = vlist->size + 1;

    i = 0;
    for (doc_features = doc_list_first(dlist); doc_features != NULL;
    doc_features = doc_list_next(dlist))
    {
        /* Allocate space for this document's vector. */
        v[i] = (int *)malloc(n_features * sizeof(int));
        if (v[i] == NULL) return NULL;
        memset(v[i], 0, n_features);

        /* Mark each word that exists in the vector. */
        for (tok = str_list_first(doc_features->token_list); tok != NULL;
        tok = str_list_next(doc_features->token_list))
        {
            vocab_item = vocab_list_lookup(vlist, tok);
            v[i][vocab_item->index] = 1;
        }

        /* Assign extra features if they exist. */
        if (doc_features->embedded_digit)
            v[i][embedded_digit_col] = 1;
        if (doc_features->camel_case)
            v[i][camel_case_col] = 1;

        i++;
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
