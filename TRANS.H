#pragma once
#ifndef _TRANS_H_
#define _TRANS_H_
 
#include "GLOBALS.H"
#include "PARSE.H"

extern Node *syntaxTreeRoot;
extern char TokenTypeMap[1 << 9][MAXTOKENLEN];

typedef enum {
	PRINTF_ = _CITEARRAY + 1, SCANF_, IFFALSE_, ASSIGN_, ARRASSIGN_, LAB_, TIMES_, ADDS_, INC_, DEC_,
	MINUS_, CALLFUNC_, DEFINEFUNCS_, MAINS_, MAINE_, DEFINEFUNCE_, CODEBLOCKS_, CODEBLOCKE_,
	DEFINEPARAS_, IFS_, IFE_, WHILES_, WHILEE_, FORS_, FORE_, DIVS_,
	MODES_, IFEQUAL_, NOTEQUAL_, BIGGER_, BIGGEREQUAL_, SMALLER_, SMALLEREQUAL_, RETURNS_,
	JUMPS_, ARGS_, ARGSS_, FUNCRETURNS_, ADDNEXT_, MINUSNEXT_, FORCMP_
} opType;

typedef enum {
	EMPTYS_ = FORCMP_ + 1, CONSTS_, STRINGS_
} addressType;

typedef struct {
	addressType addressTypes;
	char name[SMALLCONSTANT];
} address;

typedef struct quaternion {
	opType opTypes;
	address addr[3];
	struct quaternion *next;
} quaternion;


void TRANS();
void destoryQuaternion();

#endif


