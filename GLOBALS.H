#ifndef _GLOBALS_H_
#define _GLOBALS_H_
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef PAUSE
#define PAUSE system("pause")

#define MAXTOKENLEN 40
#define CONSTANT 2333
#define SMALLCONSTANT 133
#endif

#define IFMS
#endif

#pragma warning(disable: 4996)
int isLetter(char letter);
int isNum(char num);


