/**
 * NAME
 *    train
 *
 * SUMMARY
 *    Analyzes training corpus to build a classification model
 *
 * SYNOPSIS
 * 
 *    train file1 [file2 ...]
 *
 * DESCRIPTION
 *    Reads the list of files given on the command line. Files should be in
 *    the form of RFC 822 message files. The program reads the From: and 
 *    Subject: headers from the message and builds a word list for each token
 *    found in those headers.
 *
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <doc_features.h>
#include <doc_list.h>
#include <hashtbl.h>
#include <read_file.h>
#include <str_list.h>
#include <vocab_item.h>

int
main(int argc, char **argv)
{

    if (argc < 2) {
        fprintf(stderr, "usage: %s file1 [file2 ...]\n", argv[0]);
        exit(1);
    }

    int i;
    char *tok;
    DocFeatures *doc_features;
    HASHTbl *vocab_list = (HASHTbl *)malloc(sizeof(HASHTbl));

    if (hashtbl_init(vocab_list, VOCAB_ITEM_TBL_SIZE, vocab_item_hash,
                vocab_item_match, vocab_item_free) != 0) {
        fprintf(stderr, "error initializing vocabulary list\n");
        exit(-1);
    }

    /* For each specificed document, extract its features. */
    for (i = 1; i < argc; i++) {

        doc_features = read_file(argv[i]);
        if (doc_features == NULL) {
            fprintf(stderr, "unable to read file %s\n", argv[i]);
            continue;
        }

        /* Add each token to the overall vocabulary list. */
        int j = 0;
        for (tok = str_list_first(doc_features->token_list);
             tok != NULL;
             tok = str_list_next(doc_features->token_list))
        {
            VocabItem *vocab_item = vocab_item_init(tok, ++j);
            hashtbl_insert(vocab_list, vocab_item);
        }

        doc_analysis_free(doc_features);
    }

    /* Go through the list of all the vocab items. */
/*
    VocabItem *vitem = vocab_list_first(vocab_list);
    for (; vitem != NULL; vitem = vocab_list_next(vocab_list)) {
        printf("** %s:%d ** \n", vitem->word, vitem->count);
    }
*/

    int fd = open("vocab_list.dat", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("train: can't open vocab_list.dat");
        exit(-1);
    }
    write(fd, vocab_list, hashtbl_size(vocab_list));
    close(fd);


    hashtbl_destroy(vocab_list);

}
