#pragma once
#include "SCAN.H"
static char DEBUGPATH[] = "..\\compiler\\What\\DEBUG\\DEMO.txt";

static void initTokenTypeMap();
static void readNextLine(FILE *);
static Token getToken();
static int stateTransition(int, int);
static int isKeyWord(char[]);
static void getTokenVal(char *, int, int);
static double stringToNum(char[]);
static void printfToken();

static char keyWords[][MAXTOKENLEN] = {
	"main",
	"void",
	"char",
	"int",
	"float",
	"double",
	"if",
	"else",
	"while",
	"for",
	"break",
	"continue",
	"switch",
	"default",
	"case",
	"struct",
	"return",
	"printf",
	"scanf"
};
static TokenType keyWordsMap[] = {
	MAIN,
	VOID,
	CHAR,
	INT,
	FLOAT,
	DOUBLE,
	IF,
	ELSE,
	WHILE,
	FOR,
	BREAK,
	CONTINUE,
	SWITCH,
	DEFAULT,
	CASE,
	STRUCT,
	RETURN,
	PRINTF,
	SCANF
};
static int stateTransitionTable[][MAXTOKENLEN] = { 
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 2, 0, 0, 5, 0, 6, 0,10, 8,12, 0, 0, 0 },
	{ 2, 3,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 4, 0,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 5, 0, 0, 5,-1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0,-1, 7, 0, 0, 0, 0, 0 },
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ 0, 0, 0, 0, 0, 0,-1, 9, 0, 0, 0, 0, 0 },
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ 0, 0, 0, 0, 0, 0,-1,11, 0, 0, 0, 0, 0 },
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0,13,-1, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,14,15 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,14,15 },
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 } 
};
char TokenTypeMap[1 << 9][MAXTOKENLEN];

Token tokenSeq[CONSTANT];
int tokenNum;
int tokenBelongToLine[CONSTANT];
static char charBuf[CONSTANT];
static int currentInd;
static int currentLine;

void SCAN(char FILEPATH[]) {
	FILE *source = fopen(FILEPATH, "r");
	if (source == NULL) 
		return;
#ifdef IFMS
	printf("%s\n", "OPEN SUCCESS");
#endif
	while (!feof(source)) {
		currentInd = 0;
		readNextLine(source), currentLine += 1;
		int charBufLen = strlen(charBuf);
		if (charBufLen == 0) 
			continue;
		while (currentInd < charBufLen) {
			tokenSeq[tokenNum] = getToken();
			tokenBelongToLine[tokenNum] = currentLine;
			if (tokenSeq[tokenNum].tokenType != NULLS) tokenNum += 1;
		}
	}
#ifdef IFMS
	printfToken();
#endif
	fclose(source);
}

static void readNextLine(FILE *source) { 
	if (fgets(charBuf, CONSTANT, source) == NULL)
		memset(charBuf, 0, sizeof charBuf);
}

