CC = cc
OBJS = src/doc_analysis.o \
       src/doc_list.o \
       src/msg_content.o \
       src/read_file.o \
       src/spam_blocker.o \
       src/str_list.o \
       src/vocab_list.o

SRCS = $(OBJS:.o=.c)

CFLAGS = -g -Wall -std=c99 -pedantic -Iinclude

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

spam_blocker: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS)

clean:
	rm -rf $(OBJS) spam_blocker
