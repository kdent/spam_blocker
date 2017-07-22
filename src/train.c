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


#include <stdlib.h>
#include <stdio.h>

#include <doc_features.h>
#include <read_file.h>

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
    for (i = 1; i < argc; i++) {
        doc_features = read_file(argv[i]);
        if (doc_features == NULL) {
            fprintf(stderr, "unable to read file %s\n", argv[i]);
            continue;
        }
        for (tok = str_list_first(doc_features->token_list);
             tok != NULL;
             tok = str_list_next(doc_features->token_list))
        {
            printf("%s\n", tok);
        }
    }


}