static Token getToken() { 
	int lastInd = currentInd; Token res; res.tokenType = NULLS;
	char currentChar = charBuf[currentInd]; 
	if (currentChar == ' ' || currentChar == '\n' || currentChar == '\r' || currentChar == '\t' || currentChar == '\v' || currentChar == '\f') {
		currentInd++;
		return res;
	}
	switch (currentChar) {
		case '!': {
			if (charBuf[currentInd + 1] == '=')
				res.tokenType = NOTEQU, currentInd += 2;
			break;
		}
		case '+': {
			if (charBuf[currentInd + 1] == '+')
				res.tokenType = PPLUS, currentInd += 2;
			else
				res.tokenType = PLUS, currentInd += 1;
			break;
		}
		/*case '-': {
			if (charBuf[currentInd + 1] == '-')
				res.tokenType = MMINUS, currentInd += 2;
			else
				res.tokenType = MINUS, currentInd += 1;
			break;
		}*/
		case '&': {
			if (charBuf[currentInd + 1] == '&')
				res.tokenType = AND, currentInd += 2;
			else
				printf("Ubdefined char on %d line %d ind\n", currentInd, charBuf[currentInd + 1]), PAUSE, exit(0);
			break;
		}
		case '|': {
			if (charBuf[currentInd + 1] == '|')
				res.tokenType = XOR, currentInd += 2;
			else 
				printf("Ubdefined char on %d line %d ind\n", currentInd, charBuf[currentInd + 1]), PAUSE, exit(0);
			break;
		}
		case '*': 
			res.tokenType = TIMES, currentInd += 1;
			break;
		case '%': 
			res.tokenType = MODE, currentInd += 1;
			break;
		case '{': 
			res.tokenType = LB, currentInd += 1;
			break;
		case '}':
			res.tokenType = RB, currentInd += 1;
			break;
		case '[': 
			res.tokenType = LSB, currentInd += 1;
			break;
		case ']': 
			res.tokenType = RSB, currentInd += 1;
			break;
		case '(': 
			res.tokenType = LP, currentInd += 1;
			break;
		case ')': 
			res.tokenType = RP, currentInd += 1;
			break;
		case ':': 
			res.tokenType = COLON, currentInd += 1;
			break;
		case ';':
			res.tokenType = SEMICOLON, currentInd += 1;
			break;
		case ',': 
			res.tokenType = COMMA, currentInd += 1;
			break;
		case '.': 
			res.tokenType = DOT, currentInd += 1;
			break;
		case '/':
			res.tokenType = DIVIDE, currentInd += 1;
			break;
		default: {
			int g = 1;
			if (currentChar == '-') {
				if (charBuf[currentInd + 1] == '-') {
					res.tokenType = MMINUS, currentInd += 2; break;
				}
				else if (charBuf[currentInd + 1] < '0' || charBuf[currentInd + 1] > '9') {
					res.tokenType = MINUS, currentInd += 1; break;
				}
				else {
					g = -1, currentInd += 1, currentChar = charBuf[currentInd];
				}
			}
			int state = 1, nextState;
			while (TRUE) {
				int col = stateTransition(state, currentChar);
				if (col == -2) {
					printf("Error in line %d which number %d char [%c]\n",  currentLine, currentInd, currentChar);
					printfToken(); PAUSE, exit(0); return res;
				}
				nextState = stateTransitionTable[state][col];
				if (nextState == -1)
					break;
				currentInd += 1;
				currentChar = charBuf[currentInd];
				state = nextState;
			}
			getTokenVal(res.tokenVal, lastInd, currentInd);
			switch (state) {
			case 2: 
			case 4: {
				res.tokenType = NUM;
				res.numVal = stringToNum(res.tokenVal);
				break;
			}
			case 5: {
				if (isKeyWord(res.tokenVal) != -1)
					res.tokenType = keyWordsMap[isKeyWord(res.tokenVal)];
				else
					res.tokenType = ID;
				break;
			}
			case 6: 
				res.tokenType = SMALLER;
				break;
			case 7: 
				res.tokenType = SMALLEREQU;
				break;
			case 8: 
				res.tokenType = BIGGER;
				break;
			case 9: 
				res.tokenType = BIGGEREQU;
				break;
			case 10: 
				res.tokenType = EQUAL;
				break;
			case 11: 
				res.tokenType = IFEQU;
				break;
			case 12: 
				res.tokenType = DIVIDE;
				break;
			case 15: 
				res.tokenType = COMMENT;
				break;
			default:
				res.tokenType = ERROR;
			}
		}
	}
	return res;
}

static int stateTransition(int state, int currentChar) {
	int col = -2;
	switch (state) {
		case 1: {
			if (isNum(currentChar))
				col = 0;
			else if (isLetter(currentChar))
				col = 3;
			else if (currentChar == '<')
				col = 5;
			else if (currentChar == '>')
				col = 8;
			else if (currentChar == '=')
				col = 7;
			else if (currentChar == '/')
				col = 9;
			break;
		}
		case 2: {
			if (isNum(currentChar))
				col = 0;
			else if (currentChar == '.')
				col = 1;
			else
				col = 2;
			break;
		}
		case 3: {
			if (isNum(currentChar))
				col = 0;
			break;
		}
		case 4: {
			if (isNum(currentChar))
				col = 0;
			else
				col = 2;
			break;
		}
		case 5: {
			if (isNum(currentChar))
				col = 0;
			else if (isLetter(currentChar))
				col = 0;
			else
				col = 4;
			break;
		}
		case 6:
		case 8:
		case 10: {
			if (currentChar == '=')
				col = 7;
			else
				col = 6;
			break;
		}
		case 7:
		case 9:
		case 11:
		case 15: {
			col = 0;
			break;
		}
		case 12: {
			if (currentChar == '/')
				col = 9;
			else if (currentChar == '=')
				col = 10;
			break;
		}
		case 13:
		case 14: {
			if (currentChar == '\n')
				col = 12;
			else
				col = 11;
		}
	}
	return col;
}

static int isKeyWord(char words[]) {
	int Ind = -1;
	for (int pos = 0; pos < 19; pos++) 
		if (strcmp(words, keyWords[pos]) == 0) {
			Ind = pos; break;
		}
	return Ind;
}

