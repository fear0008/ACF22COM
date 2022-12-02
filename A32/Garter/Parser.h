/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
 _________________________________
|                                 |
| ........ GARTER LANGUAGE ...... |
|     __    __    __    __        |
|    /  \  /  \  /  \  /  \       |
| __/  __\/  __\/  __\/  __\__    |
| _/  /__/  /__/  /__/  /_____|   |
|  \_/ \   / \   / \   / \  \___  |
|       \_/   \_/   \_/   \___o_> |
|                                 |
| .. ALGONQUIN COLLEGE - 2022F .. |
|_________________________________|

*/

/*
************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
extern BufferReader* stringLiteralTable;
extern gar_intg line;
extern Token tokenizer();
extern gar_char* keywordTable[];
gar_intg syntaxErrorNumber = 0;

#define STR_LANGNAME	"Garter"
#define LANG_WRTE		"print"
#define LANG_READ		"input"
#define LANG_MAIN		"main"

/* Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_and,
	KW_or,
	KW_not,
	KW_def,
	KW_while,
	KW_if,
	KW_else,
	KW_return,
	KW_import,
	KW_none,
	KW_True,
	KW_False
};

/* Function definitions */
gar_void startParser();
gar_void matchToken(gar_intg, gar_intg);
gar_void syncErrorHandler(gar_intg);
gar_void printError();

/* TO_DO: Place ALL non-terminal function declarations */
gar_void program();
gar_void statement();
gar_void statements();
gar_void statementsPrime();
gar_void functionDeclaration();
gar_void functionInvocation();
gar_void optVariableList();
gar_void variableList();
gar_void variable();
gar_void variableListPrime();
gar_void outputStatement();
gar_void inputStatement();
gar_void assignmentStatement();
gar_void variableExpressions();
gar_void variableExpression();
gar_void variableExpressionsPrime();
gar_void arithmeticExpression();
gar_void unaryArithmeticExpression();
gar_void additiveArithmeticExpression();
gar_void multiplicativeArithmeticExpression();
gar_void primaryArithmeticExpression();
gar_void stringExpression();
gar_void conditionalExpressions();
gar_void conditionalExpression();
gar_void conditionalExpressionsPrime();
gar_void logicalAndExpression();
gar_void logicalOrExpression();
gar_void relationalExpression();
gar_void returnStatement();

#endif
