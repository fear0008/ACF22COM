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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR (-1)						/* General error message */
#define READER_TERMINATOR '\0'					/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for GARTER */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */
#define READER_FULL_FLAG 0x08		/* (0000.1000)_2 = (008)_10 BIT 3: FUL = Full */
#define READER_EMPTY_FLAG 0x04		/* (0000.0100)_2 = (004)_10 BIT 2: EMP: Empty */
#define READER_RELOCATION_FLAG 0x02	/* (0000.0010)_2 = (002)_10 BIT 1: REL = Relocation */
#define READER_EOB_FLAG	0x01		/* (0000.0001)_2 = (001)_10 BIT 0: END = EndOfBuffer */

#define NCHAR				128			/* Chars from 0 to 127 */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* Adjust datatypes */

/* Offset declaration */
typedef struct position {
	gar_intg mark;			/* the offset to the mark position (in chars) */
	gar_intg read;			/* the offset to the get a char position (in chars) */
	gar_intg wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	gar_char*	content;			/* pointer to the beginning of character array (character buffer) */
	gar_intg	size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	gar_intg	increment;			/* character array increment factor */
	gar_intg	mode;				/* operational mode indicator */
	gar_byte	flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;			/* Offset / position field */
	gar_intg	histogram[NCHAR];	/* Statistics of chars */
} BufferReader, * ReaderPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
ReaderPointer	readerCreate		(gar_intg, gar_intg, gar_intg);
ReaderPointer	readerAddChar		(ReaderPointer const, gar_char);
gar_boln		readerClear		    (ReaderPointer const);
gar_boln		readerFree		    (ReaderPointer const);
gar_boln		readerIsFull		(ReaderPointer const);
gar_boln		readerIsEmpty		(ReaderPointer const);
gar_boln		readerSetMark		(ReaderPointer const, gar_intg);
gar_intg		readerPrint		    (ReaderPointer const);
gar_intg		readerLoad			(ReaderPointer const, FILE* const);
gar_boln		readerRecover		(ReaderPointer const);
gar_boln		readerRetract		(ReaderPointer const);
gar_boln		readerRestore		(ReaderPointer const);
/* Getters */
gar_char		readerGetChar		(ReaderPointer const);
gar_char*		readerGetContent	(ReaderPointer const, gar_intg);
gar_intg		readerGetPosRead	(ReaderPointer const);
gar_intg		readerGetPosWrte	(ReaderPointer const);
gar_intg		readerGetPosMark	(ReaderPointer const);
gar_intg		readerGetSize		(ReaderPointer const);
gar_intg		readerGetInc		(ReaderPointer const);
gar_intg		readerGetMode		(ReaderPointer const);
gar_byte		readerGetFlags		(ReaderPointer const);
gar_intg		readerShowStat		(ReaderPointer const);

#endif
