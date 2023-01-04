/****************************************************/
/* File: tiny.y (20171643 Park Junhyeok)            */
/* The TINY Yacc/Bison specification file           */
/* We just use tiny format to implement a syntax an-*/
/* -alyzer(parser) for the C-minus Language         */
/*                                                  */
/* Things that should be done in here               */
/*  (1) Token & Node identification                 */
/*      : Just as same as TINY Compiler. We only ne-*/
/*        -ed to change the enum type for tokens and*/
/*        enum type for nodes with adding decl case */
/*  (2) Construct the parsing table with Bison      */
/*      : By using the Bison grammar, we need to wr-*/
/*        -ite BNF grammars for the c-minus convent-*/
/*        -ion parsing (production rules & actions) */
/*  (3) Modify some parsing-related functions       */
/*      : We should modify parsing-related functions*/
/*        that is provided already in global.h, etc */
/*                                                  */
/*  *NOTE: 'parse' function will call 'yyparse' whi-*/
/*         -ch constructs a parse tree and return it*/
/****************************************************/
/****************************************************/
/* The final version of my submission for the proje-*/
/* -ct phase #1 and #2, which are all completed. The*/
/* last modification has been done at 7th Dec, 2022 */
/****************************************************/

%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
// #include "parse.h" (We do not use this header)

/* For use in this assignment */  
#define YYSTYPE TreeNode *

/* Variables for LALR(1) parsing */
static char *savedName;
static char *savedFuncName;
static int savedLineNo;
static int savedValue;

/* Stores syntax tree for later return */
static TreeNode *savedTree; 

int yyerror(char *);
static int yylex(void);
%}

/* Reserved Words */
%token ELSE IF INT RETURN VOID WHILE

/* Multicharacter Tokens */
%token ID NUM 

/* Special Symbols */
%token PLUS MINUS TIMES OVER GT GTEQ LT LTEQ EQ NEQ ASSIGN SEMI COMMA 
%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK 

/* Book-keeping Tokens */
%token ERROR

/* Left-Associative Appliance */
%left PLUS MINUS
%left TIMES OVER

/* Solution for a shift-reduce conflict
   when constructing a parsing table */
%nonassoc RPAREN 
%nonassoc ELSE

/* Production Rules in the 492p of textbook: 
   < A.2 SYTAX AND SEMANTICS OF C- >
   Below codes are just a representation of those rules */
%% 

/* program -> declaration_list */ 
program         : decl_list
                  {
                      savedTree = $1;
                  }
                ;

/* declaration_list -> declaration_list declaration | declaration */
decl_list       : decl_list decl
                  {
                      YYSTYPE t = $1;
                      if (t) 
                      {
                          while (t->sibling)
                              t = t->sibling;

                          t->sibling = $2;
                          $$ = $1;
                      } 
                      else
                          $$ = $2;
                  }
                | decl
                  {
                      $$ = $1;
                  }
                ;

/* declaration -> var-declaration | fun-declaration */
decl            : var_decl
                  {
                      $$ = $1;
                  }
                | fun_decl
                  {
                      $$ = $1;
                  }
                ;

/* var-declaration -> type-specifier ID; | type-specifier ID [NUM]; */ 
var_decl        : type ID 
                  {
                      savedName = copyString(idString);
                      savedLineNo = lineno;
                  }
                  SEMI
                  {
                      $$ = newDeclNode(VarDeclKind);
                      $$->lineno = savedLineNo;
                      $$->attr.name = savedName;
                      $$->child[0] = $1;
                      $$->type = $1->type;
                  }
                | type ID
                  {
                      savedName = copyString(idString);
                      savedLineNo = lineno;
                  }
                  LBRACK NUM
                  {
                      savedValue = copyValue(tokenString);
                  }
                  RBRACK SEMI
                  {
                      $$ = newDeclNode(ArrayDeclKind);
                      $$->lineno = savedLineNo;
                      $$->attr.name = savedName;
                      $$->child[0] = $1;
                      $$->child[1] = newExpNode(ConstKind);
                      $$->child[1]->attr.val = savedValue;
                      $$->type = $1->type;
                  }
                ;

/* type-specifier -> int | void */
type            : INT
                  {
                      $$ = newExpNode(TypeKind);
                      $$->type = Integer;
                  }
                | VOID
                  {
                      $$ = newExpNode(TypeKind);
                      $$->type = Void;
                  }
                ;

