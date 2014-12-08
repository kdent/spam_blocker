#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <doc_list.h>

#define PROG_NAME "spam_blocker"
#define SPAM_SUBDIR "spam_samples"
#define EMAIL_SUBDIR "email_samples"
#define INIT_LIST_SZ 1024
#define PATH_SEP "/"

DocList *load_training_docs(char *dirname);
str_list *get_file_names(char *dirname);
char *alloc_buf(int sz);


int
main(int argc, char **argv)
{
    char *dirname;
    DocList *dlist = NULL;

    /* Assume training only until other features are added. */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <data directory>\n", PROG_NAME);
        exit(1);
    }

    dirname = argv[1];
    dlist = load_training_docs(dirname);
    if (dlist == NULL) {
        fprintf(stderr, "%s: no training files found or unable to read files in directory: %s\n",
            PROG_NAME, dirname);
        exit(1);
    }

{
DocAnalysis *doc = NULL;
printf("got %d documents\n", dlist->cur_size);
for (doc = doc_list_first(dlist); doc != NULL; doc = doc_list_next(dlist)) {
    printf("%s: ", doc->label);
    for (char *str = str_list_first(doc->token_list); str != NULL;
    str = str_list_next(doc->token_list))
    {
        printf("[%s] ", str);
    }
    printf("\n");
}
}


}

DocList *
load_training_docs(char *dirname)
{
    DIR *dh;
    struct dirent *dir_entry;
    str_list *spam_files = NULL, *email_files;
    DocList *dlist = NULL;
    char *buf;

    /* Allocate memory for various things. */
    dir_entry = (struct dirent *)malloc(sizeof(struct dirent));
    if (dir_entry == NULL) {
        fprintf(stderr, "%s: error allocating memory, quiting.\n", PROG_NAME);
        exit(-1);
    }

    /* Read through the directory for spam and email training files. */
    dh = opendir(dirname);
    if (dh == NULL) {
        fprintf(stderr, "%s: cannot read %s: %s\n", PROG_NAME, dirname,
            strerror(errno));
        exit(-1);
    }

    while ((dir_entry = readdir(dh)) != NULL) {
        buf = alloc_buf(strlen(dirname) + strlen(PATH_SEP) + strlen(dir_entry->d_name) + 1);
        sprintf(buf, "%s%s%s", dirname, PATH_SEP, dir_entry->d_name);
        if (strcmp(basename(buf), SPAM_SUBDIR) == 0) {
            spam_files = get_file_names(buf);
        }
    }
    closedir(dh);

    dlist = doc_list(spam_files);

    return dlist;
}

str_list *
get_file_names(char *dirname)
{
    DIR *dh;
    struct dirent *file_entry;
    str_list *list;
    char *buf;

    file_entry = (struct dirent *)malloc(sizeof(struct dirent));
    list = str_list_init();

    dh = opendir(dirname);
    while ((file_entry = readdir(dh)) != NULL) {
        if (file_entry->d_name[0] == '.')
            continue;
        buf = alloc_buf(strlen(dirname) + strlen(PATH_SEP) + strlen(file_entry->d_name) + 1);
        sprintf(buf, "%s%s%s", dirname, PATH_SEP, file_entry->d_name);
        str_list_add(list, buf);
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

