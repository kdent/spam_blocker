#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <doc_analysis.h>
#include <msg_content.h>
#include <str_list.h>

/*
 * The read_file() function is used to extract the document features from a
 * given file. The file is assumed to be an rfc2822 formatted message file.
 * The function invokes the analyze_doc() function to get a DocAnalysis
 * structure from the file.
 *
 * Returns a DocAnalysis structure. An error can occur if the file cannot be
 * opened for reading or if memory cannot be allocated. If an error occurs, it
 * returns NULL and either error will set errno.
 */
DocAnalysis *
read_file(char *file_name)
{

    char *combined_headers, *str;
    FILE *in;
    MsgContent *msg;
    DocAnalysis *doc;

    in = fopen(file_name, "r");
    if (in == NULL) return NULL;

    msg = msg_content(in);
    combined_headers = (char *)malloc(strlen(msg->subject) + strlen(msg->from_hdr) + 2);
    if (combined_headers == NULL) return NULL;
    combined_headers = strcat(msg->subject, msg->from_hdr);
    doc = analyze_doc(file_name, combined_headers);
    fclose(in);

    printf("%s: ", doc->label);
    for (str = str_list_first(doc->token_list); str != NULL;
    str = str_list_next(doc->token_list))
    {
        printf("[%s] ", str);
    }
    printf("\n");
    free(combined_headers);

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