/* fun-declaration -> type-specifier ID (params) compound-stmt */
fun_decl        : type ID
                  {
                      savedFuncName = copyString(idString);
                      savedLineNo = lineno;
                  }
                  LPAREN params RPAREN comp_stmt
                  {
                      $$ = newDeclNode(FunDeclKind);
                      $$->lineno = savedLineNo;
                      $$->attr.name = savedFuncName;
                      $$->child[0] = $1;
                      $$->child[1] = $5;
                      $$->child[2] = $7;
                      $$->type = $1->type;
                  }
                ;

/* params -> param-list | void */
params          : param_list
                  {
                      $$ = $1;
                  }
                | VOID
                  {
                      $$ = NULL;
                  }
                ;

/* param-list -> param-list, param | param */
param_list      : param_list COMMA param
                  {
                      YYSTYPE t = $1;
                      if (t) 
                      {
                          while (t->sibling)
                              t = t->sibling;

                          t->sibling = $3;
                          $$ = $1;
                      } 
                      else 
                          $$ = $3;
                  }
                | param
                  {
                      $$ = $1;
                  }
                ;

/* param -> type-specifier ID | type-specifier ID [] */
param           : type ID
                  {
                      $$ = newDeclNode(ParamKind);
                      $$->attr.name = copyString(idString);
                      $$->lineno = lineno;
                      $$->child[0] = $1;
                      $$->type = $1->type;
                  }
                | type ID
                  {
                      savedName = copyString(idString);
                      savedLineNo = lineno;
                  }
                  LBRACK RBRACK
                  {
                      $$ = newDeclNode(ArrayParamKind);
                      $$->lineno = savedLineNo;
                      $$->attr.name = savedName;
                      $$->child[0] = $1;
                      $$->type = $1->type;
                  }
                ;

/* compound-stmt -> { local-declarations statement-list } */
comp_stmt       : LBRACE local_decl stmt_list RBRACE
                  {
                      $$ = newStmtNode(CompoundKind);
                      $$->child[0] = $2;
                      $$->child[1] = $3;
                  }
                ;

/* local-declarations -> local-declarations var-declaration | empty */
local_decl      : local_decl var_decl
                  {
                      YYSTYPE t = $1;
                      if (t) 
                      {
                          while (t->sibling)
                              t = t->sibling;
                          
                          t->sibling = $2;
                          $$ = $1;
                      } 
                      else 
                          $$ = $2;
                  }
                | /* Empty: Do nothing */
                  {
                      $$ = NULL;
                  }
                ;

/* statement-list -> statement-list statement | empty */
stmt_list       : stmt_list stmt
                  {
                      YYSTYPE t = $1;
                      if (t) 
                      {
                          while (t->sibling)
                              t = t->sibling;
                          
                          t->sibling = $2; 
                          $$ = $1;
                      } 
                      else 
                          $$ = $2;
                  }
                | /* Empty: Do nothing */
                  {
                      $$ = NULL;
                  }
                ;

/* statement -> expression-stmt | compound-stmt | selection-stmt
               | iteration-stmt | return-stmt */
stmt            : expr_stmt
                  {
                      $$ = $1;
                  }
                | comp_stmt
                  {
                      $$ = $1;
                  }
                | select_stmt
                  {
                      $$ = $1;
                  }
                | iter_stmt
                  {
                      $$ = $1;
                  }
                | return_stmt
                  {
                      $$ = $1;
                  }
                ;

/* expression-stmt -> expression; | ; */
expr_stmt       : expr SEMI
                  {
                      $$ = $1;
                  }
                | SEMI
                  {
                      $$ = NULL;
                  }
                ;

/* selection-stmt -> if (expression) statement
                    | if (expression) statement else statement */
select_stmt     : IF LPAREN expr RPAREN stmt
                  {
                      $$ = newStmtNode(SelectionKind);
                      $$->child[0] = $3;
                      $$->child[1] = $5;
                  }
                | IF LPAREN expr RPAREN stmt ELSE stmt
                  {
                      $$ = newStmtNode(SelectionKind);
                      $$->child[0] = $3;
                      $$->child[1] = $5;
                      $$->child[2] = $7;
                  }
                ;

/* iteration-stmt -> while (expression) statement */
iter_stmt       : WHILE LPAREN expr RPAREN stmt
                  {
                      $$ = newStmtNode(IterationKind);
                      $$->child[0] = $3;
                      $$->child[1] = $5;
                  }
                ;

