#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <doc_features.h>
#include <doc_list.h>
#include <read_file.h>
#include <str_list.h>
#include <vocab_list.h>
#include <vectorize.h>

DocList *load_doc_list();

int
main(int argc, char **argv)
{
    VocabList *vlist;
    DocList *dlist;
    int **vect, i, j;
    str_list *words;
    int first;
    char *word;

    dlist = load_doc_list();
    vlist = vocab_list_from_docs(dlist);

    vect = vectorize_doc_list(vlist, dlist);

    words = word_list(vlist);
    first = 1;
    for (word = str_list_first(words); word != NULL;
    word = str_list_next(words))
    {
        if (first) {
            printf("%s", word);
            first = 0;
        } else {
            printf(",%s", word);
        }
    }
    printf(",%s,%s", "EMBEDDED", "CAMEL_CASE");
    puts("");

    for (i = 0; i < dlist->size; i++) {
        first = 1;
        for (j = 0; j < vlist->size + 2; j++) {
            if (first) {
                printf("%d", vect[i][j]);
                first = 0;
            } else {
                printf(",%d", vect[i][j]);
            }
        }
        puts("");
    }

    vector_free(vect, dlist->size);
    doc_list_free(dlist);
    str_list_free(words);
    vocab_list_free(vlist);

}

DocList *
load_doc_list()
{
    DocFeatures *doc1, *doc2, *doc3;
    DocList *dlist;

    doc1 = read_file("test/data/msg01.txt");
    doc2 = read_file("test/data/msg02.txt");
    doc2->camel_case = 1;
    doc3 = read_file("test/data/msg03.txt");
    doc3->embedded_digit = 1;
    dlist = doc_list_init();
    doc_list_add(dlist, doc1);
    doc_list_add(dlist, doc2);
    doc_list_add(dlist, doc3);
    return dlist;
}
