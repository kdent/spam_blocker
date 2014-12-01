#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <msg_content.h>

int
main(int argc, char **argv)
{
    FILE *msg_fh = NULL;
    MsgContent *msg = NULL;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <msg_file1> [<msg_file2> ...]\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        msg_fh = fopen(argv[i], "r");
        if (msg_fh == NULL) {
            fprintf(stderr, "can't open %s: %s\n", argv[1], strerror(errno));
            continue;
        }
        msg = msg_content(msg_fh);
        printf("%s %s\n", msg->from_hdr, msg->subject);
        fclose(msg_fh);
        msg_free(msg);
    }

}
