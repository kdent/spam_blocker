#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <doc_features.h>
#include <msg_content.h>
#include <str_list.h>

/*
 * The read_file() function is used to extract the document features from a
 * given file. The file is assumed to be an rfc2822 formatted message file.
 * The function invokes the extract_doc_features() function to get a
 * DocFeatures structure from the file.
 *
 * Returns a DocFeatures structure. An error can occur if the file cannot be
 * opened for reading or if memory cannot be allocated. If an error occurs, it
 * returns NULL and either error will set errno.
 */
DocFeatures *
read_file(char *file_name)
{

    char *combined_headers;
    FILE *in;
    MsgContent *msg;
    DocFeatures *doc;

    in = fopen(file_name, "r");
    if (in == NULL) return NULL;

    msg = msg_content(in);
    combined_headers = (char *)malloc(strlen(msg->subject) + strlen(msg->from_hdr) + 2);
    if (combined_headers == NULL) return NULL;
    sprintf(combined_headers, "%s %s", msg->subject, msg->from_hdr);
    doc = extract_doc_features(file_name, combined_headers);
    fclose(in);

    free(combined_headers);
    msg_free(msg);

    return doc;
}

#ifdef TEST
int
main(int argc, char **argv)
{

    if (argc < 2) {
        printf("usage: %s <file1> [<file2> ...]\n", argv[0]);
        exit(1);
    }

    printf("reading %d files\n", argc);
    for (int i = 1; i < argc; i++) {
        read_file(argv[i]);
    }

}
#endif

