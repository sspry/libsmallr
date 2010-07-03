#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "regex.h"
#include "hashmap.h"

int main()
{
    // /:(?P<sender>(?P<nick>[^! ]+)(!(?P<user>[^@]+)@(?P<host>\S+))?)
    // \s+(?P<cmd>\S+)\s+(?P<params>[^:]*)(:(?P<mess>.*))?/
    char pattern[] = ".at",
         input[]   = "cat";
    struct regex_t mtch;

    if(r_compile(&mtch, pattern, 0) < 0)
    { puts("FATAL: Bad pattern."); }

    int ret = ((int (*)(char *))(mtch.exec))(input);
    printf("Returned %d.", ret);

    free(mtch.exec);
    return 0;
}
