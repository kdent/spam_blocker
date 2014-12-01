#ifndef MSG_CONTENT_H
#define MSG_CONTENT_H

#include <stdio.h>

#define MSG_HDR_LEN 1024

typedef struct _msg_content {
    char *subject;
    char *from_hdr;
} MsgContent;

MsgContent *msg_content(FILE *in_stream);
MsgContent *msg_init(void);
void msg_free(MsgContent *msg);
void msg_set_subject(MsgContent *msg, char *subj);
void msg_set_from(MsgContent *msg, char *from);

#endif
