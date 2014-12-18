/*
 * When training Looks for directories called spam_samples and email_samples
 * for the data directory given on the command line.
 *
 */
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <doc_list.h>
#include <vocab_list.h>

#define PROG_NAME "spam_blocker"
#define SPAM_SUBDIR "spam_samples"
#define EMAIL_SUBDIR "email_samples"
#define INIT_LIST_SZ 1024
#define PATH_SEP "/"

DocList *load_training_docs(char *dirname);
str_list *get_file_names(char *dirname);
char *alloc_buf(int sz);

enum {SPAM, EMAIL};
char *CLASS_LABEL[] = { "spam", "email" };

int
main(int argc, char **argv)
{
    char *dirname;
    DocList *doc_features_list = NULL;
    VocabList *vocab_list = NULL;

    /* Assume training only until other features are added. */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <data directory>\n", PROG_NAME);
        exit(1);
    }

    /* Load the training files into a list of document features. */
    dirname = argv[1];
    doc_features_list = load_training_docs(dirname);
    if (doc_features_list == NULL || doc_features_list->cur_size == 0) {
        fprintf(stderr, "%s: no training files found or unable to read files in directory: %s\n",
            PROG_NAME, dirname);
        exit(1);
    }

    /* Create a vocabulary list from all the words in the corpus. */
    vocab_list = vocab_list_from_docs(doc_features_list);

{
char **words;
char *word;
int i;
words = word_list(vocab_list);
i = 0;
word = words[i];
/*
while (word != NULL) {
    VocabItem *tok = vocab_list_lookup(vocab_list, word);
    printf("%s\t%d\n", word, tok->count);
    word = words[++i];
}
*/
word_list_free(i, words);
}

    doc_list_free(doc_features_list, FREE_DOCS);
    vocab_list_free(vocab_list);

    return 0;
}

DocList *
load_training_docs(char *dirname)
{
    DIR *dh;
    struct dirent *dir_entry;
    str_list *spam_files, *email_files;
    DocList *spam_dlist, *dlist;
    DocFeatures *doc_index;
    char *buf;

    /*
     * Read through the directory looking for spam and email training files.
     */
    dh = opendir(dirname);
    if (dh == NULL) {
        fprintf(stderr, "%s: cannot read %s: %s\n", PROG_NAME, dirname,
            strerror(errno));
        exit(-1);
    }

    while ((dir_entry = readdir(dh)) != NULL) {
        buf = alloc_buf(strlen(dirname) + strlen(PATH_SEP) + strlen(dir_entry->d_name) + 1);
        sprintf(buf, "%s%s%s", dirname, PATH_SEP, dir_entry->d_name);
        if (strcmp(dir_entry->d_name, SPAM_SUBDIR) == 0) {
            spam_files = get_file_names(buf);
        } else if (strcmp(dir_entry->d_name, EMAIL_SUBDIR) == 0) {
            email_files = get_file_names(buf);
        }
        free(buf);
    }
    closedir(dh);

    /*
     * Combine spam document features and email document features into a
     * a single document list.
     */
    spam_dlist = doc_list_from_files(SPAM, spam_files);
    dlist = doc_list_from_files(EMAIL, email_files);
    for (doc_index = doc_list_first(spam_dlist); doc_index != NULL;
    doc_index = doc_list_next(spam_dlist))
    {
        doc_list_add(dlist, doc_index);
    }

    str_list_free(email_files);
    str_list_free(spam_files);
    doc_list_free(spam_dlist, DONT_FREE_DOCS);
 
    return dlist;
}

str_list *
get_file_names(char *dirname)
{
    DIR *dh;
    struct dirent *file_entry;
    str_list *list;
    char *buf;

    list = str_list_init();

    dh = opendir(dirname);
    while ((file_entry = readdir(dh)) != NULL) {
        if (file_entry->d_name[0] == '.')
            continue;
        buf = alloc_buf(strlen(dirname) + strlen(PATH_SEP) + strlen(file_entry->d_name) + 1);
        sprintf(buf, "%s%s%s", dirname, PATH_SEP, file_entry->d_name);
        str_list_add(list, buf);
        free(buf);
    }
    closedir(dh);
    free(file_entry);
    return list;
}

/*
 * Allocate a string buffer or die.
 */
char *
alloc_buf(int sz)
{
    char *buf = (char *)malloc(sz);
    if (buf == NULL) {
        fprintf(stderr, "%s: error allocating memory, quiting.\n", PROG_NAME);
        exit(-2);
    }
    return buf;
}

