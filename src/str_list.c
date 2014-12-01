#include <stdlib.h>
#include <string.h>

#include <str_list.h>

static int add(str_list *list, char *str);

/*
 * Initialize a str_list list. When the caller is finished with the list, it
 * should call str_list_free to free the memory used by the list.
 */
str_list *
str_list_init()
{
    str_list *list = (str_list *)malloc(sizeof(str_list));
    if (list == NULL)
        return NULL;
    list->first_item = NULL;
    list->cur_item = NULL;
    list->last_item = NULL;
    list->n_items = 0;
    return list;
}

void
str_list_free(str_list *list)
{
    struct _list_item *cur_item = NULL;
    struct _list_item *next_item = NULL;

    if (list == NULL)
        return;

    cur_item = list->first_item;
    while (cur_item != NULL) {
        next_item = cur_item->next;
        if (cur_item->str != NULL) free(cur_item->str);
        free(cur_item);
        cur_item = next_item;
    }

    free(list);
}

/*
 * Copy the string in str_val and add it to the list.
 * Return 1 on success or 0 if there was an error allocating memory
 * or if list is null
 */
int
str_list_add(str_list *list, char *str_val)
{
    char *new_str = (char *)malloc(strlen(str_val) + 1);

    if (list == NULL)     return 0;
    if (new_str == NULL) return 0;

    strncpy(new_str, str_val, strlen(str_val) + 1);
    return add(list, new_str);
}

int
str_list_add_substr(str_list *list, char *str_val, int nchars)
{
    char *new_str = (char *)malloc(strlen(str_val) + 1);

    if (list == NULL)     return 0;
    if (new_str == NULL)  return 0;

    strncpy(new_str, str_val, nchars);
    return add(list, new_str);
}

static int
add(str_list *list, char *str)
{
    struct _list_item *new_item =
        (struct _list_item *)malloc(sizeof(struct _list_item));
    if (new_item == NULL) return 0;

    new_item->str = str;
    new_item->next = NULL;

    if (list->first_item == NULL) {
        list->first_item = new_item;
    } else {
        if (list->last_item == NULL)   /* Something wrong here. */
            return 0;
        list->last_item->next = new_item;
    }
    list->last_item = new_item;
    list->n_items++;

    return 1;
}

char *
str_list_replace(str_list *list, char *existing_item, char *replacement)
{
    struct _list_item *cur;
    int replacement_made = 0;

    cur = list->first_item;
    while (cur != NULL) {
        if (strcasecmp(cur->str, existing_item) == 0) {
            free(cur->str);
            cur->str = (char *)malloc(strlen(replacement) + 1);
            if (cur->str == NULL)   /* Memory error, bail out. */
                return NULL;
            strcpy(cur->str, replacement);
            replacement_made = 1;
            break;
        }
        cur = cur->next;
    }
    if (replacement_made)
        return cur->str;
    else
        return NULL;
}

char *
str_list_first(str_list *list)
{
    if (list == NULL)
        return NULL;

    list->cur_item = list->first_item;
    if (list->cur_item != NULL)
        return list->cur_item->str;
    else
        return NULL;
}

char *
str_list_next(str_list *list)
{
    if (list == NULL)
        return NULL;

    if (list->cur_item == NULL)
        return NULL;

    list->cur_item = list->cur_item->next;
    if (list->cur_item == NULL)
        return NULL;
    else
        return list->cur_item->str;
}

int
str_list_contains(str_list *list, char *str_val)
{
    if (list == NULL)
        return 0;
    if (str_val == NULL)
        return 0;

    list->cur_item = list->first_item;
    while (list->cur_item != NULL) {
        if (strcasecmp(list->cur_item->str, str_val) == 0)
            return 1;
        list->cur_item = list->cur_item->next;
    }
    return 0;
}

int
str_list_size(str_list *list)
{
    if (list == NULL)
        return 0;
    return list->n_items;
}

