# Cell
## An embeddable shell for POSIX systems, wrtiten in C

This library is meant to be an easily embeddable shell for providing a customized CUI to your program. It supports syntax highlighting, intrinsics, and should support autocomplete in the future.

## API

#### Files

1. `cell.h` : The library header containing declarations of the structures and functions.

2. `cell.c` : The implementation of the library.

#### Types

1. `CellStringParts` : To tokenize a string at the occurrence of a space and store the parts. The `parts` array contains the null terminated splitted strings, and the `part_count` variable contains the number of parts in the string. Any consecutively occurring space is considered to be a string of size 0 (1 if you count the null character).

2. `cell_action` : The method type which will be invoked when a keyword is entered. The first argument to the function is the whole line given at the shell at the time of invokation, space splitted, including the keyword name as the first part. The second argument is the shell itself, whose internal state can also be modified by the action, for example setting the `run` variable to `0` will cause the REPL to exit when the action method terminates.

2. `CellKeyword` : To specify a collection of characters as a keyword. The `keyword` variable is the actual keyword string, `color` variable is one of the `ANSI_COLOR_*` from `cell.h`, and the `action` variable is a pointer to the method which is to be invoked when a line preceding that keyword is entered in the shell, followed by an `Enter`.

3. `Cell` : The actual shell structure, which contains the state of the shell at any given point of time.

#### Functions

1. `cell_init` : Initialize a shell with a given prefix string, i.e. the leftmost string to display when the shell is waiting for user input.

2. `cell_add_keyword` : Add a keyword to the shell, with a color to highlight when the keyword is entered, and an action to invoke when the keyword is called.

3. `cell_repl` : Start the REPL loop. The only way to exit this loop is by setting the `run` variable to 0, so you must provide some sort of an `exit` keyword to the user.

4. `cell_destroy` : Destroy a initialized cell. It basically frees the keyword array, since the cell is statically allocated.

5. `cell_stringparts_free` : Free a `CellStringParts` structure. Each invoked action must manually do this after the arguments are used.
