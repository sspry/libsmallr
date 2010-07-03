#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/mman.h>
#include <sys/user.h>

#include "regex.h"

int r_compile(struct regex_t *res, char *pat, int flags)
{
    int len = 1024; char *c;
    if(posix_memalign((void **)&res->exec, PAGE_SIZE, len)) return -1;
    if(mprotect(res->exec, 5, PROT_EXEC | PROT_WRITE))
    { free(res->exec); return -1; }
    c = res->exec;

    #define pcmd(cm) *c = cm; c++
    pcmd(0x31); pcmd(0xC0);          // xor %eax, %eax;
    pcmd(0xFF); pcmd(0xC0);          // inc %eax;
    pcmd(0xC3);                      // ret;

    return 0;
}

#define add_item(ret, item) ((int *)(ret))[item / 32] |= 1 << (item % 32)
#define del_item(ret, item) ((int *)(ret))[item / 32] &= ~(1 << item % 32)
#define flp_item(ret, item) ((int *)(ret))[item / 32] ^= 1 << (item % 32)
#define set_item(ret, item, val) \
            if(val) { add_item(ret, item); } \
            else    { del_item(ret, item); }
#define get_item(ret, item) ((int *)(ret))[item / 32] &= 1 << (item % 32)

void set_items(void *ret, char *items, int val)
{
    for(; *items; items++)
        set_item(ret, *items, val);
}

void set_range(void *ret, int from, int to, int val)
{
    int i;
    for(i = from; i <= to; i++)
        set_item(ret, i, val);
}

void flp_items(void *ret, char *items)
{
    for(; *items; items++)
        flp_item(ret, *items);
}

void flp_range(void *ret, int from, int to)
{
    int i = from + 1; to--;
    for(; i <= to; i++)
        flp_item(ret, i);
}

void *gen_class(char *inf)
{
    if(*inf == '[')
    { inf++;
      if(inf[strlen(inf) - 1] == ']') inf[strlen(inf) - 1] = 0;
      else return NULL; }

    int state;
    void *ret = malloc(32);
    if(*inf == '^') { memset(ret, 0xFF, 32); state = 0; inf++; }
    else            { memset(ret, 0x00, 32); state = 1; }
    if(*inf == '-')
        { flp_item(ret, '-'); inf++; }
    else if(inf[strlen(inf) - 1] == '-')
        { flp_item(ret, '-'); inf[strlen(inf) - 1] = 0; }

    while(*inf)
    {
        if(*inf == '-')
        {
            set_range(ret, inf[-1], inf[1], state);
        }
        else set_item(ret, inf[0], state);
        inf++;
    }

    return ret;
}
