/****************************************************/
/* File: main.c (20171643 Park Junhyeok)            */
/* Main program for C-minus compiler                */
/* The final version of my submission for the proje-*/
/* -ct phase #1 and #2, which are all completed. The*/
/* last modification has been done at 7th Dec, 2022 */
/****************************************************/

#include "globals.h"

/* Set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* Sset NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE TRUE
/* Set NO_CODE to TRUE to get a compiler that does not
 * generate code */
#define NO_CODE TRUE

#include "util.h"
#if NO_PARSE
// #include "scan.h" (We do not use this header)
#else
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif

 /* Allocate global variables */
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

/* Allocate and set tracing flags */
/* We don't use these except for Parse one
 * for the project #2, so set 'TraceParse' */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;
int Error = FALSE;

int main(int argc, char *argv[])
{	/* Source code file name is 'argv[1]' ex) test1.c */
    /* You must guarantee 'argv[1](test1.c)' is the UNIX
     * LF type of file. This is very important point! */
    char input[20]; char output[40];
    if (argc == 2) 
        strcpy(input, argv[1]);
    else
    {
        fprintf(stderr, "Error: hw2_binary file_name(.c)\n");
        exit(1);
    }

    if (strchr(input, '.') == NULL)
        strcat(input, ".tny");

    source = fopen(input, "r");
    if (source == NULL)
    {
        fprintf(stderr, "File '%s' not found\n", input);
        exit(1);
    }

    strcpy(output, input);
    for (int i = 0; i < strlen(output); i++)
        if (output[i] == '.') 
        { output[i] = '\0'; break; }
    strcat(output, "_20171643.txt");

    /* Send listing to '(file_name)_20171643.txt' 
     * if input is 'test1.c', then 'test1_20171643.txt' */
    listing = fopen(output, "wt");
    fprintf(listing, "\nTINY COMPILATION: %s\n", input);

#if NO_PARSE
    fprintf(listing, "    line number\t\t\ttoken\t\t\tlexeme\n");
    for (int i = 0; i < 80; i++) fprintf(listing, "=");
    fprintf(listing, "\n");

    while (getToken() != ENDFILE);
#else
    /* In the project #2, we gonna construct a parse
     * tree based on the result of scanning phase */
    TreeNode *syntaxTree;
    syntaxTree = parse();
    if (TraceParse) {
        fprintf(listing, "\nSyntax tree:\n");
        printTree(syntaxTree);
    }
#if !NO_ANALYZE
    if (!Error)
    {
        if (TraceAnalyze) fprintf(listing, "\nBuilding Symbol Table...\n");
        buildSymtab(syntaxTree);
        if (TraceAnalyze) fprintf(listing, "\nChecking Types...\n");
        typeCheck(syntaxTree);
        if (TraceAnalyze) fprintf(listing, "\nType Checking Finished\n");
    }
#if !NO_CODE
    if (!Error)
    {
        char * codefile;
        int fnlen = strcspn(pgm, ".");
        codefile = (char *)calloc(fnlen + 4, sizeof(char));
        strncpy(codefile, pgm, fnlen);
        strcat(codefile, ".tm");
        code = fopen(codefile, "w");
        if (code == NULL)
        {
            printf("Unable to open %s\n", codefile);
            exit(1);
        }
        codeGen(syntaxTree, codefile);
        fclose(code);
    }
#endif
#endif
#endif
    fclose(source);
    fclose(listing);
    return 0;
}
