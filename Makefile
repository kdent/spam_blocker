CC = cc
OBJS = src/doc_features.o \
       src/doc_list.o \
       src/msg_content.o \
       src/read_file.o \
       src/str_list.o \
       src/vectorize.o \
       src/vocab_list.o
DEPS = include/doc_features.h \
       include/doc_list.h \
       include/msg_content.h \
       include/read_file.h \
       include/str_list.h \
       include/vectorize.h \
       include/vocab_list.h

SRCS = $(OBJS:.o=.c)

CFLAGS = -g -Wall -std=c99 -pedantic -Iinclude

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

spam_blocker: $(OBJS) $(DEPS) src/spam_blocker.c
	$(CC) -o $@ $(CFLAGS) $(OBJS) src/$(@).c


test_doc_list: $(OBJS) $(DEPS) test/test_doc_list.c
	$(CC) -o $@ $(CFLAGS) $(OBJS) test/$(@).c

test_doc_features: $(OBJS) $(DEPS) test/test_doc_features.c test/test_cases.o
	$(CC) -o $@ $(CFLAGS) $(OBJS) test_cases.o test/$(@).c

test_msg_content: $(OBJS) $(DEPS) test/test_msg_content.c
	$(CC) -o $@ $(CFLAGS) $(OBJS) test/$(@).c

test_vectorize: $(OBSJS) $(DEPS) test/test_vectorize.c test/test_cases.o
	$(CC) -o $@ $(CFLAGS) $(OBJS) test_cases.o test/$(@).c

test_vocab_list: $(OBSJS) $(DEPS) test/test_vocab_list.c test/test_cases.o
	$(CC) -o $@ $(CFLAGS) $(OBJS) test_cases.o test/$(@).c

test/test_cases.o: test/test_cases.c test/test_cases.h
	$(CC) -c $(?)

clean:
	rm -rf $(OBJS) spam_blocker src/spam_blocker.o test_* *.dSYM test/*.gch
