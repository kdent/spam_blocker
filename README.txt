
Steps

Training:

get list of message files

for each file:

- read email message to extract from address and subject line.
    (read_file.c, msg_content.c)
- tokenize the from address ignoring the address itself and the subject line
    o look for other features like camel caps, numbers for letters substitution
    (doc_features.c)
- store each unique token in the list of words
- create a matrix of all the words in the corpus


