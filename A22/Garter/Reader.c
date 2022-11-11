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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

ReaderPointer readerCreate(gar_intg size, gar_intg increment, gar_intg mode) {
	ReaderPointer readerPointer;
	int i = 0;
	/* Defensive programming */
	if (size < 0 || increment < 0)
		return NULL;

	/* Adjust the values according to parameters */
	if (size == 0) {
		size = READER_DEFAULT_SIZE;
		increment = READER_DEFAULT_INCREMENT;
	}

	if (increment == 0)
		mode = MODE_FIXED;

	if (mode != MODE_FIXED && mode != MODE_ADDIT && mode != MODE_MULTI)
		return NULL;

	readerPointer = (ReaderPointer)calloc(1, sizeof(BufferReader));
	/* Defensive programming */
	if (!readerPointer)
		return NULL;

	readerPointer->content = (gar_char*)malloc(size);

	/* Defensive programming */
	if (!readerPointer->content) {
		free(readerPointer);
		return NULL;
	}

	/* Initialize the histogram */
	for (i = 0; i < NCHAR; i++)
	{
		readerPointer->histogram[i] = 0;
	}
	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;
	/* Initialize flags */
	readerPointer->flags = READER_DEFAULT_FLAG;
	/* The created flag must be signalized as EMP */
	readerPointer->flags = readerPointer->flags | READER_EMPTY_FLAG;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

ReaderPointer readerAddChar(ReaderPointer const readerPointer, gar_char ch) {
	gar_char* tempReader = NULL;
	gar_intg newSize = 0;
	/* Defensive programming */
	if (!readerPointer || ch > NCHAR || ch < 0)
		return NULL;
	/* Reset Realocation */
	readerPointer->flags = readerPointer->flags & (~READER_RELOCATION_FLAG);
	/* Test the inclusion of chars */
	if (readerPointer->position.wrte * (gar_intg)sizeof(gar_char) < readerPointer->size) {
		/* This buffer is NOT full */
		readerPointer->flags = readerPointer->flags & (~READER_FULL_FLAG);
	} else {
		/* Reset Full flag */
		readerPointer->flags = readerPointer->flags | READER_FULL_FLAG;

		switch (readerPointer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			/* Adjust new size */
			newSize = readerPointer->size + readerPointer->increment;
			break;
		case MODE_MULTI:
			/* Adjust new size */
			newSize = readerPointer->size * readerPointer->increment;
			break;
		default:
			return NULL;
		}
		/* Defensive programming */
		if (newSize > READER_MAX_SIZE || newSize < 0)
			return NULL;

		/* New reader allocation */
		tempReader = realloc(readerPointer->content, newSize);
		/* Defensive programming */
		if (!tempReader)
			return NULL;
		/* Check Relocation */
		if (readerPointer->content != tempReader) {
			readerPointer->flags = readerPointer->flags | READER_RELOCATION_FLAG;
		}

		/* Set the size and content of the reader pointer */
		readerPointer->size = newSize;
		readerPointer->content = tempReader;
	}
	/* Add the char */
	readerPointer->content[readerPointer->position.wrte++] = ch;
	/* Clear empty flag */
	readerPointer->flags = readerPointer->flags & (~READER_EMPTY_FLAG);
	/* Set full flag when full */
	if (readerPointer->position.wrte > readerPointer->size) {
		readerPointer->flags = readerPointer->flags | READER_FULL_FLAG;
	}
	/* Updates histogram */
	readerPointer->histogram[ch]++;
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_boln readerClear(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return GAR_FALSE;
	/* Adjust flags original */
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	readerPointer->position.wrte = 0;
	readerPointer->flags = READER_EMPTY_FLAG;
	return GAR_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_boln readerFree(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return GAR_FALSE;
	/* Free pointers */
	free(readerPointer->content);
	free(readerPointer);
	return GAR_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_boln readerIsFull(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return GAR_FALSE;
	/* Check flag if buffer is FUL */
	if ((readerPointer->flags & READER_FULL_FLAG) == READER_FULL_FLAG)
		return GAR_TRUE;

	return GAR_FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_boln readerIsEmpty(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return GAR_FALSE;
	/* Check flag if buffer is EMP */
	if ((readerPointer->flags & READER_EMPTY_FLAG) == READER_EMPTY_FLAG)
		return GAR_TRUE;

	return GAR_FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_boln readerSetMark(ReaderPointer const readerPointer, gar_intg mark) {
	/* Defensive programming */
	if (!readerPointer || mark < 0 || mark > readerPointer->position.wrte)
		return GAR_FALSE;
	/* Adjust mark */
	readerPointer->position.mark = mark;
	return GAR_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_intg readerPrint(ReaderPointer const readerPointer) {
	gar_intg cont = 0;
	gar_char c;
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;

	c = readerGetChar(readerPointer);
	/* Check flag if buffer EOB has achieved */
	while (cont < readerPointer->position.wrte || 
		(readerPointer->flags & (~READER_EOB_FLAG)) == READER_EOB_FLAG ) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_intg readerLoad(ReaderPointer const readerPointer, FILE* const fileDescriptor) {
	gar_intg size = 0;
	gar_char c;
	/* Defensive programming */
	if (!readerPointer || !fileDescriptor)
		return READER_ERROR;

	c = (gar_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	/* Defensive programming */
	if (size < 0)
		return READER_ERROR;

	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_boln readerRecover(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return GAR_FALSE;
	/* Recover positions */
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	return GAR_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_boln readerRetract(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer || readerPointer->position.read < 1)
		return GAR_FALSE;
	/* Retract (return 1 pos read) */
	readerPointer->position.read--;
	return GAR_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_boln readerRestore(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return GAR_FALSE;
	/* Restore positions (read/mark) */
	readerPointer->position.read = readerPointer->position.mark;
	return GAR_TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_char readerGetChar(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Check condition to read/wrte */
	if (readerPointer->position.read == readerPointer->position.wrte) {
		/* Set EOB flag */
		readerPointer->flags = readerPointer->flags | READER_EOB_FLAG;
		return READER_TERMINATOR;
	}
	/* Reset EOB flag */
	readerPointer->flags = readerPointer->flags & (~READER_EOB_FLAG);
	return readerPointer->content[readerPointer->position.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_char* readerGetContent(ReaderPointer const readerPointer, gar_intg pos) {
	/* Defensive programming */
	if (!readerPointer || pos < 0 || pos > readerPointer->position.wrte)
		return NULL;
	/* Return content (string) */
	return readerPointer->content + pos; /* NOTE: improper pointer arithmatic?*/
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_intg readerGetPosRead(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Return read */
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_intg readerGetPosWrte(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Return wrte */
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_intg readerGetPosMark(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Return mark */
	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_intg readerGetSize(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_intg readerGetInc(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Return increment */
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_intg readerGetMode(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Return mode */
	return readerPointer->mode;
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_byte readerGetFlags(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Return flags */
	return readerPointer->flags;
}


/*
***********************************************************
* Function name: readerShowStat
* Purpose: Returns the number of chars used.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
gar_intg readerShowStat(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* Updates the histogram */
	gar_intg i = 0;
	gar_intg total = 0;
	for (i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] > 0)
			total++;
	}

	return total;
}
