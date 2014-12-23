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
#include <sys/param.h>

#include <doc_list.h>
#include <read_file.h>
#include <vectorize.h>
#include <vocab_list.h>

#define PROGNAME "spam_blocker"
#define SPAM_SUBDIR "spam_samples"
#define EMAIL_SUBDIR "email_samples"
#define INIT_LIST_SZ 1024
#define PATH_SEP "/"

typedef enum {SPAM, EMAIL} Classification;
char *CLASS_LABEL[] = { "spam", "email" };

DocList *load_training_docs(char *dirname);
void analyze_files(Classification class, char *dirname, DocList *dlist);
char *alloc_buf(int sz);

int
main(int argc, char **argv)
{
    char *dirname;
    DocList *doc_features_list = NULL;
    VocabList *vocab_list = NULL;

    /* Assume training only until other features are added. */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <data directory>\n", PROGNAME);
        exit(1);
    }

    /* Load the training files into a list of document features. */
    dirname = argv[1];
    doc_features_list = load_training_docs(dirname);
    if (doc_features_list == NULL || doc_features_list->cur_size == 0) {
        fprintf(stderr, "%s: no training files found or unable to read files in directory: %s\n",
            PROGNAME, dirname);
        exit(1);
    }

    /* Create a vocabulary list from all the words in the corpus. */
    vocab_list = vocab_list_from_docs(doc_features_list);

    vectorize_doc_list(vocab_list, doc_features_list);

/*
{
str_list *words;
char *word;
words = word_list(vocab_list);

for (word = str_list_first(words); word != NULL; word = str_list_next(words)) {
    VocabItem *tok = vocab_list_lookup(vocab_list, word);
    printf("%s\t%d\n", word, tok->count);
}

str_list_free(words);
}
*/

    doc_list_free(doc_features_list);
    vocab_list_free(vocab_list);

    return 0;
}

DocList *
load_training_docs(char *dirname)
{
    DIR *dh;
    struct dirent *dir_entry;
    DocList *dlist;
    char *buf;

    /*
     * Read through the directory looking for spam and email training files.
     */
    dh = opendir(dirname);
    if (dh == NULL) {
        fprintf(stderr, "%s: cannot read %s: %s\n", PROGNAME, dirname,
            strerror(errno));
        exit(-1);
    }

    dlist = doc_list_init();

    while ((dir_entry = readdir(dh)) != NULL) {
        buf = alloc_buf(strlen(dirname) + strlen(PATH_SEP) + strlen(dir_entry->d_name) + 1);
        sprintf(buf, "%s%s%s", dirname, PATH_SEP, dir_entry->d_name);
        if (strcmp(dir_entry->d_name, SPAM_SUBDIR) == 0) {
            analyze_files(SPAM, buf, dlist);
        } else if (strcmp(dir_entry->d_name, EMAIL_SUBDIR) == 0) {
            analyze_files(EMAIL, buf, dlist);
        }
        free(buf);
    }
    closedir(dh);

    return dlist;
}

void
analyze_files(Classification class, char *dirname, DocList *dlist)
{
    DIR *dh;
    struct dirent *file_entry;
    DocFeatures *doc_analysis = NULL;
    char buf[MAXPATHLEN + 1];

    dh = opendir(dirname);
    while ((file_entry = readdir(dh)) != NULL) {
        if (file_entry->d_name[0] == '.')
            continue;
        sprintf(buf, "%s%s%s", dirname, PATH_SEP, file_entry->d_name);
        doc_analysis = read_file(buf);
        if (doc_analysis == NULL) {
             fprintf(stderr, "%s: error in read_file(): %s\n", PROGNAME,
                strerror(errno));
            exit(-1);
        }
        doc_analysis->class = class;
        if (doc_list_add(dlist, doc_analysis) != 0) {
            fprintf(stderr, "%s: error in doc_list_add(): %s\n", PROGNAME,
                strerror(errno));
            exit(-1);
        }
    }
    closedir(dh);
}

/*
 * Allocate a string buffer or die.
 */
char *
alloc_buf(int sz)
{
    char *buf = (char *)malloc(sz);
    if (buf == NULL) {
        fprintf(stderr, "%s: error allocating memory, quiting.\n", PROGNAME);
        exit(-2);
    }
    return buf;
}

