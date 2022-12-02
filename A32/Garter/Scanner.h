/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Jul 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: '(' ) */
	VNID_T,		/*  2: Variable name identifier token */
	STR_T,		/*  3: String literal token */
	INL_T,		/*  4: Integer literal token */
	KW_T,		/*  5: Keyword token */
	ASS_OP_T,	/*  6: Assignment operator token */
	ART_OP_T,	/*  7: Arithmetic operator token */
	REL_OP_T,	/*  8: Relational operator token */
	LOG_OP_T,	/*  9: Logical operator token */
	LPR_T,		/* 10: Left parenthesis token */
	RPR_T,		/* 11: Right parenthesis token */
	COM_T,		/* 12: Comma token */
	COL_T,		/* 13: Colon token */
	QUE_T,		/* 14: Question mark token (ternary operator) */
	EOS_T,		/* 15: End of statement */
	RTE_T,		/* 16: Run-time error token */
	SEOF_T		/* 17: Source end-of-file token */
};

/* Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT, OP_TER } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	gar_intg codeType;					/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	gar_intg intValue;					/* integer literal attribute (value) */
	gar_intg keywordIndex;				/* keyword index in the keyword table */
	gar_intg contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	gar_real floatValue;				/* floating-point literal attribute (value) */
	gar_char idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	gar_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	gar_byte flags;			/* Flags information */
	union {
		gar_intg intValue;				/* Integer value */
		gar_char* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	gar_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';', 
 *  white space, #comment\n , ',' , ';' , '-' , '+' , '*' , '/', # , 
 *  .&., .|. , .!. , SEOF.
 */

/* */
#define MNIDPREFIX '('

/* Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '\"'

/* Error states and illegal state */
#define FS		100		/* Final / Illegal state */
#define ESNR	9		/* Error state with no retract */
#define ESWR	10		/* Error state with retract */

 /* State transition table definition */
#define TABLE_COLUMNS 6

/* Transition table - type of states defined in separate table */
static gar_intg transitionTable[][TABLE_COLUMNS] = {
/*   [A-z] , [0-9],    _,    ", SEOF, other
	   L(0),  D(1), U(2), Q(3), E(4),  O(5) */
	{     2,	 7,	   1,    5, ESWR, ESNR}, //  S0: NOAS
	{     1,     1,    1,	 3, ESWR,    3}, //  S1: NOAS
	{     2,     1,    1,    4, ESWR,    4}, //  S2: NOAS
	{    FS,    FS,   FS,   FS,   FS,   FS}, //  S3: ASWR (MNID/VNID)
	{    FS,	FS,	  FS,	FS,	  FS,	FS}, //  S4: ASWR (KEY-MNID/VNID)
	{     5,     5,    5,    6, ESWR,    5}, //  S5: NOAS
	{    FS,    FS,   FS,   FS,   FS,   FS}, //  S6: ASNR (SL)
	{     8,     7,    8,	 8, ESWR,    8}, //  S7: NOAS
	{	 FS,	FS,	  FS,	FS,	  FS,	FS}, //  S8: ASWR (IL)
	{	 FS,	FS,	  FS,	FS,	  FS,	FS}, //  S9: ASNR (Err 1)
	{	 FS,	FS,	  FS,	FS,   FS,	FS}  // S10: ASWR (Err 2)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* Define list of acceptable states */
static gar_intg stateType[] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	NOFS, /* 02 */
	FSNR, /* 03 (VID) */
	FSNR, /* 04 (KEY-VID) */
	NOFS, /* 05 */
	FSNR, /* 06 (SL) */
	NOFS, /* 07 */
	FSWR, /* 08 (IL) */
	FSNR, /* 09 (Err1 - no retract) */
	FSWR  /* 10 (Err2 - retract) */
};

/*
-------------------------------------------------
Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
gar_intg startScanner(ReaderPointer psc_buf);
static gar_intg nextClass(gar_char c);				/* character class function */
static gar_intg nextState(gar_intg, gar_char);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(gar_char* lexeme);

/* Declare accepting states functions */
Token funcSL	(gar_char lexeme[]);
Token funcIL	(gar_char lexeme[]);
Token funcID	(gar_char lexeme[]);
Token funcKEY	(gar_char lexeme[]);
Token funcErr	(gar_char lexeme[]);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -		  [00] */
	NULL,		/* -		  [01] */
	NULL,		/* -		  [02] */
	funcID,		/* V/MNID	  [03] */
	funcID,		/* KEY-M/VNID [04] */
	NULL,		/* -		  [05] */
	funcSL,		/* SL		  [06] */
	NULL,		/* -		  [07] */
	funcIL,		/* IL		  [08] */
	funcErr,	/* ERR1 ESNR  [09] */
	funcErr		/* ERR2 ESWR  [10] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* Define the number of Keywords from the language */
#define KWT_SIZE 12

/* Define the list of keywords */
static gar_char* keywordTable[KWT_SIZE] = {
	"and",
	"or",
	"not",
	"def",
	"while",
	"if",
	"else",
	"return",
	"import",
	"none",
	"True",
	"False"
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	gar_char indentationCharType;
	gar_intg indentationCurrentPos;
	/* Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

#endif
