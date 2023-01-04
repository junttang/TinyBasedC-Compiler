/****************************************************/
/* File: util.c (20171643 Park Junhyeok)            */
/* Utility function implementation                  */
/* for the C-minus compiler                         */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Token names corresponding to each enum(token) number */
static char const* tokenStr[] =
{
	"EOF", "ERROR", 
	"ELSE", "IF", "INT", "RETURN", "VOID", "WHILE",
	"ID", "NUM", 
	"+", "-", "*", "/", 
	">", ">=", "<", "<=", "==", "!=",
	"=", ";", ",", 
	"(", ")", "{", "}", "[", "]"
};

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 * (This procedure is the only one that
 * is used in this project phase(lex analyze))
*/
void printToken(TokenType token, const char* tokenString)
{
	char* _token; char* _lexeme = tokenString;

	switch (token)
	{
	/* Book-keeping tokens */
	case ENDFILE: case ERROR: 

	/* Reserved words */
	case ELSE: case IF: case INT: case RETURN: case VOID: case WHILE:

	/* Multicharacter tokens */
	case ID: case NUM: 

	/* Special symbols */
	case PLUS: case MINUS: case TIMES: case OVER:
	case GT: case GTEQ: case LT: case LTEQ: case EQ: case NEQ:
	case ASSIGN: case SEMI: case COMMA:
	case LPAREN: case RPAREN: case LBRACE: case RBRACE: case LBRACK: case RBRACK: 
	_token = tokenStr[token]; break;

	/* This should never happen */
	default: fprintf(listing, "Unknown token: %d\n", token); exit(-1);
	}

	fprintf(listing, "\t\t\t%s\t\t\t%s\n", _token, _lexeme);
}


/* All codes below are not used in this project phase */
/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
	}
	return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{
	int n;
	char * t;
	if (s == NULL) return NULL;
	n = strlen(s) + 1;
	t = malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else strcpy(t, s);
	return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
	int i;
	for (i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode * tree)
{
	int i;
	INDENT;
	while (tree != NULL) {
		printSpaces();
		if (tree->nodekind == StmtK)
		{
			switch (tree->kind.stmt) {
			case IfK:
				fprintf(listing, "If\n");
				break;
			case RepeatK:
				fprintf(listing, "Repeat\n");
				break;
			case AssignK:
				fprintf(listing, "Assign to: %s\n", tree->attr.name);
				break;
			case ReadK:
				fprintf(listing, "Read: %s\n", tree->attr.name);
				break;
			case WriteK:
				fprintf(listing, "Write\n");
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK)
		{
			switch (tree->kind.exp) {
			case OpK:
				fprintf(listing, "Op: ");
				printToken(tree->attr.op, "\0");
				break;
			case ConstK:
				fprintf(listing, "Const: %d\n", tree->attr.val);
				break;
			case IdK:
				fprintf(listing, "Id: %s\n", tree->attr.name);
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else fprintf(listing, "Unknown node kind\n");
		for (i = 0; i < MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}
