#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <termios.h>

typedef struct{
    char **parts;
    size_t part_count;
} CellStringParts;

void cell_stringparts_free(CellStringParts parts);

struct Cell;

typedef void (*cell_action)(CellStringParts parts, struct Cell *t);

typedef struct{
    const char *keyword;
    const char *color;
    cell_action action;
} CellKeyword;

typedef struct Cell{
    tcflag_t c_lflag_bak;
    char *line, c;
    const char* prefix;
    size_t size;
    size_t curpos;
    size_t allocated_size;
    uint8_t escseen;
    uint8_t run;
    CellKeyword *keywords;
    size_t keyword_count;
} Cell;

// Colored characters
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_FONT_BOLD     "\x1b[1m"

Cell cell_init(const char *prefix);
void cell_add_keyword(Cell *t, const char *keyword, const char *color, cell_action action);
void cell_repl(Cell *t);
void cell_destroy(Cell *t);
