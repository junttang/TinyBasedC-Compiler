/****************************************************/
/* File: scan.h (20171643 Park Junhyeok)            */
/* The scanner interface for the TINY compiler      */
/****************************************************/
/****************************************************/
/* The final version of my submission for the proje-*/
/* -ct phase #1 and #2, which are all completed. The*/
/* last modification has been done at 7th Dec, 2022 */
/****************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN+1];
/* tokenString array stores the lexeme of identifier token */
extern char idString[MAXTOKENLEN + 1];

/* In the project phase #2, the Bison requires the
 * 'TokenType' to be just an integer type 
 * This is also declared in 'global.h' header */
typedef int TokenType;

/* function getToken returns the 
 * next token in source file */
TokenType getToken(void);

#endif
