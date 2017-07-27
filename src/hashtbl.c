#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "hashtbl.h"

/*
 * Initialize a new hash table.
 */
int hashtbl_init(HASHTbl *htbl, int buckets, int (*h)(const void *key), int
   (*match)(const void *key1, const void *key2), void (*destroy)(void*data))
{

    int i;

    /* Allocate space for the hash table. */
    if ((htbl->table = (List *)malloc(buckets * sizeof(List))) == NULL)
        return -1;

    /* Initialize the buckets. */
    htbl->buckets = buckets;
    for (i = 0; i < htbl->buckets; i++)
        list_init(&htbl->table[i], destroy);

    /* Encapsulate the functions. */
    htbl->h = h;
    htbl->match = match;
    htbl->destroy = destroy;

    /* Initialize the number of elements in the table. */
    htbl->size = 0;

    return 0;

}

/*
 * Free the resources associated with the hash table.
 */
void hashtbl_destroy(HASHTbl *htbl) {

    int i;
 
    /* Destroy each bucket. */

    for (i = 0; i < htbl->buckets; i++) {
        list_destroy(&htbl->table[i]);
    }

    /* Free the storage allocated for the hash table. */
    free(htbl->table);

    /* Also clear the structure. */
    memset(htbl, 0, sizeof(HASHTbl));

    return;

}

/*
 * Insert an item into the hash table. If the item is already in the table, 
 * no change is made.
 *
 * return 0 on success, 1 if the item is already in the table, and -1 otherwise
 */
int hashtbl_insert(HASHTbl *htbl, const void *data) {

    void *temp;
    int bucket, retval;
 
    /* Do nothing if the data is already in the table. */
    temp = (void *)data;

    if (hashtbl_lookup(htbl, &temp) == 0)
        return 1;

    /* Hash the key. */
    bucket = htbl->h(data) % htbl->buckets;

    /* Insert the data into the bucket. */
    if ((retval = list_ins_next(&htbl->table[bucket], NULL, data)) == 0)
        htbl->size++;

    return retval;

}

/*
 * Remove an item from the hash table.
 */
int hashtbl_remove(HASHTbl *htbl, void **data) {

    ListElmt *element, *prev;
    int bucket;
 
    /*  Hash the key. */
    bucket = htbl->h(*data) % htbl->buckets;

    /* Search for the data in the bucket. */
    prev = NULL;

    for (element = list_head(&htbl->table[bucket]); element != NULL; element =
                list_next(element)) {
        if (htbl->match(*data, list_data(element))) {

            /* Remove the item from the bucket. */
            if (list_rem_next(&htbl->table[bucket], prev, data) == 0) {
                htbl->size--;
                return 0;
            } else {
                return -1;
            }
        }

        prev = element;

    }

    /* Data not found. */
    return -1;

}

/*
 * Retreive the specified item from the hash table.
 */
int hashtbl_lookup(const HASHTbl *htbl, void **data) {

    ListElmt *element;
    int bucket;
 
    /*  Hash the key. */
    bucket = htbl->h(*data) % htbl->buckets;

    /* Search for the data in the bucket. */
    for (element = list_head(&htbl->table[bucket]); element != NULL; element =
            list_next(element)) {

        if (htbl->match(*data, list_data(element))) {
            /* Pass back the data from the table. */
            *data = list_data(element);
            return 0;
        }

    }

    /* Data not found. */
    return -1;

}
