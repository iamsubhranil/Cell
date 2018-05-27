#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <termios.h>

typedef struct{
    char **parts;
    size_t part_count;
} StringParts;

void parts_free(StringParts parts);

struct Terminal;

typedef void (*term_action)(StringParts parts, struct Terminal *t);

typedef struct{
    const char *keyword;
    const char *color;
    term_action action;
} Keyword;

typedef struct Terminal{
    tcflag_t c_lflag_bak;
    char *line, c;
    const char* prefix;
    size_t size;
    size_t curpos;
    uint8_t escseen;
    uint8_t run;
    Keyword *keywords;
    size_t keyword_count;
} Terminal;

Terminal terminal_init(const char *prefix);
void terminal_add_keyword(Terminal *t, const char *keyword, const char *color, term_action action);
void terminal_repl(Terminal *t);
void terminal_destroy(Terminal *t);
