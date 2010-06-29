#ifndef HASHMAP_H
#define HASHMAP_H

#define OP_NONE       0
#define OP_MOVE_UP    1
#define OP_MOVE_FRONT 2

typedef struct _hashent
{
    void *v;
    char *k; int kl;
    struct _hashent *next, *prev;
} hashent;

typedef struct _hashmap
{
    int size, items, maxbk, op, *nl;
    struct _hashent **table;
} hashmap;

int mhash(char *key, int size);
void move_up(hashmap *h, int b, hashent *e);
void move_front(hashmap *h, int b, hashent *e);

hashent *he_new(char *k, void *data);
void he_free(hashent *e);

hashmap *hm_new(int size, int maxbk, int op);
void remove_link(hashmap *h, int b, hashent *e);
void free_chain(hashent *e);
void hm_free(hashmap *h);

hashent *search_bucket(hashmap *h, int b, char *k);

int hm_insert(hashmap *h, char *k, void *data);
void *hm_get(hashmap *h, char *k);
void *hm_replace(hashmap *h, char *k, void *data);
void *hm_remove(hashmap *h, char *k);

#endif
