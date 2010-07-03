#ifndef REGEX_H
#define REGEX_H

/*
 * This file is supposed to be the face of the project,
 * and aims to offer the main functionality without
 * exposing unnecessary details.
 */

struct regex_t
{
    void *exec;
};

int r_compile(struct regex_t *res, char *pat, int flags);
void r_append_group(char **exec, char *g);
void r_append_char(char **exec, char c);
#endif
