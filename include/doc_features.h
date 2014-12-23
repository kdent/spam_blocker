#ifndef DOC_FEATURES_H
#define DOC_FEATURES_H

#include <str_list.h>
#include <msg_content.h>

typedef struct _doc_analysis {
    char *label;
    int class;
    str_list *token_list;
    int embedded_digit;
    int camel_case;
} DocFeatures;

DocFeatures *extract_doc_features(char *label, char *document);
void doc_analysis_free(DocFeatures *doc);

#endif
