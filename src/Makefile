
OBJS = doc_analysis.o \
       doc_list.o \
       msg_content.o \
       read_file.o \
       spam_blocker.o \
       str_list.o \
       vocab_list.o

CFLAGS = -g -Wall -std=c99 -ansi -pedantic -I../include

spam_blocker: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS)

clean:
	rm -rf $(OBJS) spam_blocker
