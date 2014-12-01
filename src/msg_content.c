#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <msg_content.h>

void chomp(char *str);

const char *FROM = "From:";
const char *SUBJ = "Subject:";

/*
 * Reads from the provided input stream extracting words from the From: and
 * Subject: lines. Assumes that the input stream contains the contents of an
 * RFC2822 email message.
 *
 * Returns a pointer to a msg_content struct. Caller is responsible to free the
 * struct with a call to msg_free().
 */
MsgContent *
msg_content(FILE *in_stream)
{
    char *line = NULL;
    MsgContent *msg = msg_init();
    int from_set = 0, subj_set = 0;

    line = (char *)malloc(MSG_HDR_LEN);

    while (fgets(line, MSG_HDR_LEN, in_stream) != NULL) {

        chomp(line);

        /* Checking From: header */
        if (strncmp(FROM, line, strlen(FROM)) == 0) {
            msg_set_from(msg, &line[strlen(FROM)]);
            from_set = 1;
        }

        /* Checking Subject: header */
        if (strncmp(SUBJ, line, strlen(SUBJ)) == 0) {
            msg_set_subject(msg, &line[strlen(SUBJ)]);
            subj_set = 1;
        }

        if (from_set && subj_set)
            break;

        memset(line, 0, MSG_HDR_LEN);

    }

    free(line);
    return msg;

}

MsgContent *
msg_init()
{
    MsgContent *msg = NULL;
    msg = (MsgContent *)malloc(sizeof(MsgContent));
    if (msg == NULL)
        return NULL;
    msg->subject = (char *)malloc(MSG_HDR_LEN + 1);
    msg->from_hdr = (char *)malloc(MSG_HDR_LEN + 1);
    if (msg->subject == NULL || msg->from_hdr == NULL)
        return NULL;

    msg->subject[0] = '\0';
    msg->from_hdr[0] = '\0';
    return msg;
}

void
msg_free(MsgContent *msg)
{
    free(msg->subject);
    free(msg->from_hdr);
    free(msg);
}

void
msg_set_subject(MsgContent *msg, char *subj)
{
    strncpy(msg->subject, subj, MSG_HDR_LEN); 
}

void
msg_set_from(MsgContent *msg, char *from)
{
    strncpy(msg->from_hdr, from, MSG_HDR_LEN);
}

/*
 * Removes line endings from the end of a string. chomp() alters the provided
 * string modifying it in place. If there are no line endings in the string,
 * it is not modified.
 */
void
chomp(char *str)
{
    for (int i = strlen(str) - 1; i > 0; i++) {
        if (str[i] == '\n' || str[i] == '\r')
            str[i] = '\0';
        else
            break;
    }
}

