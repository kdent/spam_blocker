#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <doc_analysis.h>
#include <doc_list.h>
#include <vocab_list.h>

#include "testing.h"


int
main(int argc, char **argv)
{
    DocAnalysis *doc_analysis1 = NULL;
    DocAnalysis *doc_analysis2 = NULL;
    DocList *doc_list = NULL;
    VocabList *vlist = NULL;
    VocabItem *item = NULL;
    char **wlist = NULL;

    doc_list = doc_list_init();
    doc_analysis1 = (DocAnalysis *)malloc(sizeof(DocAnalysis));
    memset(doc_analysis1, 0, sizeof(DocAnalysis));
    doc_analysis1->token_list = str_list_init();
    doc_list_add(doc_list, doc_analysis1);
    str_list_add(doc_analysis1->token_list, "first");
    str_list_add(doc_analysis1->token_list, "first");
    str_list_add(doc_analysis1->token_list, "second");

    doc_analysis2 = (DocAnalysis *)malloc(sizeof(DocAnalysis));
    memset(doc_analysis2, 0, sizeof(DocAnalysis));
    doc_analysis2->token_list = str_list_init();
    doc_list_add(doc_list, doc_analysis2);
    str_list_add(doc_analysis2->token_list, "third");
    str_list_add(doc_analysis2->token_list, "fourth");
    str_list_add(doc_analysis2->token_list, "fourth");

    vlist = vocab_list(doc_list);
    assertNotNull(vlist);
    item = vocab_list_lookup(vlist, "first");
    assertNotNull(item);
    assertStrEq("first", item->word);
    assertIntEq(2, item->count);
    item = vocab_list_lookup(vlist, "second");
    assertStrEq("second", item->word);
    assertIntEq(1, item->count);
    item = vocab_list_lookup(vlist, "third");
    assertNotNull(item);
    assertStrEq("third", item->word);
    item = vocab_list_lookup(vlist, "fourth");
    assertStrEq("fourth", item->word);
    assertIntEq(2, item->count);
    item = vocab_list_lookup(vlist, "nonexistentword");
    assertNull(item);
    item = vocab_list_lookup(vlist, NULL);
    assertNull(item);
    item = vocab_list_lookup(NULL, "word");
    assertNull(item);

    wlist = word_list(vlist);
    for (int i = 0; i < vlist->item_count; i++) {
        if (strcmp(wlist[i], "first") == 0) {
            assertStrEq("first", wlist[i]);
        } else if (strcmp(wlist[i], "second") == 0) {
            assertStrEq("second", wlist[i]);
        } else if (strcmp(wlist[i], "third") == 0) {
            assertStrEq("third", wlist[i]);
        } else if (strcmp(wlist[i], "fourth") == 0) {
            assertStrEq("fourth", wlist[i]);
        } else {
            assertFail("Unexpected value");
        }
    }
    word_list_free(vlist->item_count, wlist);

    vocab_list_free(vlist);

    vlist = vocab_list(NULL);
    assertNull(vlist);

    doc_list_free(doc_list);

    if (get_fail_count() > 0) {
        printf("%d out of %d tests failed\n", get_fail_count(), get_test_count());
    } else {
        printf("all tests passed\n");
    }
}