static double stringToNum(char string[]) {
	return atof(string);
}

static void getTokenVal(char *address, int L, int R) {
	char tempChar = charBuf[R];
	charBuf[R] = '\0';
	strcpy(address, charBuf + L);
	charBuf[R] = tempChar;
}

static void initTokenTypeMap() {
	strcpy(TokenTypeMap[INT], "INT");
	strcpy(TokenTypeMap[FLOAT], "FLOAT");
	strcpy(TokenTypeMap[CHAR], "CHAR");
	strcpy(TokenTypeMap[DOUBLE], "DOUBLE");
	strcpy(TokenTypeMap[VOID], "VOID");
	strcpy(TokenTypeMap[MAIN], "MAIN");
	strcpy(TokenTypeMap[IF], "IF");
	strcpy(TokenTypeMap[ELSE], "ELSE");
	strcpy(TokenTypeMap[WHILE], "WHILE");
	strcpy(TokenTypeMap[PRINTF], "PRINTF");
	strcpy(TokenTypeMap[SCANF], "SCANF");
	strcpy(TokenTypeMap[FOR], "FOR");
	strcpy(TokenTypeMap[EQUAL], "=");
	strcpy(TokenTypeMap[PLUS], "+");
	strcpy(TokenTypeMap[PPLUS], "++");
	strcpy(TokenTypeMap[MINUS], "-");
	strcpy(TokenTypeMap[MMINUS], "--");
	strcpy(TokenTypeMap[TIMES], "*");
	strcpy(TokenTypeMap[MODE], "%");
	strcpy(TokenTypeMap[DIVIDE], "/");
	strcpy(TokenTypeMap[SMALLER], "<");
	strcpy(TokenTypeMap[SMALLEREQU], "<=");
	strcpy(TokenTypeMap[BIGGER], ">");
	strcpy(TokenTypeMap[BIGGEREQU], ">=");
	strcpy(TokenTypeMap[NOTEQU], "!=");
	strcpy(TokenTypeMap[IFEQU], "==");
	strcpy(TokenTypeMap[LB], "{");
	strcpy(TokenTypeMap[RB], "}");
	strcpy(TokenTypeMap[LSB], "[");
	strcpy(TokenTypeMap[RSB], "]");
	strcpy(TokenTypeMap[LP], "(");
	strcpy(TokenTypeMap[RP], ")");
	strcpy(TokenTypeMap[COLON], ":");
	strcpy(TokenTypeMap[SEMICOLON], ";");
	strcpy(TokenTypeMap[COMMA], ",");
	strcpy(TokenTypeMap[DOT], ".");
	strcpy(TokenTypeMap[RETURN], "RETURN");
	strcpy(TokenTypeMap[AUTO], "AUTO");
	strcpy(TokenTypeMap[BREAK], "BREAK");
	strcpy(TokenTypeMap[CASE], "CASE");
	strcpy(TokenTypeMap[CONST], "CONST");
	strcpy(TokenTypeMap[CONTINUE], "CONTINUE");
	strcpy(TokenTypeMap[STRUCT], "STRUCT");
	strcpy(TokenTypeMap[DEFAULT], "DEFALUT");
	strcpy(TokenTypeMap[SWITCH], "SWITCH");
	strcpy(TokenTypeMap[AND], "&&");
	strcpy(TokenTypeMap[XOR], "||");
	strcpy(TokenTypeMap[NOT], "!");
	strcpy(TokenTypeMap[NUM], "NUM");
	strcpy(TokenTypeMap[ID], "ID");
	strcpy(TokenTypeMap[ERROR], "ERROR");
}

static void printfToken() {
	initTokenTypeMap();
	for (int lineNum = 1, Ind = 0; lineNum <= currentLine; lineNum++) {
		printf("%.2d: ", lineNum);
		int CNT = 0;
		while (Ind < tokenNum && tokenBelongToLine[Ind] == lineNum) {
			CNT++;
			/*if (CNT % 12 == 0) printf("\n    ");*/
			/*if (tokenSeq[Ind].tokenType == NUM)
				printf("[%.1f] ", tokenSeq[Ind].numVal);
			else if (tokenSeq[Ind].tokenType == ID)
				printf("[%s] ", tokenSeq[Ind].tokenVal);
			else */
				printf("[%4s] ", TokenTypeMap[tokenSeq[Ind].tokenType]);
			Ind += 1;
		} printf("%s", "\n");
	} printf("%s", "\n");
}