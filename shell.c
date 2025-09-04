#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "interpreter.h"
#include "shellmemory.h"

int parseInput (char ui[]);

// Start of everything
int main (int argc, char *argv[]) {
    // remove this in A3 - 2.1, printf ("Shell version 1.4 created December 2024\n");
    
    // new message of entrance
    printf("Frame Store Size = %d; Variable Store Size = %d\n", FRAME_S_SIZE, VAR_S_SIZE); 

    // thanks max, for 3.5
    fflush (stdout);

    char prompt = '$';          // Shell prompt
    char userInput[MAX_USER_INPUT];     // user's input stored here
    int errorCode = 0;          // zero means no error, default

    //init user input
    for (int i = 0; i < MAX_USER_INPUT; i++) {
        userInput[i] = '\0';
    }

    //init shell memory
    mem_init ();
    init_source ();

    while (1) {
        // here you should check the unistd library 
        // so that you can find a way to not display $ in the batch mode

        // if that is true, it's a terminal. so 2.2 done    
        if (isatty (fileno (stdin))) {
            printf ("%c ", prompt);
        }
        // ok do 2.1
        // If the input is null -> leave in batch mode
        if (fgets (userInput, MAX_USER_INPUT - 1, stdin) == NULL) {
            break;
        }

        errorCode = parseInput (userInput);
        if (errorCode == -1)
            exit (99);          // ignore all other errors
        memset (userInput, 0, sizeof (userInput));
    }

    return 0;
}

int wordEnding (char c) {
    // You may want to add ';' to this at some point,
    // or you may want to find a different way to implement chains.
    return c == '\0' || c == '\n' || c == ' ' || c == ';';      // changed for 4. 
}

int parseInput (char inp[]) {   // inp being the input by the user 
    char tmp[200], *words[100];
    int ix = 0, w = 0;
    int wordlen;
    int errorCode;
    char *com_array[10];        // because 10 is the limit of commands

    // at most 1000 characters for all intructions, otherwhise error
    int size_combined = strlen (inp);
    if (1000 < size_combined) {
        printf ("\n");          // print enpty line I guess
        return 1;               // 1 because error
    }
    // at most 10 instructions 
    int i = 0;                  // counter
    int j = 0;
    char *current_com = strtok (inp, ";");      // takes first command 
    while (i < 10 && current_com != NULL) {
        com_array[i] = current_com;
        i = i + 1;
        current_com = strtok (NULL, ";");       // go to the next command
    }

    // Carlos : ok so array com_array has all commands, it could have between 1 and 10

    int n = 0;                  // to chose between the commands 
    int size = i;               // the i is equivalent to the number of commands
    while (n < i) {
        // ahhhh reset ix and w each loop because shit happens
        ix = 0;
        w = 0;

        // skip white spaces for each command 
        for (ix = 0; com_array[n][ix] == ' ' && ix < 1000; ix++);

        // get word
        while (com_array[n][ix] != '\n' && com_array[n][ix] != '\0'
               && ix < 1000) {

            for (wordlen = 0; !wordEnding (com_array[n][ix]) && ix < 1000;
                 ix++, wordlen++) {
                tmp[wordlen] = com_array[n][ix];
            }

            tmp[wordlen] = '\0';
            words[w] = strdup (tmp);
            w++;
            if (com_array[n][ix] == '\0')
                break;
            ix++;
        }
        // after preparing it, call the command 
        errorCode = interpreter (words, w);
        n = n + 1;
    }
    return errorCode;
}
