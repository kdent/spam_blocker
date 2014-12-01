#ifndef STR_LIST_H
#define STR_LIST_H

struct _list_item {
    char *str;
    struct _list_item *next;
};

typedef struct _str_list {
    struct _list_item *first_item;
    struct _list_item *cur_item;
    struct _list_item *last_item;
    int n_items;
} str_list;

str_list *str_list_init();
void str_list_free(str_list *list);
int str_list_add(str_list *list, char *str_val);
int str_list_add_substr(str_list *list, char *str_val, int nchars);
char *str_list_replace(str_list *list, char *existing_item, char *replacement);
char *str_list_first(str_list *list);
char *str_list_next(str_list *list);
int str_list_contains(str_list *list, char *str_val);
int str_list_size(str_list *list);

#endif
