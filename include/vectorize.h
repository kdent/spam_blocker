#ifndef VECTORIZE_H
#define VECTORIZE_H

#include <doc_list.h>
#include <vocab_list.h>

int **vectorize_doc_list(VocabList *vlist, DocList *dlist);
void vector_free(int **v, int nrows);

#endif
