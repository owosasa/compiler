#pragma once
#ifndef _SCAN_H_
#define _SCAN_H_
#include "GLOBALS.H"

typedef enum {
	/*    int  float  char  double  void  main  if  else  while  printf  scanf  for  */
	INT = 255, FLOAT, CHAR, DOUBLE, VOID, MAIN, IF, ELSE, WHILE, PRINTF, SCANF, FOR,
	/*  =     +     ++      -      --      *       /     %  */
	EQUAL, PLUS, PPLUS, MINUS, MMINUS, TIMES, DIVIDE, MODE,
	/*    <          <=       >         >=      !=     ==  */
	SMALLER, SMALLEREQU, BIGGER, BIGGEREQU, NOTEQU, IFEQU,
	/* {  }   [    ]   (   )  */
	LB, RB, LSB, RSB, LP, RP,
	/*  :          ;      ,    .  */
	COLON, SEMICOLON, COMMA, DOT,
	/*            //   return */
	NUM, ID, COMMENT, RETURN,
	/* auto break  case  const  continue  */
	AUTO, BREAK, CASE, CONST, CONTINUE,
	/* struct default switch  */
	STRUCT, DEFAULT, SWITCH,
	/* &   |   !  */
	AND, XOR, NOT,

	ERROR, NULLS
} TokenType;

typedef struct {
	TokenType tokenType;
	char tokenVal[MAXTOKENLEN];
	double numVal;
} Token;

void SCAN(char[]);

#endif





