/****************************************************/
/* File: main.c (20171643 Park Junhyeok)            */
/* Main program for C-minus compiler                */
/****************************************************/

#include "globals.h"

/* Set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE TRUE
/* Sset NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE

/* Set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE FALSE

#include "util.h"
#if NO_PARSE
// #include "scan.h" (We do not use this header)
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif

 /* Allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* Allocate and set tracing flags */
/* We don't use these except for Scan one */
int EchoSource = FALSE;
int TraceScan = TRUE;
int TraceParse = FALSE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;
int Error = FALSE;

int main(int argc, char *argv[])
{	/* Source code file name is 'argv[1]' ex) hw1.c */
	/* You must guarantee 'argv[1](hw1.c)' is the UNIX
	   LF type of file */
	char input[20]; char output[40];
	if (argc == 2) 
		strcpy(input, argv[1]);
	else
	{
		fprintf(stderr, "Error: hw1_binary file_name(.c)\n", input);
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
	   if input is 'hw1.c', then 'hw1_20171643.txt' */
	listing = fopen(output, "wt");
	fprintf(listing, "\nTINY COMPILATION: %s\n", input);
	fprintf(listing, "    line number\t\t\ttoken\t\t\tlexeme\n");
	for (int i = 0; i < 80; i++) fprintf(listing, "=");
	fprintf(listing, "\n");

#if NO_PARSE
	while (getToken() != ENDFILE);
#else
	TreeNode* syntaxTree;
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
