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

    dlist = load_doc_list();
    vlist = vocab_list_from_docs(dlist);

printf("no problem to here.\n");
    vect = vectorize_doc_list(vlist, dlist);
printf("past vectorizing\n");

    for (i = 0; i < dlist->cur_size; i++) {
printf("outer loop\n");
        for (j = 0; j < vlist->item_count; j++) {
            printf("%d\t", vect[i][j]);
        }
        puts("");
    }

}

DocList *
load_doc_list()
{
    DocFeatures *doc1;
    DocFeatures *doc2;
    DocList *dlist;

    doc1 = read_file("test/data/msg01.txt");
    doc2 = read_file("test/data/msg02.txt");
    dlist = doc_list_init();
    doc_list_add(dlist, doc1);
    doc_list_add(dlist, doc2);
    return dlist;
}
