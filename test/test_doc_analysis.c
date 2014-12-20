#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <doc_analysis.h>
#include <str_list.h>

void test_str(char *label, char *str);
void print_tokens(char *label, str_list *tokens);

int
main(int argc, char **argv)
{
    test_str(NULL, NULL);
    test_str("1", "");
    test_str("2", "a");
    test_str("3", "New Xbox Wii PS3 at over 90% off retail?");
    test_str("4", "\"Help Wanted - Winter 2012-2013\" <Help.Wanted..Winter.20122013@marketobtainindividualize.info>");
    test_str("5", "\"Alcohol_Rehab\" <Chad@igingamer.info>");
    test_str("6", "=?windows-1252?Q?Fall_Jobs_are_Here_=96_Apply_Now?=");
    test_str("7", "\"Criminal_JusticeEducation\" <crimjust2012@webs4sell.com>");
    test_str("8", "\"ChristianMingle Dating \" <ChristianMingle.Dating.@allocateprioritizeinform.info>");
    test_str("9", " \"MyDietPatchToday\" <MyDietPatchToday@schnauzerproblems.com>");
    test_str("10", "Don`t become another Bed Bug victim!");
    test_str("11", "Eliminate L0$$ES!!");
    test_str("12", "=?UTF-8?B?V2ViY2FzdDogQXZvaWQgUENJIGNvbXBsaWFuY2UgYW5kIHNjb3BlIHJlZHVjdGlvbiBzb2x1dGlvbnMgdGhhdCBzaW1wbHkgd29u4oCZdCBzY2FsZSB3aXRoIHlvdXIgYnVzaW5lc3M=?=");
    test_str("13", "No final punctuation");
    test_str("14", ".");

}

void
test_str(char *label, char *str) {
    struct doc_analysis *doc_f;

    doc_f = extract_doc_features(label, str);
    if (doc_f == NULL) {
        printf("ERROR: extract_doc_features() returned an error\n");
    } else {
        print_tokens(doc_f->label, doc_f->token_list);
    }
    doc_analysis_free(doc_f);
}

void
print_tokens(char *label, str_list *tokens) {
    printf("%s: ", label);
    for (char *tok = str_list_first(tokens); tok != NULL; tok = str_list_next(tokens)) {
        printf("[%s] ", tok);
    }
    puts("");
}
