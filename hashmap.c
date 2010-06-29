#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashmap.h"

int mhash(char *key, int size)
{
    unsigned h, b;

    if(!key || !size) return 0;
    for(h = tolower(*key++); *key; key++)
    { b = (h & 1) ? (1 << 3) : 0;
      h >>= 1; h += tolower(*key) + b; }
    return h % size;
}

void move_up(hashmap *h, int b, hashent *e)
{
    hashent *x, *y, *z;
    if(!e->prev) return;

    x = e->prev; y = x->prev; z = e->next;
    if(y) y->next = e; else h->table[b] = e;
    if(z) z->prev = x;
    x->next = e->next; e->prev = e;
    e->next = x; e->prev = y;
}

void move_front(hashmap *h, int b, hashent *e)
{
    if(e == h->table[b]) return;
    e->prev->next = e->next;
    if(e->next) e->next->prev = e->prev;

    e->next = h->table[b]; e->prev = NULL;
    h->table[b]->prev = e; h->table[b] = e;
}

hashent *he_new(char *k, void *data)
{
    int kl = strlen(k);
    hashent *e;

    if(!(e = malloc(sizeof(*e) + kl + 1))) return NULL;

    e->next = NULL; e->prev = NULL;
    e->v = data; e->kl = kl; e->k = (char *)(e + 1);
    strcpy(e->k, k);

    return e;
}

void he_free(hashent *e)
{
    if(!e) return;
    else free(e);
}

hashmap *hm_new(int size, int maxbk, int op)
{
    hashmap *h; int i = 1;

    if(!(h = malloc(sizeof(*h)))) return NULL;
    h->size = 1; h->items = 0; h->maxbk = maxbk; h->op = op;
    while(h->size < size) h->size = 1 << i++;

    if(!(h->table = calloc(h->size, sizeof(hashent *))))
        { free(h); return NULL; }
    if(!(h->nl = calloc(h->size, sizeof(int))))
        { free(h->table); free(h); return NULL; }
    return h;
}

void remove_link(hashmap *h, int b, hashent *e)
{
    if(e->prev) e->prev->next = e->next;
    else h->table[b] = e->next;
    if(e->next) e->next->prev = e->prev;
}

void free_chain(hashent *e)
{
    hashent *pos = e, *n = pos;

    if(!e) return;
    while(pos) { n = pos->next;
    he_free(pos); pos = n; }
}

void hm_free(hashmap *h)
{
    int i = 1;

    for(i = 0; i < h->size; i++)
        free_chain(h->table[i]);
    free(h->table);
    free(h->nl);
    free(h);
}

hashent *search_bucket(hashmap *h, int b, char *k)
{
    hashent *e;

    for(e = h->table[b]; e; e = e->next)
    {

        if((e->kl == strlen(k)) && (!strcmp(e->k, k)))
        {
            switch(h->op)
            {
            case OP_MOVE_FRONT:
                move_front(h, b, e); break;
            case OP_MOVE_UP:
                move_up(h, b, e); break;
            }
            return e;
        }
    }
    return NULL;
}

int hm_insert(hashmap *h, char *k, void *data)
{
    hashent *e, *p = NULL; unsigned hkey;

    hkey = mhash(k, h->size);
    if(h->items + 1 > h->size) return 0;
    if(search_bucket(h, hkey, k)) return 0;
    if(!(e = he_new(k, data))) return 0;

    e->next = h->table[hkey]; e->prev = NULL;
    if(e->next) e->next->prev = e; h->table[hkey] = e;

    if(h->maxbk && h->nl[hkey] >= h->maxbk)
    {
        for(p = h->table[hkey]; p->next; p = p->next);

        remove_link(h, hkey, p);
        he_free(p);
    }
    else h->nl[hkey]++;
    h->items++;

    return 1;
}

void *hm_get(hashmap *h, char *k)
{
    hashent *e; unsigned hkey;

    hkey = mhash(k, h->size);
    e = search_bucket(h, hkey, k);

    return e ? e->v : NULL;
}

void *hm_replace(hashmap *h, char *k, void *data)
{
    hashent *e; unsigned hkey;
    void *old = NULL;

    hkey = mhash(k, h->size);
    e = search_bucket(h, hkey, k);

    if(!e) return NULL;
    old = e->v; e->v = data;

    return old;
}

void *hm_remove(hashmap *h, char *k)
{
    hashent *e; unsigned hkey;
    void *ret = NULL;

    hkey = mhash(k, h->size);
    e = search_bucket(h, hkey, k);

    if(e)
    {
        ret = e->v;
        remove_link(h, hkey, e);
        h->items--; h->nl[hkey]--; he_free(e);
    }

    return ret;
}
