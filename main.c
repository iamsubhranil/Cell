#include <string.h>
#include <stdio.h>
#include "display.h"
#include "cell.h"

static void not_implemented_action(CellStringParts parts, Cell *t){
    (void)t;
    warn("Action not implemented : %s", parts.parts[0]);
    cell_stringparts_free(parts);
}

static void exit_action(CellStringParts parts, Cell *t){
    (void)parts;
    (void)t;
    info("Exiting..\n");
    t->run = 0;
    cell_stringparts_free(parts);
}

static CellKeyword keywords[] = {
    {"help", ANSI_COLOR_GREEN, not_implemented_action},
    {"run", ANSI_COLOR_GREEN, not_implemented_action},
    {"run_from", ANSI_COLOR_GREEN, not_implemented_action},
    {"load", ANSI_COLOR_GREEN, not_implemented_action},
    {"load_at", ANSI_COLOR_GREEN, not_implemented_action},
    {"save", ANSI_COLOR_GREEN, not_implemented_action},
    {"save_as", ANSI_COLOR_GREEN, not_implemented_action},
    {"interpret", ANSI_COLOR_GREEN, not_implemented_action},
    {"exit", ANSI_COLOR_YELLOW, exit_action}
};

static void hi_test_action(CellStringParts parts, Cell *t){
    (void)t;
    dbg("In hi_test_action!");
    dbg("Argument Count : %" "zd", parts.part_count);
    for(size_t i = 0;i < parts.part_count;i++){
        dbg("Argument[%" "zd" "] : %s", i+1, parts.parts[i]);
    }
    cell_stringparts_free(parts);
}

void nested_term_action(CellStringParts parts, Cell *t1){
    (void)parts; (void)t1;
    dbg("Spawning nested cell");
    char np[strlen(t1->prefix) + strlen(ANSI_FONT_BOLD) + strlen(ANSI_COLOR_RESET) + 4];
    snprintf(np, sizeof(np), "%s" ANSI_FONT_BOLD "->>" ANSI_COLOR_RESET, t1->prefix);
    Cell t = cell_init(np);
    cell_add_keyword(&t, "hi", ANSI_COLOR_BLUE, hi_test_action);
    for(size_t i = 0;i < sizeof(keywords)/sizeof(CellKeyword);i++)
        cell_add_keyword(&t, keywords[i].keyword, keywords[i].color, keywords[i].action);
    cell_add_keyword(&t, "nest", ANSI_COLOR_YELLOW, nested_term_action);
    cell_repl(&t);
    cell_destroy(&t);
    cell_stringparts_free(parts);
}

int main(){
    Cell t = cell_init(ANSI_FONT_BOLD ">>" ANSI_COLOR_RESET);
    cell_add_keyword(&t, "hi", ANSI_COLOR_BLUE, hi_test_action);
    for(size_t i = 0;i < sizeof(keywords)/sizeof(CellKeyword);i++)
        cell_add_keyword(&t, keywords[i].keyword, keywords[i].color, keywords[i].action);
    cell_add_keyword(&t, "nest", ANSI_COLOR_YELLOW, nested_term_action);
    cell_repl(&t);
    cell_destroy(&t);
    return 0;
}
