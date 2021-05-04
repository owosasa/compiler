#pragma once
#ifndef _PARSE_H_
#define _PARSE_H_
 
#include "GLOBALS.H"
#include "SCAN.H"

extern Token tokenSeq[CONSTANT];
extern int tokenNum;
extern int tokenBelongToLine[CONSTANT];
extern char TokenTypeMap[1 << 9][MAXTOKENLEN];

typedef enum {
	_STMT = NULLS + 1, _EXP
} NodeType;

typedef enum {
	_IF = _EXP + 1, _FOR, _WHILE, _RETURN, _ASSIGN,
	_FUNC, _DEFINEPARA, _MAIN, _DEFINEARRAY,
	_STRUCT, _SWITCH, _CASE, _BREAK, _DEFAULT
} StmtType;

typedef enum {
	_OP = _DEFAULT + 1, _CONST, _ID, _CITEARRAY
} ExpType;

typedef struct Node {
	struct Node *son[4];
	struct Node *bro;
	int LINE;
	NodeType nodeType;
	union {
		StmtType stmtType;
		ExpType expType;
	} detailedType;
	union {
		TokenType op;
		double value;
		char name[SMALLCONSTANT];
	} attribute;
	TokenType dataType;
} Node;

void PARSE();
void destryTree();

#endif
