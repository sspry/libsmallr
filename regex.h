#ifndef REGEX_H
#define REGEX_H

struct regex_t
{
    void *exec;
};

int r_compile(struct regex_t *res, char *pat, int flags);
void r_append_mchar(char c, char **exe);
void r_append_mdot(int lf, char **exe);
#endif
