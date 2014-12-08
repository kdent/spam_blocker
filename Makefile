CC = cc
OBJS = src/doc_analysis.o \
       src/doc_list.o \
       src/msg_content.o \
       src/read_file.o \
       src/str_list.o \
       src/vocab_list.o
DEPS = include/doc_analysis.h \
       include/doc_list.h \
       include/msg_content.h \
       include/read_file.h \
       include/str_list.h \
       include/vocab_list.h

SRCS = $(OBJS:.o=.c)

CFLAGS = -g -Wall -std=c99 -pedantic -Iinclude

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

spam_blocker: $(OBJS) $(DEPS) src/spam_blocker.c
	$(CC) -o $@ $(CFLAGS) $(OBJS) src/$(@).c


test_doc_list: $(OBJS) $(DEPS) test/test_doc_list.c
	$(CC) -o $@ $(CFLAGS) $(OBJS) test/$(@).c

clean:
	rm -rf $(OBJS) spam_blocker test_* *.dSYM
