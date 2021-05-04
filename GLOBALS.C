#pragma once
#include "GLOBALS.H"
 
int isLetter(char letter) { return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || (letter == '_'); }

int isNum(char num) { return (num >= '0' && num <= '9'); }


