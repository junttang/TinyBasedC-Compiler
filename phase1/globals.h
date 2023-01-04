/****************************************************/
/* File: globals.h (20171643 Park Junhyeok)         */
/* Global types and vars for C-minus compiler       */
/* must come before other include files             */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXTOKENLEN = Maximum length of tokens */
/* This macro is defined in scan.h also.  */
/* Thus, don't include scan.h in any needed files */
#define MAXTOKENLEN 40

/* MAXRESERVED = The number of reserved words */
#define MAXRESERVED 6

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

extern FILE* source;			/* Source code text file */
extern FILE* listing;			/* Listing output text file */
extern FILE* code;				/* Code text file for TM simulator */

extern int lineno;				/* Source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/* These codes are not used in this project phase */
/**************************************************/

typedef enum { StmtK, ExpK } NodeKind;
typedef enum { IfK, RepeatK, AssignK, ReadK, WriteK } StmtKind;
typedef enum { OpK, ConstK, IdK } ExpKind;

/* ExpType is used for type checking */
typedef enum { Void, Integer, Boolean } ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
{
	struct treeNode * child[MAXCHILDREN];
	struct treeNode * sibling;
	int lineno;
	NodeKind nodekind;
	union { StmtKind stmt; ExpKind exp; } kind;
	union {
		TokenType op;
		int val;
		char * name;
	} attr;
	ExpType type; /* for type checking of exps */
} TreeNode;

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
