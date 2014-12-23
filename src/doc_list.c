#include <stdlib.h>
#include <string.h>

#include <doc_features.h>
#include <doc_list.h>
#include <read_file.h>

/*
 * Initialize a new document list of documents and their features.
 */
DocList *
doc_list_init()
{
    DocList *dlist = NULL;
    dlist = (DocList *)malloc(sizeof(DocList));
    dlist->max_size = DOC_LIST_SIZE;
    dlist->size = 0;
    dlist->cur_item = 0;
    dlist->list = (DocFeatures **)malloc(sizeof(DocFeatures *) * dlist->max_size);
    memset(dlist->list, 0, sizeof(DocFeatures *) * dlist->max_size);
    return dlist;
}

/*
 * The doc_list_from_files() function calls read_file() for each file given in
 * file_list. Files should be rfc2822-formatted email messages. The read_file()
 * function will extract features to produce an DocFeatures structure for
 * each document. The label is the classification label to assign to all of
 * the documents in the file_list.
 *
 * Returns the generated list of DocFeatures structures or NULL if it
 * encounters any errors. If an error occurs errno will be set by the system
 * call that originated it.
 */
DocList *
doc_list_from_files(int class, str_list *file_list)
{
    DocList *dlist = NULL;
    DocFeatures *doc_analysis = NULL;
    char *filename;

    dlist = doc_list_init();
    for (filename = str_list_first(file_list); filename != NULL;
    filename = str_list_next(file_list))
    {
        doc_analysis = read_file(filename);
        if (doc_analysis == NULL) return NULL;
        doc_analysis->class = class;
        if (doc_list_add(dlist, doc_analysis) != 0) {
            return NULL;
        }
    }
    return dlist;
}

int
doc_list_add(DocList *dlist, DocFeatures *doc)
{

    if (dlist->size >= dlist->max_size) {
        DocFeatures **new_list = NULL;
        new_list = realloc(dlist->list, (sizeof(DocFeatures *) * dlist->max_size) * 2);
        if (new_list == NULL)
            return -1;

        /* Zero-fill the new portion of the memory. */
        memset(new_list + dlist->max_size, 0, dlist->max_size);

        dlist->list = new_list;
        dlist->max_size *= 2;
    }
    dlist->list[dlist->size++] = doc;
    return 0;
}

DocFeatures *
doc_list_first(DocList *dlist)
{
    dlist->cur_item = 1;
    return dlist->list[0];
}

DocFeatures *
doc_list_next(DocList *dlist)
{
    if (dlist->cur_item > dlist->size)
        return NULL;
    else
        return dlist->list[dlist->cur_item++];
}

void
doc_list_free(DocList *dlist)
{
    int i;
    for (i = 0; i < dlist->size; i++) {
        doc_analysis_free(dlist->list[i]);
    }
    free(dlist->list);
    free(dlist);
}

