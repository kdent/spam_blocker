#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <doc_features.h>
#include <str_list.h>

#define INIT_BUFSIZ 4


/*
 * Perform analysis on a documents's content. A document is a string of words
 * that have been extracted from an email message From: and Subject: header
 * lines. First the code tokenizes the document string into a list of tokens 
 * and determines features for the document. Features are stored in a struct
 * that contains the following attributes:
 *   char *label;               // readable name for the class spam or not spam
 *   int class;                 // integer value representing the class
 *   str_list *token_list;      // list of tokens from the document
 *   int embedded_digit;        // flag set to 1 if tokens contain a digit in place of a letter
 *   int camel_case;            // flag set to 1 if tokens use camel case instead of normal word spacing
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
static State state_transition(DocFeatures *doc_features, struct token_buf *buf, Alphabet symbol, State cur_state, char c);
static int push_char(struct token_buf *buf, char c);
static int make_token(DocFeatures *doc_features, struct token_buf *buf);
static char digit_to_char(char c);
static Alphabet get_symbol(char c, State current_state, DocFeatures *doc_features);

/*
 * Returns a doc_features structure or NULL if there are memory errors.
 */
DocFeatures *
extract_doc_features(char *label, char *doc)
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

    /*
     * Process document using a finite-state transducer. The transducer accepts
     * the entire document as a string and outputs a list of tokens. The
     * transducer additionally converts all characters to lower case, embedded
     * digits to the letters they represent (e.g. "l0ve -> love"), and
     * tokenizes camel-case words into separate tokens (e.g. MakeMoneyFast ->
     * [make, money, fast]). The list of tokens and other features are stored
     * in * the doc_features structure during processing.
     */
    for (i = 0; i < strlen(doc); i++) {
        c = doc[i];
        current_symbol = get_symbol(c, current_state, doc_features);
        current_state = state_transition(doc_features, &buf, current_symbol, current_state, c);
        if (current_state == ERROR)
            return NULL;
    }

    /* If processing finishes on a word, tokenize it. */
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

static State
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

static int
push_char(struct token_buf *buf, char c)
{
    if (buf->end_ptr < MSG_HDR_LEN) {
        buf->cur_token[buf->end_ptr++] = c;
    }
    return 0;
}

static int
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

static char
digit_to_char(char c)
{
    if (c == '0') {
        c = 'o';
    } else if (c == '1')
        c = 'l';

    return c;
}

/*
 * Converts an input character to a symbol in the Alphabet. How the character
 * should be interpreted can depend on the current_state. Some of the extracted
 * features of a document come from the underlying character before it is
 * converted to a symbol (such as digits embedded in a word). The doc_features
 * structure will be updated to include features extracted at this point.
 *
 * get_symbol() always returns an Alphabet symbol.
 */
static Alphabet
get_symbol(char c, State current_state, DocFeatures *doc_features)
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

