/****************************************************/
/* File: util.c (20171643 Park Junhyeok)            */
/* Utility function implementation                  */
/* for the C-minus compiler                         */
/****************************************************/
/****************************************************/
/* The final version of my submission for the proje-*/
/* -ct phase #1 and #2, which are all completed. The*/
/* last modification has been done at 7th Dec, 2022 */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Number of tokens */
#define TOKEN_NUM 29

/* Token names corresponding to each enum number */
static char const *tokenStr[] =
{
    "ELSE", "IF", "INT", "RETURN", "VOID", "WHILE",
    "ID", "NUM", 
    "+", "-", "*", "/", 
    ">", ">=", "<", "<=", "==", "!=",
    "=", ";", ",", 
    "(", ")", "{", "}", "[", "]",
    "ERROR", "EOF"
};

/* Statement names corresponding to each enum number */
static char const *stmtStr[] = 
{
    "If", "Return", "While", "Compound Statement", "Simple Expression"
};

/* Declaration names corresponding to each enum number */
static char const *declStr[] =
{
    "Variable", "Array", "Function", "Parameter", "Array Parameter"
};

/* Expression types names corresponding to each enum number */
static char const *expTypeStr[] =
{
    "void", "int"
};

/* For printing the result of scanned token */
static char const *_token; 
/* For printing the result of scanned lexeme */
static char const *_lexeme;

/* Procedure printToken prints a token and its lexeme to 
 * the listing file. This gonna be used for error reporting
 * in this project phase #2 */
void printToken(TokenType token, const char *tokenString)
{
    _lexeme = tokenString;

    switch (token)
    {
    /* Book-keeping tokens */
    case ERROR: 

    /* Reserved words */
    case ELSE: case IF: case INT: case RETURN: case VOID: case WHILE:

    /* Multicharacter tokens */
    case ID: case NUM: 

    /* Special symbols */
    case PLUS: case MINUS: case TIMES: case OVER:
    case GT: case GTEQ: case LT: case LTEQ: case EQ: case NEQ:
    case ASSIGN: case SEMI: case COMMA:
    case LPAREN: case RPAREN: case LBRACE: case RBRACE: case LBRACK: case RBRACK: 
    _token = tokenStr[token - ELSE]; break;

    case ENDFILE:
    _token = tokenStr[TOKEN_NUM - 1]; break;

    /* This should never happen */
    default: fprintf(listing, "Unknown token: %d\n", token); exit(-1);
    }

    fprintf(listing, "\t\t\t%s\t\t\t%s\n", _token, _lexeme);
}


/* Function newStmtNode creates a new statement
 * node for syntax tree construction */
TreeNode *newStmtNode(StmtKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else {
        for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtNodeKind;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction */
TreeNode *newExpNode(ExpKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else {
        for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpNodeKind;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

/* Function newDeclNode creates a new declaration
 * node for syntax tree construction */
TreeNode *newDeclNode(DeclKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else {
        for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = DeclNodeKind;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string */
char *copyString(char *s)
{
    int n;
    char *t;
    if (s == NULL) return NULL;
    n = strlen(s) + 1;
    t = malloc(n);
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else strcpy(t, s);
    return t;
}

/* Function copyValue calculates the result value
 * of the passed string */
int copyValue(char *s)
{
    int ret;
    if (s == NULL) return -1;
    ret = atoi(s);
    return ret;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=4
#define UNINDENT indentno-=4

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
    int i;
    for (i = 0; i < indentno; i++)
        fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees */
void printTree(TreeNode *tree)
{
    int i;
    INDENT;
    while (tree != NULL) {
        printSpaces();
        if (tree->nodekind == StmtNodeKind)
        {
            switch (tree->kind.stmt) 
            {
            case SelectionKind: case ReturnKind: 
            case IterationKind: case CompoundKind:
            fprintf(listing, "%s\n", stmtStr[tree->kind.stmt]); break;

            default: fprintf(listing, "Unknown StmtNode kind\n"); break;
            }
        }
        else if (tree->nodekind == ExpNodeKind)
        {
            switch (tree->kind.exp) {
            case AssignKind:
            fprintf(listing, "Assign : %s\n", tokenStr[tree->attr.op - ELSE]); break;

            case SimpExpKind:
            fprintf(listing, "Simple Expression\n"); break;

            case AddExpKind:
            fprintf(listing, "Additive Expression\n"); break;

            case OpKind: 
            fprintf(listing, "Operator : %s\n", tokenStr[tree->attr.op - ELSE]); break;

            case TermKind: 
            fprintf(listing, "Term\n"); break;

            case ConstKind: 
            fprintf(listing, "Constant : %d\n", tree->attr.val); break;

            case IdKind: 
            fprintf(listing, "Variable : %s\n", tree->attr.name); break;

            case TypeKind: 
            fprintf(listing, "Type : %s\n", expTypeStr[tree->type]); break;

            case FunCallKind: 
            fprintf(listing, "Function Call : %s\n", tree->attr.name); break;

            case ArrSubKind: 
            fprintf(listing, "Array Subscript : %s\n", tree->attr.name); break;

            default: fprintf(listing, "Unknown ExpNode kind\n"); break;
            }
        }
        else if (tree->nodekind == DeclNodeKind)
        {
            switch (tree->kind.decl) 
            {
            case VarDeclKind: case ArrayDeclKind: case FunDeclKind:
            fprintf(listing, "%s Declare : %s\n", 
                declStr[tree->kind.decl], tree->attr.name); break;

            case ParamKind: case ArrayParamKind:
            fprintf(listing, "%s : %s\n", 
                declStr[tree->kind.decl], tree->attr.name); break;
            
            default: fprintf(listing, "Unknown DeclNode kind\n"); break;
            }
        }
        else fprintf(listing, "Unknown node kind\n");
        for (i = 0; i < MAXCHILDREN; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}
