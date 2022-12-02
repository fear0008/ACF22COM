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
* File name: mainBuffer.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* This is the function to start the parser - check your program definition */

gar_void startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
gar_void matchToken(gar_intg tokenCode, gar_intg tokenAttribute) {
	gar_intg matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* This is the function to handler error - adjust basically datatypes */
gar_void syncErrorHandler(gar_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* This is the function to error printing - adjust basically datatypes */
gar_void printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case VNID_T:
		printf("VNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);;
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case ASS_OP_T:
		printf("ASS_OP_T\t\t=\n");
		break;
	case ART_OP_T:
		printf("ART_OP_T\t\t%d\n", t.attribute.arithmeticOperator);
		break;
	case REL_OP_T:
		printf("REL_OP_T\t\t%d\n", t.attribute.relationalOperator);
		break;
	case LOG_OP_T:
		printf("LOG_OP_T\t\t%d\n", t.attribute.logicalOperator);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case COL_T:
		printf("COL_T\n");
		break;
	case QUE_T:
		printf("QUE_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {MNID_T (main&)}.
 ***********************************************************
 */
gar_void program() {
	switch (lookahead.code) {
	case SEOF_T:
		; // Empty
		break;
	default:
		statements();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
gar_void statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
gar_void statement() {
	switch (lookahead.code) {
	case VNID_T:
		assignmentStatement();
		break;
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_def:
			functionDeclaration();
			break;
		case KW_while:
			matchToken(KW_T, KW_while);
			conditionalExpressions();
			break;
		case KW_if:
			matchToken(KW_T, KW_if);
			conditionalExpressions();
			break;
		case KW_else:
			matchToken(KW_T, KW_else);
			matchToken(COL_T, NO_ATTR);
			break;
		case KW_return:
			returnStatement();
			break;
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 5) == 0) {
			outputStatement();
		}
		else if (strncmp(lookahead.attribute.idLexeme, LANG_READ, 5) == 0) {
			inputStatement();
		}
		else {
			functionInvocation();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
gar_void statementsPrime() {
	switch (lookahead.code) {
	case MNID_T:
	case VNID_T:
	case KW_T:
		statements();
	default:
		; //empty string
	}
}


/*
************************************************************
* Function Declaration
************************************************************
*/
gar_void functionDeclaration() {
	matchToken(KW_T, KW_def);
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	optVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(COL_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Function declaration parsed");
}

/*
************************************************************
* Function Invocation
************************************************************
*/
gar_void functionInvocation() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	optVariableList();
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Function invocation parsed");
}


/*
************************************************************
* Optional Variable List
************************************************************
*/
gar_void optVariableList() {
	switch (lookahead.code) {
	case VNID_T:
	case STR_T:
	case INL_T:
		variableList();
	default:
		; //empty string
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List parsed");
}

/*
************************************************************
* Variable List
************************************************************
*/
gar_void variableList() {
	variable();
	variableListPrime();
	printf("%s%s\n", STR_LANGNAME, ": Variable List parsed");
}

/*
************************************************************
* Variable
************************************************************
*/
gar_void variable() {
	switch (lookahead.code) {
	case VNID_T:
		matchToken(VNID_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	default:
		printError();
	}
 	printf("%s%s\n", STR_LANGNAME, ": Variable parsed");
}

/*
************************************************************
* Variable List Prime
************************************************************
*/
gar_void variableListPrime() {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		variableList();
	default:
		; // empty string
	}
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
gar_void outputStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	optVariableList();
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
************************************************************
* Input Statement
************************************************************
*/
gar_void inputStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	matchToken(VNID_T, NO_ATTR);
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
}

/*
************************************************************
* Assignment Statement
************************************************************
*/
gar_void assignmentStatement() {
	matchToken(VNID_T, NO_ATTR);
	matchToken(ASS_OP_T, NO_ATTR);
	variableExpressions();
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}

/*
************************************************************
* Variable Expressions
************************************************************
*/
gar_void variableExpressions() {
	variableExpression();
	variableExpressionsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Variable expressions parsed");
}

/*
************************************************************
* Variable Expression
************************************************************
*/
gar_void variableExpression() {
	switch (lookahead.code) {
	case VNID_T:
	case INL_T:
	case ART_OP_T:
		arithmeticExpression();
		break;
	case STR_T:
		stringExpression();
		break;
	case MNID_T:
		functionInvocation();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Variable expression parsed");
}

/*
************************************************************
* Variable Expressions Prime
************************************************************
*/
gar_void variableExpressionsPrime() {
	switch (lookahead.code) {
	case ART_OP_T:
		variableExpressions();
	default:
		; // empty string
	}
}

/*
************************************************************
* Arithmetic Expression
************************************************************
*/
gar_void arithmeticExpression() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arithmeticOperator) {
		case OP_ADD:
		case OP_SUB:
			unaryArithmeticExpression();
			break;
		case OP_MUL:
		case OP_DIV:
			multiplicativeArithmeticExpression();
		}
		break;
	case VNID_T:
	case INL_T:
		primaryArithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}

/*
************************************************************
* Unary Arithmetic Expression
************************************************************
*/
gar_void unaryArithmeticExpression() {
	matchToken(ART_OP_T, NO_ATTR);
	switch (lookahead.code) {
	case INL_T:
	case VNID_T:
		primaryArithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Unary arithmetic expression parsed");
}

/*
************************************************************
* Additive Arithmetic Expression
************************************************************
*/
gar_void additiveArithmeticExpression() {
	switch (lookahead.code) {
	case INL_T:
	case VNID_T:
		multiplicativeArithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Additive arithmetic expression parsed");
}

/*
************************************************************
* Multiplicative Arithmetic Expression
************************************************************
*/
gar_void multiplicativeArithmeticExpression() {
	matchToken(ART_OP_T, NO_ATTR);
	switch (lookahead.code) {
	case INL_T:
	case VNID_T:
		primaryArithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Multiplicative arithmetic expression parsed");
}

/*
************************************************************
* Primary Arithmetic Expression
************************************************************
*/
gar_void primaryArithmeticExpression() {
	switch (lookahead.code) {
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case VNID_T:
		matchToken(VNID_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Primary arithmetic expression parsed");
}

/*
************************************************************
* String Expression
************************************************************
*/
gar_void stringExpression() {
	matchToken(STR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": String expression parsed");
}

/*
************************************************************
* Conditional Expressions
************************************************************
*/
gar_void conditionalExpressions() {
	conditionalExpression();
	conditionalExpressionsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Conditional expressions parsed");
}

/*
************************************************************
* Conditional Expression
************************************************************
*/
gar_void conditionalExpression() {
	switch (lookahead.code) {
	case VNID_T:
		matchToken(VNID_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	default:
		printError();
	}
	relationalExpression();
	printf("%s%s\n", STR_LANGNAME, ": Conditional expression parsed");
}

/*
************************************************************
* Conditional Expressions Prime
************************************************************
*/
 gar_void conditionalExpressionsPrime() {
	switch (lookahead.code) {
		case COL_T:
			matchToken(COL_T, NO_ATTR);
			break;
		case KW_T:
			switch (lookahead.attribute.keywordIndex) {
			case KW_and:
				logicalAndExpression();
				break;
			case KW_or:
				logicalOrExpression();
				break;
			}
			conditionalExpressions();
			break;
	default:
		; // empty string
	}
}

/*
************************************************************
* Logical AND Expression
************************************************************
*/
gar_void logicalAndExpression() {
	matchToken(KW_T, KW_and);
	printf("%s%s\n", STR_LANGNAME, ": Logical AND expression parsed");
}

/*
************************************************************
* Logical OR Expression
************************************************************
*/
gar_void logicalOrExpression() {
	matchToken(KW_T, KW_or);
	printf("%s%s\n", STR_LANGNAME, ": Logical OR expression parsed");
}

/*
************************************************************
* Relational Expression
************************************************************
*/
gar_void relationalExpression() {
	switch (lookahead.code) {
		case REL_OP_T:
			matchToken(REL_OP_T, NO_ATTR);
			switch (lookahead.code) {
			case VNID_T:
				matchToken(VNID_T, NO_ATTR);
				break;
			case INL_T:
				matchToken(INL_T, NO_ATTR);
				break;
			default:
				printError();
			}
			break;
		default:
			printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Relational expression parsed");
}

/*
************************************************************
* Return Statement
************************************************************
*/
gar_void returnStatement() {
	matchToken(KW_T, KW_return);
	matchToken(LPR_T, NO_ATTR);
	variableList();
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Return statement parsed");
}