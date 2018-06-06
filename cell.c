#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

//#include "display.h"
#include "cell.h"

typedef size_t siz;
#define Psiz "zd"
typedef uint8_t u8;

#define KB_BACKSPACE 127
#define KB_DELETE 126
#define KB_TAB 9
#define KB_ARW_UP 65
#define KB_ARW_DWN 66
#define KB_ARW_RGT 67
#define KB_ARW_LFT 68
#define KB_ESC 27
#define KB_SQB_OPN 91

void cell_stringparts_free(CellStringParts s){
    for(siz i = 0;i < s.part_count;i++)
        free(s.parts[i]);
    free(s.parts);
}

static CellStringParts string_split(const char *str, char c, siz max){
    CellStringParts sp;
    sp.part_count = 0;
    sp.parts = NULL;
    siz s = 0;
    while(str[s] != '\0' && s < max){
        sp.parts = (char **)realloc(sp.parts, sizeof(char *) * ++sp.part_count);
        sp.parts[sp.part_count - 1] = NULL;
        siz t = 0;
        while(str[s] != '\0' && str[s] != c && s < max){
            sp.parts[sp.part_count - 1] = (char *)realloc(sp.parts[sp.part_count - 1],
                    ++t);
            sp.parts[sp.part_count - 1][t - 1] = str[s];
            s++;
        }
        if(str[s] == c)
            s++;
        sp.parts[sp.part_count - 1] = (char *)realloc(sp.parts[sp.part_count - 1], ++t);
        sp.parts[sp.part_count - 1][t - 1] = '\0';
    }
    return sp;
}

static void noecho(Cell *term){
    struct termios t;
    tcgetattr(0, &t);
    term->c_lflag_bak = t.c_lflag;
    t.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(0, TCSANOW, &t);
}

static void noecho_off(Cell *term){
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag = term->c_lflag_bak;
    tcsetattr(0, TCSANOW, &t);
}

static CellKeyword* keyword_match(const char* line, Cell *t){
    //CellStringParts sp = string_split(&line[pos], ' ', strlen(line));
    //dbg("Searching %s\n", sp.parts[0]);
    for(siz i = 0;i < t->keyword_count;i++){
        if(strcmp(line, t->keywords[i].keyword) == 0)
            return &t->keywords[i];
    }
    return NULL;
}

static void highlight_and_print(Cell *t, siz max){
    printf("\r%s ", t->prefix);
    siz printed = 0;
    CellStringParts part = string_split(t->line, ' ', strlen(t->line));
    for(siz i = 0;i < part.part_count && printed < max;i++){
        //u8 space_printed = 0;
        if(part.parts[i][0] != '\0'){
            CellKeyword* keyword = keyword_match(part.parts[i], t);
            if(keyword != NULL){
                printf("%s", keyword->color);
            }
            for(siz j = 0;printed<max && part.parts[i][j] != '\0';j++,printed++)
                putchar(part.parts[i][j]);
            printf(ANSI_COLOR_RESET);
        }
        if(printed < max)
            putchar(' '), printed++;
    }
    cell_stringparts_free(part);
}

static void line_clear(const char *prefix, siz size){
    printf("\r%*s ", (int)strlen(prefix), " ");
    for(siz i = 0;i < size + 1;i++)
        printf(" ");
}

static void execute_statement(Cell *t){
    CellStringParts sp = string_split(t->line, ' ', t->size);
    for(siz i = 0;i < t->keyword_count;i++){
        if(strcmp(sp.parts[0], t->keywords[i].keyword) == 0){
            t->keywords[i].action(sp, t);
            return;
        }
    }
    printf(ANSI_COLOR_RED ANSI_FONT_BOLD "\n[Error]" ANSI_COLOR_RESET " No such action : %s", sp.parts[0]);
    cell_stringparts_free(sp);
}

