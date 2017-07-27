#ifndef HASHTBL_H
#define HASHTBL_H

#include <stdlib.h>

#include "list.h"

/* Define a structure for chained hash tables. */
typedef struct HASHTbl_ {

    int buckets;

    int (*h)(const void *key);
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);

    int size;
    List *table;

} HASHTbl;

/* Define prototypes for functions. */
int hashtbl_init(HASHTbl *htbl, int buckets, int (*h)(const void *key), int
   (*match)(const void *key1, const void *key2), void (*destroy)(void *data));

void hashtbl_destroy(HASHTbl *htbl);

int hashtbl_insert(HASHTbl *htbl, const void *data);

int hashtbl_remove(HASHTbl *htbl, void **data);

int hashtbl_lookup(const HASHTbl *htbl, void **data);

#define hashtbl_size(htbl) ((htbl)->size)

#endif
