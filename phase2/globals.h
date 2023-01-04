/****************************************************/
/* File: globals.h (20171643 Park Junhyeok)         */
/* Global types and vars for C-minus compiler       */
/* This file is included in every file attached     */
/****************************************************/
/****************************************************/
/* The final version of my submission for the proje-*/
/* -ct phase #1 and #2, which are all completed. The*/
/* last modification has been done at 7th Dec, 2022 */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* In the project #2 phase, I'm gonna make the Bison produce
 * a LALR(1) parsing table, and use the production rules of it 
 * to perform a LALR(1) parsing for some c-minus source codes.
 *
 * For this approach, we should keep in mind that the Bison 
 * generates the corresponding values for the input tokens  
 * internally, and we can use these values by including the 
 * 'tab.h' header which is made by the Bison option '-d'.
 *
 * Thus, let's include 'tab.h' just like below. 'tiny.tab.h'
 *
 * Note that the 'YYPARSER' flag prevents a situation that the 
 * 'tab.h' header is included in the Bison output file. 
 * 
 * And also note that 'ENDFILE' macro isn't defined in the Bison,
 * so we should declare here to make the 'tiny.l' file use it. */
#ifndef YYPARSER
#include "tiny.tab.h"
#define ENDFILE 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* Set PROJECT2 to TRUE to replace the TokenType with Bison's */
#define PROJECT2 TRUE

/* MAXTOKENLEN = Maximum length of tokens 
 * This macro is defined in scan.h also.
 * Thus, don't include scan.h in here */
#define MAXTOKENLEN 40

/* MAXRESERVED = The number of reserved words */
#define MAXRESERVED 6

#if !PROJECT2
/* Token types based on lexical conventions of C-minus */
typedef enum
{
    /* Book-keeping tokens */
    ENDFILE, ERROR,

    /* Reserved words */
    ELSE, IF, INT, RETURN, VOID, WHILE,

    /* Multicharacter tokens */
    ID, NUM,

    /* Special symbols */
    PLUS, MINUS, TIMES, OVER,							//	 + - * /
    GT, GTEQ, LT, LTEQ, EQ, NEQ,						//	 > >= < <= == !=
    ASSIGN, SEMI, COMMA,								//   = ; ,
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACK, RBRACK		//   ( ) { } [ ]
} TokenType;
#else
/* In the project phase #2, the Bison requires the 
 * 'TokenType' to be just an integer type */
typedef int TokenType;
#endif

extern FILE *source;			/* Source code text file */
extern FILE *listing;			/* Listing output text file */
extern FILE *code;				/* Code text file for TM simulator */

extern int lineno;				/* Source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/* The main data structure for this project phase */
/**************************************************/

/* < Kinds of Node >
 * - Statement Node
 * - Expression Node
 * - Declaration Node 
 * We reference the given production rules from the textbook.
 * Check out the 492p of textbook: A.2 SYTAX AND SEMANTICS OF C- */
typedef enum 
{ StmtNodeKind, ExpNodeKind, DeclNodeKind } NodeKind;

/* < Kinds of Statement >
 * - Selection Statement
 * - Return Statement 
 * - Iteration Statement
 * - Compound Statement 
 * - Expression Statement (it's gonna be reduced to below) */
typedef enum 
{ SelectionKind, ReturnKind, IterationKind, CompoundKind } StmtKind;

/* < Kinds of Expression >
 * - Assign Expression
 * - Simple Expression
 * - Additive Expression
 * - Operator (just an operator)
 * - Term (multiplying situation, for printing)
 * - Constant (just a number) 
 * - Identifier (just a name) 
 * - Type (integer or void) 
 * - Function Call (ex. output())
 * - Array Subscript (ex. arr[3]) */
typedef enum 
{ AssignKind, SimpExpKind, AddExpKind, OpKind, TermKind,
ConstKind, IdKind, TypeKind, FunCallKind, ArrSubKind } ExpKind;

/* < Kinds of Declaration > 
 * - Variable Declaration
 * -- Array Declaration (just for fancy printing) 
 * - Function Declaration
 * - Parameter Declaration
 * -- Array Parameter Declaration (just for fancy printing) */
typedef enum 
{ VarDeclKind, ArrayDeclKind, FunDeclKind, 
ParamKind, ArrayParamKind } DeclKind;

/* < Types > 
 * - void Type or int Type */
typedef enum 
{ Void, Integer } ExpType;

/* Number of maximum children */
#define MAXCHILDREN 4

/* Node structure for the parse tree */
typedef struct treeNode
{
    struct treeNode *child[MAXCHILDREN];
    struct treeNode *sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; DeclKind decl; } kind;
    union {
        TokenType op;
        int val;
        char *name;
    } attr;
    ExpType type; /* for type checking of exps */
} TreeNode;

/* Declaration of main parsing function
 * Note that the real implementation has been
 * done in 'tiny.y' file. It calls yyparse() */
TreeNode *parse(void);

/**************************************************/
/***********   Flags for tracing       ************/
/* Many of these flags are not used in this phase */
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;
#endif
