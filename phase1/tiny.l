/****************************************************/
/* File: tiny.l (20171643 Park Junhyeok)            */
/* Lex specification for C-minus                    */
/* We just use tiny format to implement a lexical a-*/
/* -nalyzer(scanner) for the C-minus Language       */
/*                                                  */
/* Things that should be done in here               */
/*  (1) Token identification                        */
/*      : Just as same as TINY Compiler. We only ne-*/
/*        -ed to change the enum type for tokens    */
/*  (2) Comments error handling                     */
/*      : This is the only lexcial error that can be*/
/*        detected in this phase(lexical analysis)  */
/*  (3) Add 'yywrap' func in order to avoid error   */
/*                                                  */
/*  *NOTE: We don't have to check the combination of*/
/*         ID and NUM since this type of error will */
/*         be detected in the next 'parsing' phase. */
/*    - ex) 123hello, x1, a123bc (wrong identifier) */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
// #include "scan.h" (We do not use this header)

#define AGAIN (EOF - 1)
#define PASS  (EOF - 2)

/* Lexeme of Identifier or Reserved word */
char tokenString[MAXTOKENLEN+1];		// 40 + 1

/* Comment Error Handling Function */
TokenType commentError(void);			
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"else"          {return ELSE;}
"if"            {return IF;}
"int"           {return INT;}
"return"        {return RETURN;}
"void"          {return VOID;}
"while"         {return WHILE;}

"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}

">"             {return GT;}
">="            {return GTEQ;}
"<"             {return LT;}
"<="            {return LTEQ;}
"=="            {return EQ;}
"!="            {return NEQ;}

"="             {return ASSIGN;}
";"             {return SEMI;}
","             {return COMMA;}

"("             {return LPAREN;}
")"             {return RPAREN;}
"{"             {return LBRACE;}
"}"             {return RBRACE;}
"["             {return LBRACK;}
"]"             {return RBRACK;}

"/*"            { 
                char c;
				/* Finite Automata for validation of comments */
				for ( ; ; ) {		// BIG for
					c = input();

					switch(c) {
					case EOF: return commentError();	// Fail
					case '*':							// '*' Situation
						for ( ; ; ) {
							c = input();

							switch(c) {
							case '/': c = PASS; break;		 // Pass
							case EOF: return commentError(); // Fail
							case '*': break;				 // Iterate
							case '\n': lineno++;			 // Line Increment
							default: c = AGAIN; break;		 // Go to 'BIG for'
							}
							if (c == AGAIN || c == PASS) break;
						}
						break;

					case '\n': lineno++;				// Line Increment
					default: break;						// Iterate 
					}

					if (c == PASS) break;	// Pass
				}
                }

<<EOF>>         {return ENDFILE;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
.				{return ERROR;}

%%

TokenType getToken(void)
{ 
	static int firstTime = TRUE;
	TokenType currentToken;

	if (firstTime)
	{ 
		firstTime = FALSE;
		lineno++;
		yyin = source;
		yyout = listing;
	}

	currentToken = yylex();
	strncpy(tokenString,yytext,MAXTOKENLEN);
  
	if (TraceScan) {
		fprintf(listing,"\t%d ",lineno);
		printToken(currentToken,tokenString);
	}
  
	return currentToken;
}

int yywrap(void) 
{
	/* The procedure for enabling lex to read EOF correctly */
}

TokenType commentError(void) 
{	/* Comments error handling function */
	strcpy(yytext, "Comment Error");
	return ERROR;
}