void cell_repl(Cell *t){
    noecho(t);
    printf("\n%s ", t->prefix);
    //char *line = NULL, c;
    //siz size = 0, curpos = 0;
    //u8 escseen = 0;
    while(t->run){
        t->c = getchar();
        //dbg("KeyPressed : %d [%c]", c, c);
        if(t->c == '\n'){
            if(t->size != 0){
                t->line[t->size] = '\0';
                highlight_and_print(t, t->size);
                execute_statement(t);
                memset(t->line, 0, t->size + 1);
                t->size = 0;
                t->curpos = 0;
            }
            if(t->run)
                printf("\n%s ", t->prefix);
        }
        else if(t->c == KB_BACKSPACE || t->c == KB_DELETE){
            if(t->curpos > 0){
                line_clear(t->prefix, t->size);
                for(siz i = t->curpos-1; i < t->size;i++)
                    t->line[i] = t->line[i+1];
                t->line[t->size - 1] = '\0';
                t->size--;
                t->curpos--;
                highlight_and_print(t, t->size);
                if(t->curpos != t->size){
                    highlight_and_print(t, t->curpos);
                }
                //printf(" ");
            }
        }
        else if(t->c == KB_ESC)
            t->escseen = 1;
        else if(t->escseen && t->c == '['){
            t->c = getchar();
            if(t->c == KB_ARW_LFT){
                if(t->curpos > 0){
                    t->curpos--;
                    //dbg("Curpos : %" Psiz "\tSize : %" Psiz "\n", curpos, size);
                    highlight_and_print(t, t->curpos);
                }
            } 
            else if(t->c == KB_ARW_RGT){
                if(t->curpos < t->size){
                    t->curpos++;
                    //dbg("Curpos : %" Psiz "\tSize : %" Psiz "\n", curpos, size);
                    highlight_and_print(t, t->curpos);
                }
            } 
            else if(t->c == KB_ARW_UP){}
            else if(t->c == KB_ARW_DWN){}
        }
        else{
            if(t->allocated_size == 0){ 
                t->line = (char *)realloc(t->line, 2);
                t->allocated_size = 2;
                t->line[1] = '\0';
            }
            else if(t->allocated_size < (t->size + 2)){
                t->allocated_size++;
                t->line = (char *)realloc(t->line, (t->allocated_size));
                t->line[t->allocated_size - 1] = '\0';
            }
            for(siz i = t->size;i > t->curpos;i--)
                t->line[i] = t->line[i - 1];
            t->line[t->curpos] = t->c;
            //dbg("Curpos : %" Psiz "\tSize : %" Psiz "\n", curpos + 1, size + 1);
            highlight_and_print(t, t->size + 1);
            t->curpos++;
            t->size++;
            if(t->curpos != t->size)
                highlight_and_print(t, t->curpos);
        }
    }
    free(t->line);
    t->line = NULL;
    t->size = 0;
    t->curpos = 0;
    noecho_off(t);
}

Cell cell_init(const char *prefix){
    Cell t;
    t.c = 0;
    t.curpos = 0;
    t.escseen = 0;
    t.keywords = NULL;
    t.keyword_count = 0;
    t.line = NULL;
    t.prefix = prefix;
    t.run = 1;
    t.size = 0;
    t.c_lflag_bak = 0;
    t.allocated_size = 0;
    return t;
}

void cell_add_keyword(Cell *t, const char *keyword, const char *color, cell_action action){
    CellKeyword *k = keyword_match(keyword, t);
    if(k != NULL){
        k->color = color;
        k->action = action;
        return;
    }
    t->keywords = (CellKeyword *)realloc(t->keywords, sizeof(CellKeyword) * ++t->keyword_count);
    t->keywords[t->keyword_count - 1].keyword = keyword;
    t->keywords[t->keyword_count - 1].color = color;
    t->keywords[t->keyword_count - 1].action = action;
}

void cell_destroy(Cell *t){
    free(t->keywords);
}
