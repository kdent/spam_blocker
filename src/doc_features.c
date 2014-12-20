#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <doc_features.h>
#include <str_list.h>

#define INIT_BUFSIZ 4


/*
 * Perform analysis on a documents's content.
 */

/* Define state machine's alphabet and states. */
typedef enum {
    NON_WORD,
    LWORD,
    UWORD,
    IN_ADDR,
    ERROR
} State;

typedef enum {
    LOWER,
    UPPER,
    DIGIT,
    LEFT_ABRACKET,
    RIGHT_ABRACKET,
    NON_LETTER
} Alphabet;

/* State Transition Table 
 NON_WORD | LWORD | UWORD | IN_ADDR | ERROR */
int transition_tbl[6][5] = {
  {1,        1,       1,      3,       4},    /* LOWER          */
  {2,        1,       2,      3,       4},    /* UPPER          */
  {0,        0,       0,      3,       4},    /* DIGIT          */
  {3,        3,       3,      3,       4},    /* LEFT_ABRACKET  */
  {0,        0,       0,      0,       4},    /* RIGHT_ABRACKET */
  {0,        0,       0,      3,       4}     /* NON_LETTER     */
};

struct token_buf {
    char cur_token[MSG_HDR_LEN];
    int end_ptr;
};

/* Internal function prototypes */
State    state_transition(DocFeatures *doc_features, struct token_buf *buf, Alphabet symbol, State cur_state, char c);
int      push_char(struct token_buf *buf, char c);
int      make_token(DocFeatures *doc_features, struct token_buf *buf);
char     digit_to_char(char c);
Alphabet get_symbol(DocFeatures *doc_features, State current_state, char c);

/*
 *
 * Returns a doc_features structure or NULL if there are memory errors.
 */
DocFeatures *
analyze_doc(char *label, char *doc)
{
    State current_state = NON_WORD;
    Alphabet current_symbol;
    DocFeatures *doc_features;
    struct token_buf buf;
    char c;
    int i;

    if (doc == NULL) doc = "";
    if (label == NULL) label = "";

    /* Initialize the doc_features structure. */
    doc_features = (DocFeatures *)malloc(sizeof(DocFeatures));
    if (doc_features == NULL)
        return NULL;
    doc_features->label = (char *)malloc(strlen(label) + 1);
    if (doc_features->label == NULL)
        return NULL;
    strcpy(doc_features->label, label);
    doc_features->token_list = str_list_init(); 
    doc_features->embedded_digit = 0;

    /* Initialize buffer for building tokens. */
    buf.end_ptr = 0;
    memset(buf.cur_token, 0, MSG_HDR_LEN);

    /* */
    for (i = 0; i < strlen(doc); i++) {
        c = doc[i];
        current_symbol = get_symbol(doc_features, current_state, c);
        current_state = state_transition(doc_features, &buf, current_symbol, current_state, c);
        if (current_state == ERROR)
            return NULL;
    }
    if (current_state == LWORD || current_state == UWORD)
        make_token(doc_features, &buf);

    return doc_features;
}

void
doc_analysis_free(DocFeatures *doc)
{
    str_list_free(doc->token_list);
    free(doc->label);
    free(doc);
}

State
state_transition(DocFeatures *doc_features, struct token_buf *buf,
Alphabet symbol, State cur_state, char c)
{
    /* Get the next state. */
    State next_state = transition_tbl[symbol][cur_state];

    /* Execute functions associated with an arc. */
    switch (cur_state) {
        case NON_WORD:
            if (symbol ==  LOWER) {
                if (push_char(buf, c) != 0)
                    next_state = ERROR;
            } else if (symbol == UPPER) {
                if (push_char(buf, tolower(c)) != 0)
                    next_state = ERROR;
            }
            break;

        case LWORD:
            if (symbol == LOWER) {
                if (push_char(buf, c) != 0)
                    next_state = ERROR;
            } else if (symbol == UPPER) {
                if (make_token(doc_features, buf) != 0)
                    next_state = ERROR;
                doc_features->camel_case = 1;
                if (push_char(buf, tolower(c)) != 0)
                    next_state = ERROR;
            } else if (symbol == DIGIT || symbol == NON_LETTER) {
                if (make_token(doc_features, buf) != 0)
                    next_state = ERROR;
            }
            break;

        case UWORD:
            if (symbol == LOWER) {
                if (push_char(buf, c) != 0)
                    next_state = ERROR;
            } else if (symbol == UPPER) {
                if (push_char(buf, tolower(c)) != 0)
                    next_state = ERROR;
            } else if (symbol == DIGIT || symbol == NON_LETTER) {
                if (make_token(doc_features, buf) != 0)
                    next_state = ERROR;
            }

        case IN_ADDR:
            break;

        default:
            /* Unknown or Error state. */
            break;
    }

    return next_state;
}

int
push_char(struct token_buf *buf, char c)
{
    if (buf->end_ptr < MSG_HDR_LEN) {
        buf->cur_token[buf->end_ptr++] = c;
    }
    return 0;
}

int
make_token(DocFeatures *doc_features, struct token_buf *buf)
{
    char *token = (char *)malloc(buf->end_ptr + 1);
    if (token == NULL)
        return -1;
    memcpy(token, buf->cur_token, buf->end_ptr);
    token[buf->end_ptr] = '\0';
    if (! str_list_add(doc_features->token_list, token))
        return -1;
    free(token);
    buf->end_ptr = 0;
    memset(buf->cur_token, 0, MSG_HDR_LEN);
    return 0;
}

char
digit_to_char(char c)
{
    if (c == '0') {
        c = 'o';
    } else if (c == '1')
        c = 'l';

    return c;
}

Alphabet
get_symbol(DocFeatures *doc_features, State current_state, char c)
{
    Alphabet cur_symbol;

    if (islower(c))
        cur_symbol = LOWER;
    else if (isupper(c))
        cur_symbol = UPPER;
    else if (isdigit(c)) {
        if (current_state == LWORD || current_state == UWORD) {
            digit_to_char(c);
            if (isalpha(c)) {
                if (current_state == UWORD) {
                    c = toupper(c);
                    cur_symbol = UPPER;
                } else {
                    cur_symbol = LOWER;
                }
                doc_features->embedded_digit = 1;
            } else {
                cur_symbol = DIGIT;
            }
        } else {
            cur_symbol = DIGIT;
        }
    } else if (c == '<')
        cur_symbol = LEFT_ABRACKET;
    else if (c == '>')
        cur_symbol = RIGHT_ABRACKET;
    else if (c == '$') {
        if (current_state == LWORD || current_state == UWORD) {
            if (current_state == UWORD) {
                c = 'S';
                cur_symbol = UPPER;
            } else {
                c = 's';
                cur_symbol = LOWER;
            }
            doc_features->embedded_digit = 1; 
        } else {
            cur_symbol = NON_LETTER;
        }
    } else
        cur_symbol = NON_LETTER;

    return cur_symbol;
}