/* return-stmt -> return; | return expression; */
return_stmt     : RETURN SEMI
                  {
                      $$ = newStmtNode(ReturnKind);
                  }
                | RETURN expr SEMI
                  {
                      $$ = newStmtNode(ReturnKind);
                      $$->child[0] = $2;
                  }
                ;

/* expression -> var = expression | simple-expression */
expr            : var ASSIGN expr
                  {
                      $$ = newExpNode(AssignKind);
                      $$->attr.op = ASSIGN;
                      $$->child[0] = $1;
                      $$->child[1] = $3;
                  }
                | simple_expr
                  {
                      $$ = $1;
                  }
                ;

/* var -> ID | ID[expression] */
var             : ID
                  {
                      $$ = newExpNode(IdKind);
                      $$->attr.name = copyString(idString);
                      $$->lineno = lineno;
                  }
                | ID
                  {
                      savedName = copyString(idString);
                      savedLineNo = lineno;
                  }
                  LBRACK expr RBRACK
                  {
                      $$ = newExpNode(ArrSubKind);
                      $$->lineno = savedLineNo;
                      $$->attr.name = savedName;
                      $$->child[0] = $4;
                  }
                ;

/* simple-expression -> additive-expression relop additive-expression
                       | additive-expression */
simple_expr     : add_expr relop add_expr
                  {
                      $$ = newExpNode(SimpExpKind);
                      $$->child[0] = $1;
                      $$->child[1] = $2;
                      $$->child[2] = $3;
                  }
                | add_expr
                  {
                      $$ = $1;
                  }
                ;

/* relop -> <= | < | > | >= | == | != */
relop           : LTEQ
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = LTEQ;
                  }
                | LT
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = LT; 
                  }
                | GT
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = GT;
                  }
                | GTEQ
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = GTEQ;
                  }
                | EQ
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = EQ;
                  }
                | NEQ
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = NEQ;
                  }
                ;

/* additive-expression -> additive-expression addop term | term */
add_expr        : add_expr addop term
                  {
                      $$ = newExpNode(AddExpKind);
                      $$->child[0] = $1;
                      $$->child[1] = $2;
                      $$->child[2] = $3;
                  }
                | term
                  {
                      $$ = $1;
                  }
                ;

/* addop -> + | - */
addop           : PLUS
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = PLUS;
                  }
                | MINUS
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = MINUS;
                  }
                ;

/* term -> term mulop factor | factor */
term            : term mulop factor
                  {
                      $$ = newExpNode(TermKind);
                      $$->child[0] = $1;
                      $$->child[1] = $2;
                      $$->child[2] = $3;
                  }
                | factor
                  {
                      $$ = $1;
                  }
                ;

/* mulop -> * | / */
mulop           : TIMES
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = TIMES;
                  }
                | OVER
                  {
                      $$ = newExpNode(OpKind);
                      $$->attr.op = OVER;
                  }
                ;

/* factor -> { expression } | var | call | NUM */
factor          : LPAREN expr RPAREN
                  {
                      $$ = $2;
                  }
                | var
                  {
                      $$ = $1;
                  }
                | call
                  {
                      $$ = $1; 
                  }
                | NUM
                  {
                      $$ = newExpNode(ConstKind);
                      $$->attr.val = copyValue(tokenString);
                  }
                ;

/* call -> ID (args) */
call            : ID
                  {
                      savedName = copyString(idString);
                      savedLineNo = lineno;
                  }
                  LPAREN args RPAREN
                  {
                      $$ = newExpNode(FunCallKind);
                      $$->lineno = savedLineNo;
                      $$->attr.name = savedName;
                      $$->child[0] = $4;
                  }
                ;

/* args -> arg-list | empty */
args            : arg_list
                  {
                      $$ = $1;
                  }
                | /* Empty: Do nothing */
                  {
                      $$ = NULL;
                  }
                ;

/* arg-list -> arg-list, expression | expression */ 
arg_list        : arg_list COMMA expr
                  {
                      YYSTYPE t = $1;
                      if (t) {
                          while (t->sibling)
                              t = t->sibling;
         
                          t->sibling = $3;
                          $$ = $1;
                      } 
                      else
                          $$ = $3;
                  }
                | expr
                  {
                      $$ = $1;
                  }
                ;

%%

int yyerror(char *message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner */
static int yylex(void)
{ return getToken(); }

TreeNode *parse(void)
{ yyparse();
  return savedTree;
}
