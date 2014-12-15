#include <stdlib.h>
#include <stdio.h>

#include <doc_list.h>

int
main(int argc, char **argv)
{
    DocList *dlist = NULL;
    DocFeatures doc1;
    DocFeatures doc2;
    DocFeatures doc3;
    DocFeatures doc4;
    DocFeatures *retrieved = NULL;

    doc1.label = "spam 1";
    doc1.class = 1;
    doc2.label = "spam 2";
    doc2.class = 1;
    doc3.label = "ham 1";
    doc3.class = 0;
    doc4.label = "ham 2";
    doc4.class = 0;

    dlist = doc_list_init();
    doc_list_add(dlist, &doc1);
    doc_list_add(dlist, &doc2);
    doc_list_add(dlist, &doc3);
    doc_list_add(dlist, &doc4);

    for (retrieved = doc_list_first(dlist); retrieved != NULL; retrieved = doc_list_next(dlist))
    {
        printf("doc: %s class: %d\n", retrieved->label, retrieved->class);
    }

    doc_list_free(dlist, 0);

}
