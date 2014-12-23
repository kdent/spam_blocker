#include <stdlib.h>
#include <stdio.h>

#include <doc_features.h>
#include <read_file.h>

void print_doc_tokens(DocFeatures *doc);

int
main(int argc, char **argv)
{

   DocFeatures *doc1, *doc2, *doc3;

    doc1 = read_file("test/data/msg01.txt");
    doc2 = read_file("test/data/msg02.txt");
    doc3 = read_file("test/data/msg03.txt");

    print_doc_tokens(doc1);
    print_doc_tokens(doc2);
    print_doc_tokens(doc3);


}

void
print_doc_tokens(DocFeatures *doc)
{
    char *tok;

    for (tok = str_list_first(doc->token_list); tok != NULL;
    tok = str_list_next(doc->token_list))
    {
        printf("[%s] ", tok);
    }
    puts("");
}
