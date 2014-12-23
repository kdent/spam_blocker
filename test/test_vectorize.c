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
    char *word;

    dlist = load_doc_list();
    vlist = vocab_list_from_docs(dlist);

    vect = vectorize_doc_list(vlist, dlist);

    words = word_list(vlist);
    for (word = str_list_first(words); word != NULL; word = str_list_next(words))
    {
        printf("%s\t", word);
    }
    puts("");

    for (i = 0; i < dlist->size; i++) {
        for (j = 0; j < vlist->size + 2; j++) {
            printf("%d\t", vect[i][j]);
        }
        puts("");
    }

}

DocList *
load_doc_list()
{
    DocFeatures *doc1, *doc2, *doc3;
    DocList *dlist;

    doc1 = read_file("test/data/msg01.txt");
    doc2 = read_file("test/data/msg02.txt");
    doc3 = read_file("test/data/msg03.txt");
    doc3->embedded_digit = 1;
    dlist = doc_list_init();
    doc_list_add(dlist, doc1);
    doc_list_add(dlist, doc2);
    doc_list_add(dlist, doc3);
    return dlist;
}
