#ifndef DOC_ANALYSIS_H
#define DOC_ANALYSIS_H

#include <str_list.h>
#include <msg_content.h>

typedef struct _doc_analysis {
    char *label;
    int class;
    str_list *token_list;
    int embedded_digit;
    int camel_case;
} DocAnalysis;

DocAnalysis *analyze_doc(char *label, char *document);
void doc_analysis_free(DocAnalysis *doc);

#endif
