#include <stdlib.h>
#include <string.h>

#include <doc_analysis.h>
#include <doc_list.h>
#include <read_file.h>

/*
 * The doc_list() function calls read_file() for each file given in
 * file_list. The parameter filec is the number of files in file_list.
 *
 * Returns the generated list or NULL if it encounters any errors. If an error
 * occurs errno will be set by the system call that originated it.
 */
DocList *
doc_list(int filec, char **file_list)
{
    DocList *doc_list = NULL;
    DocAnalysis *doc_analysis = NULL;

    doc_list = doc_list_init();
    for (int i = 0; i < filec; i++) {
        doc_analysis = read_file(file_list[i]);
        if (doc_analysis == NULL) return NULL;
        if (doc_list_add(doc_list, doc_analysis) != 0) {
            return NULL;
        }
    }
    return doc_list;
}

DocList *
doc_list_init()
{
    DocList *dlist = NULL;
    dlist = (DocList *)malloc(sizeof(DocList));
    dlist->max_size = DOC_LIST_SIZE;
    dlist->cur_size = 0;
    dlist->cur_item = 0;
    dlist->list = (DocAnalysis **)malloc(sizeof(DocAnalysis) * dlist->max_size);
    memset(dlist->list, 0, sizeof(DocAnalysis) * dlist->max_size);
    return dlist;
}

int
doc_list_add(DocList *dlist, DocAnalysis *doc)
{

    if (dlist->cur_size >= dlist->max_size) {
        DocAnalysis **new_list = realloc(dlist->list, dlist->max_size * 2);
        if (new_list == NULL)
            return -1;

        dlist->list = new_list;
        dlist->max_size *= 2;
    }
    dlist->list[dlist->cur_size++] = doc;
    return 0;
}

DocAnalysis *
doc_list_first(DocList *dlist)
{
    dlist->cur_item = 1;
    return dlist->list[0];
}

DocAnalysis *
doc_list_next(DocList *dlist)
{
    if (dlist->cur_item > dlist->cur_size)
        return NULL;
    else
        return dlist->list[dlist->cur_item++];
}

void
doc_list_free(DocList *dlist)
{
    for (int i = 0; i < dlist->cur_size; i++) {
        doc_analysis_free(dlist->list[i]);
    }
    free(dlist->list);
    free(dlist);
}

