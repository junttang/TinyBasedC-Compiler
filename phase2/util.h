/****************************************************/
/* File: util.h (20171643 Park Junhyeok)            */
/* Utility functions for the C-minus compiler       */
/****************************************************/
/****************************************************/
/* The final version of my submission for the proje-*/
/* -ct phase #1 and #2, which are all completed. The*/
/* last modification has been done at 7th Dec, 2022 */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

/* Procedure printToken prints a token
 * and its lexeme to the listing file */
void printToken(TokenType, const char *);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction */
TreeNode *newStmtNode(StmtKind);

/* Function newExpNode creates a new expression
 * node for syntax tree construction */
TreeNode *newExpNode(ExpKind);

/* Function newDeclNode creates a new declaration
 * node for syntax tree construction */
TreeNode *newDeclNode(DeclKind);

/* Function copyString allocates and makes a new
 * copy of an existing string */
char *copyString(char *);

/* Function copyValue calculates the result value
 * of the passed string */
int copyValue(char *);

/* Procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees */
void printTree(TreeNode *);

#endif
