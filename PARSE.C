#pragma once
#include "PARSE.H"

static void match(TokenType);
static void getToken();
static Node *newStmtNode(StmtType);
static Node *newExpNode(ExpType);
static int isDataType(TokenType);
static int isStmtHeader(TokenType);
static int isCompare(TokenType);
static Node *codeBlock();
static Node *structureStmtNode(TokenType);
static Node *moduleFor();
static Node *moduleDefine();
static Node *moduleDefineInit();
static Node *entityParaList();
static Node *moduleIf();
static Node *bureExp();
static Node *moduleWhile();
static Node *moduleSwitch();
static Node *caseStmt();
static Node *breakStmt();
static Node *defaultStmt();
static Node *baseStmt();
static Node *moduleExp();
static Node *moduleExps();
static Node *assignment();
static Node *funCall();
static Node *simpleExp();
static Node *term();
static Node *factor();
static void prinPARSETreeNode();

static Token token;
static int tokenSeqInd;
Node *syntaxTreeRoot = NULL;

static int isDataType(TokenType tokenType) {
	return tokenType == INT ||
		tokenType == DOUBLE ||
		tokenType == CHAR ||
		tokenType == VOID;
}

static int isStmtHeader(TokenType tokenType) {
	return tokenType == FOR ||
		tokenType == WHILE ||
		tokenType == IF ||
		tokenType == ID ||
		tokenType == RETURN ||
		tokenType == STRUCT ||
		tokenType == SWITCH ||
		tokenType == CASE ||
		tokenType == BREAK ||
		tokenType == DEFAULT ||
		isDataType(tokenType);
}

static int isCompare(TokenType tokenType) {
	return tokenType == SMALLER ||
		tokenType == SMALLEREQU ||
		tokenType == BIGGER ||
		tokenType == BIGGEREQU ||
		tokenType == NOTEQU ||
		tokenType == IFEQU;
	/*
		tokenType == XOR || 
		tokenType == AND;
	*/
}

static void getToken() {
	if (tokenSeqInd <= tokenNum)
		token = tokenSeq[tokenSeqInd++];
	else
		printf("Toekn not exist"), PAUSE, exit(0);
}

static void match(TokenType accept) {
	if (token.tokenType != accept) {
		printf("Out of expectaion in line %d\n", tokenBelongToLine[tokenSeqInd - 1]); PAUSE, exit(0);
	} getToken();
}

void PARSE() {
	Node *root, *tmp, *next = tmp = root = NULL; int tagCnt = 0;
	getToken();
	while (tokenSeqInd < tokenNum) {
		if (!(isDataType(token.tokenType) || (token.tokenType == STRUCT)))
			printf("Para Error\n"), PAUSE, exit(0);

		if (token.tokenType == INT && tokenSeq[tokenSeqInd].tokenType == MAIN) {
			/* main */
			next = newStmtNode(_MAIN);
			match(INT), match(MAIN), match(LP);
			if (token.tokenType != RP)
				next->son[1] = entityParaList();
			match(RP);
			if (token.tokenType != LB) printf("Missing LB\n"), PAUSE, exit(0);
			next->son[2] = codeBlock();
			tagCnt += 1;
		}
		else if (tokenSeq[tokenSeqInd].tokenType == ID && tokenSeq[tokenSeqInd + 1].tokenType == LP) {
			/* function */
			next = newStmtNode(_FUNC);
			match(token.tokenType);
			Node *e = newExpNode(_ID);
			match(ID);
			next->son[0] = e;
			match(LP);
			if (token.tokenType != RP) next->son[1] = entityParaList();
			match(RP);
			if (token.tokenType != SEMICOLON && token.tokenType != LB) printf("Funtioc Error\n"), PAUSE, exit(0);
			next->son[2] = codeBlock();
		}
		else if (token.tokenType == STRUCT && tokenSeq[tokenSeqInd].tokenType == ID && tokenSeq[tokenSeqInd + 1].tokenType == LB) {
			/* struct */
			next = newStmtNode(_STRUCT);
			match(token.tokenType);
			Node *e = newExpNode(_ID);
			match(ID);
			next->son[0] = e;
			if (token.tokenType == LB)
				next->son[1] = codeBlock();
		}
		else if (isDataType(token.tokenType)) {
			/* static variable */
			next = newStmtNode(_DEFINEPARA);
			match(token.tokenType);
			if (token.tokenType != ID) printf("Para Error\n"), PAUSE, exit(0);
			next->son[0] = moduleDefineInit();

			if (token.tokenType != SEMICOLON) printf("Missing SEMICOLON\n"), PAUSE, exit(0);
			match(SEMICOLON);
		}

		if (tmp != NULL) tmp->bro = next;
		tmp = next;
		if (root == NULL) root = next;
	}
	if (tagCnt != 1) printf("Main is not only one\n"), PAUSE, exit(0);
	syntaxTreeRoot = root;
#ifdef IFMS
	prinPARSETreeNode();
#endif
}

static Node *newStmtNode(StmtType stmtType) {
	Node *p = (Node*)malloc(sizeof(Node));
	p->nodeType = _STMT;
	p->detailedType.stmtType = stmtType;
	p->LINE = tokenBelongToLine[tokenSeqInd - 1];
	p->bro = NULL;
	for (int pos = 0; pos < 4; pos++) p->son[pos] = NULL;

	if (p->detailedType.stmtType == _MAIN || p->detailedType.stmtType == _DEFINEPARA) 
		p->dataType = token.tokenType;
	else if (p->detailedType.stmtType == _FUNC && isDataType(token.tokenType))
		p->dataType = token.tokenType;
	if (p->detailedType.stmtType == _DEFINEPARA)
		strcpy(p->attribute.name, tokenSeq[tokenSeqInd].tokenVal);
	return p;
}

static Node *newExpNode(ExpType expType) {
	Node *p = (Node*)malloc(sizeof(Node));
	p->nodeType = _EXP;
	p->detailedType.expType = expType;
	p->LINE = tokenBelongToLine[tokenSeqInd - 1];
	p->bro = NULL;
	for (int pos = 0; pos < 4; pos++) p->son[pos] = NULL;

	if (expType == _OP)
		p->attribute.op = token.tokenType;
	else if (expType == _CONST)
		p->attribute.value = token.numVal;
	else if (expType == _ID && tokenSeq[tokenSeqInd].tokenType != DOT)
		strcpy(p->attribute.name, token.tokenVal);
	
	
	if (expType == _ID && tokenSeq[tokenSeqInd].tokenType == DOT) {
		char structAttrName[SMALLCONSTANT] = { '\0' };
		strcpy(structAttrName, token.tokenVal);
		strcpy(structAttrName + strlen(structAttrName), ".");
		strcpy(structAttrName + strlen(structAttrName), tokenSeq[tokenSeqInd + 1].tokenVal);
		strcpy(p->attribute.name, structAttrName);
	}
	return p;
}

static Node *entityParaList() {
	Node *root, *now, *next = now = root = NULL;
	do {
		if (root != NULL) match(COMMA);
		next = newStmtNode(_DEFINEPARA);
		if (root == NULL) root = next;
		if (isDataType(token.tokenType))
			match(token.tokenType);
		else
			printf("Para List DataType Error\n"), PAUSE, exit(0);
		if (tokenSeq[tokenSeqInd].tokenType == LSB) {
			next->son[0] = newExpNode(_CITEARRAY);
			next->son[0]->son[0] = newExpNode(_ID);
			match(ID);
			match(LSB);
			match(RSB);
		}
		else {
			next->son[0] = newExpNode(_ID);
			match(ID);
		}
		if (now != NULL)
			now->bro = next;
		now = next;
	} while (token.tokenType == COMMA);
	return root;
}

static Node *codeBlock() {
	Node *root, *now, *next = now = root = NULL;
	match(LB);
	while (isStmtHeader(token.tokenType) || token.tokenType == SEMICOLON) {
		if (token.tokenType == SEMICOLON) {
			match(token.tokenType); continue;
		}
		if (token.tokenType == BREAK) {
			int Ind = tokenSeqInd - 2, LBcnt = 0, able = 0;
			while (Ind >= 0) {
				if (tokenSeq[Ind].tokenType == LB) LBcnt++;
				else if (tokenSeq[Ind].tokenType == RB) LBcnt--;
				if (tokenSeq[Ind].tokenType == FOR || tokenSeq[Ind].tokenType == WHILE) 
					if (LBcnt >= 1) {
						able = 1; break;
					}
				Ind--;
			}
			if (!able) printf("Break Error\n"), PAUSE, exit(0);
		}

		next = structureStmtNode(token.tokenType);
		if (root == NULL) root = next;
		if (now != NULL)
			now->bro = next;
		now = next;
	}
	match(RB);
	return root;
}

static Node *structureStmtNode(TokenType tokenType) {
	Node *root = NULL;
	switch (tokenType) {
	case FOR:
		root = moduleFor(); break;
	case WHILE:
		root = moduleWhile(); break;
	case IF:
		root = moduleIf(); break;
	case SWITCH:
		root = moduleSwitch(); break;
	case CASE:
		root = caseStmt(); break;
	case BREAK:
		root = breakStmt(); break;
	case DEFAULT:
		root = defaultStmt(); break;
	default:
		root = baseStmt();
		if (token.tokenType != SEMICOLON) {
			/*for (int pos = tokenSeqInd; pos < tokenNum; pos++)
				printf("%s ", TokenTypeMap[tokenSeq[pos - 1].tokenType]);*/

			printf("Missing SEMICOLON\n"), PAUSE, exit(0);
		}
		else
			match(SEMICOLON);
		break;
	}
	return root;
}
static void prinTreeNode(Node *now, int g);
static Node *moduleFor() {
	Node *root = newStmtNode(_FOR);
	match(FOR), match(LP);
	if (token.tokenType != SEMICOLON) {
		if (token.tokenType == INT)
			root->son[0] = moduleDefine();
		else 
			root->son[0] = moduleExps();
	}
	match(SEMICOLON);
	if (token.tokenType != SEMICOLON) {
		if (tokenSeq[tokenSeqInd].tokenType == EQUAL)
			printf("Para Of For Error\n"), PAUSE, exit(0);
		else {
			if (tokenSeq[tokenSeqInd].tokenType == SEMICOLON) {
				Node *son0 = bureExp();
				tokenSeqInd--;
				token.tokenType = BIGGER, strcpy(token.tokenVal, ">");
				root->son[1] = newExpNode(_OP);
				token.tokenType = CONST, token.numVal = 0;
				root->son[1]->son[1] = newExpNode(_CONST);
				root->son[1]->son[0] = son0;
				match(token.tokenType);
			}
			else {
				root->son[1] = moduleExps();
				if (root->son[1]->son[1] != NULL && root->son[1]->son[1]->detailedType.expType == _OP) {
					if (root->son[1]->attribute.op == BIGGER)
						root->son[1]->attribute.op = SMALLEREQU;
					else if (root->son[1]->attribute.op == SMALLEREQU)
						root->son[1]->attribute.op = BIGGER;
					else if (root->son[1]->attribute.op == BIGGEREQU)
						root->son[1]->attribute.op = SMALLER;
					else
						root->son[1]->attribute.op = BIGGEREQU;
				}
				
			}
		}
	}
	match(SEMICOLON);
	if (token.tokenType != RP) 
		root->son[2] = moduleExps();
	match(RP);
	if (token.tokenType == SEMICOLON) {
		match(SEMICOLON); return root;
	}
	root->son[3] = codeBlock();
	return root;
}

static Node *moduleDefine() {
	Node *root = newStmtNode(_DEFINEPARA);
	if (isDataType(token.tokenType))
		root->dataType = token.tokenType, match(token.tokenType);
	else if (token.tokenType == STRUCT)
		match(token.tokenType), match(ID);
	else
		printf("Defein Error\n"), PAUSE, exit(0);
	root->son[0] = moduleDefineInit();
	return root;
}

static Node *moduleDefineInit() {
	Node *root = NULL, *tmp = NULL, *next;
	if (tokenSeq[tokenSeqInd].tokenType == SEMICOLON) {
		root = newExpNode(_ID);
		match(ID);
		return root;
	}
	while (tokenSeq[tokenSeqInd].tokenType == EQUAL ||
		tokenSeq[tokenSeqInd].tokenType == COMMA ||
		tokenSeq[tokenSeqInd].tokenType == LSB) {
		switch (tokenSeq[tokenSeqInd].tokenType) {
		case EQUAL: {
			next = assignment();
			if (tmp != NULL) tmp->bro = next;
			tmp = next;
			if (token.tokenType == COMMA)
				match(COMMA);
			if (root == NULL) root = next;
			break;
		}
		case COMMA: {
			next = newExpNode(_ID);
			match(ID), match(COMMA);
			if (tmp != NULL) tmp->bro = next;
			tmp = next;
			if (root == NULL) root = next;
			break;
		}
		case LSB: {
			next = newStmtNode(_DEFINEARRAY);
			Node *e = newExpNode(_ID);
			match(ID), match(LSB);
			next->son[0] = e;
			if (token.tokenType != NUM)
				printf("VarInit Error[NUM]\n"), PAUSE, exit(0);
			if (token.tokenType == NUM) {
				e = newExpNode(_CONST);
				match(NUM);
				next->son[1] = e;
			}
			if (token.tokenType != RSB)
				printf("VarInit Error[RSB]\n"), PAUSE, exit(0);
			match(RSB);
			if (tmp != NULL) tmp->bro = next;
			tmp = next;
			if (root == NULL) root = next;

			
			if (token.tokenType == EQUAL && tokenSeq[tokenSeqInd].tokenType == LB) {
				match(EQUAL), match(LB);
				int Ind = 0;
				while (token.tokenType != RB) {
					if (Ind >= next->son[1]->attribute.value) printf("Too many array list parameters\n"), PAUSE, exit(0);
					if (token.tokenType != NUM) printf("Init Error\n"), PAUSE, exit(0);
					Node *initRoot = newStmtNode(_ASSIGN);
					
					Node *arrs = newExpNode(_CITEARRAY);
					Node *son0 = newExpNode(_ID);
					Node *son1 = newExpNode(_CONST);
					strcpy(son0->attribute.name, next->son[0]->attribute.name);
					arrs->son[0] = son0;
					son1->attribute.value = Ind; Ind++;
					arrs->son[1] = son1;

					initRoot->son[0] = arrs;
					initRoot->son[1] = moduleExps();

					tmp->bro = initRoot;
					tmp = initRoot;
					if (token.tokenType == COMMA) match(COMMA);
				} match(RB);
			}
			if (token.tokenType == COMMA)
				match(COMMA);
			break;
		}
		}
	}
	if (tokenSeq[tokenSeqInd].tokenType == SEMICOLON) {
		next = newExpNode(_ID);
		match(ID);
		if (tmp != NULL) tmp->bro = next;
		tmp = next;
		if (root == NULL) root = next;
	} 
	else if (token.tokenType != SEMICOLON) printf("MISSING SEMICOLON\n"), PAUSE, exit(0);
	return root;
}

static Node *moduleIf() {
	Node *root = newStmtNode(_IF);
	match(IF), match(LP);
	if (tokenSeq[tokenSeqInd].tokenType == EQUAL && tokenSeq[tokenSeqInd + 1].tokenType != EQUAL) 
		printf("PARA OF IF ERROR"), PAUSE, exit(0);
	if (tokenSeq[tokenSeqInd].tokenType == RP) {
		Node *son0 = bureExp();
		tokenSeqInd--;
		token.tokenType = BIGGER, strcpy(token.tokenVal, ">");
		root->son[0] = newExpNode(_OP);
		token.tokenType = CONST, token.numVal = 0;
		root->son[0]->son[1] = newExpNode(_CONST);
		root->son[0]->son[0] = son0;
		match(token.tokenType);
	}
	else {
		root->son[0] = bureExp();
	}
	match(RP);
	root->son[1] = codeBlock();
	if (token.tokenType == ELSE && tokenSeq[tokenSeqInd].tokenType == IF) {
		match(ELSE);
		root->son[2] = moduleIf();
		/*match(IF);*/
		/*
			this have's tag
		*/
	}
	if (token.tokenType == ELSE && tokenSeq[tokenSeqInd].tokenType != IF) {
		match(ELSE);
		root->son[2] = codeBlock();
	}
	return root;
}

static Node *bureExp() {
	Node *root = NULL;
	/*if ((tokenSeq[tokenSeqInd].tokenType == AND && tokenSeq[tokenSeqInd + 1].tokenType == AND) ||
		(tokenSeq[tokenSeqInd].tokenType == XOR && tokenSeq[tokenSeqInd + 1].tokenType == XOR))
		;
	else*/
		root = moduleExps();
	return root;
}

static Node *moduleWhile() {
	Node *root = newStmtNode(_WHILE);
	match(WHILE), match(LP);
	if (tokenSeq[tokenSeqInd].tokenType == RP) {
		Node *son0 = bureExp();
		tokenSeqInd--;
		token.tokenType = BIGGER, strcpy(token.tokenVal, ">");
		root->son[0] = newExpNode(_OP);
		token.tokenType = CONST, token.numVal = 0;
		root->son[0]->son[1] = newExpNode(_CONST);
		root->son[0]->son[0] = son0;
		match(token.tokenType);
	}
	else {
		root->son[0] = moduleExps();
	}
	match(RP);
	if (token.tokenType == LB)
		root->son[1] = codeBlock();
	else if (token.tokenType == SEMICOLON) match(SEMICOLON);
	return root;
}

static Node *moduleSwitch() {
	Node *root = newStmtNode(_SWITCH);
	match(SWITCH), match(LP);
	root->son[0] = moduleExps();
	match(RP);
	if (token.tokenType == LSB)
		root->son[1] = codeBlock();
	return root;
}

static Node *caseStmt() {
	Node *root = newStmtNode(_CASE);
	match(CASE);
	root->son[0] = newExpNode(_ID);
	match(ID), match(COLON);
	if (token.tokenType == LSB)
		root->son[1] = codeBlock();
	return root;
}

static Node *breakStmt() {
	Node *root = newStmtNode(_BREAK);
	match(BREAK), match(SEMICOLON);
	return root;
}

static Node *defaultStmt() {
	Node *root = newStmtNode(_DEFAULT);
	match(DEFAULT), match(COLON);
	if (token.tokenType == LSB)
		root->son[0] = codeBlock();
	return root;
}

static int isArrayExp() {
	int RSBIND = tokenNum;
	if (tokenSeq[tokenSeqInd].tokenType == LSB) {
		RSBIND = tokenSeqInd + 2;
		while (tokenSeqInd < tokenNum && tokenSeq[RSBIND].tokenType != RSB) RSBIND++;
	}
	if (RSBIND < tokenNum && (tokenSeq[RSBIND + 1].tokenType == EQUAL || tokenSeq[RSBIND + 1].tokenType == MMINUS || tokenSeq[RSBIND + 1].tokenType == PPLUS)) return TRUE;
	return FALSE;
}

static Node *baseStmt() {
	Node *root = NULL;
	if (isDataType(token.tokenType) || token.tokenType == STRUCT)  
		root = moduleDefine();
	else if (token.tokenType == RETURN) {
		root = newStmtNode(_RETURN);
		match(RETURN);
		root->son[0] = moduleExp();
	}
	else if (token.tokenType == ID) {
		if (isArrayExp() ||
			tokenSeq[tokenSeqInd].tokenType == EQUAL ||
			(tokenSeq[tokenSeqInd].tokenType == DOT && tokenSeq[tokenSeqInd + 2].tokenType == EQUAL))
			root = assignment();
		else if (tokenSeq[tokenSeqInd].tokenType == LP)
			root = funCall();
		else if (TRUE)
			root = moduleExp();
	}
	return root;
}

static Node *paraOfFunCall() {
	Node *root, *tmp = root = factor();
	while (token.tokenType == COMMA) {
		match(COMMA);
		tmp->bro = factor();
		tmp = tmp->bro;
	}
	return root;
}


static Node *funCall() {
	Node *root = newStmtNode(_FUNC);
	root->son[0] = newExpNode(_ID);
	match(ID), match(LP);
	root->son[1] = paraOfFunCall();
	match(RP);
	return root;
}

static Node *moduleExp() {
	Node *root, *tmp = simpleExp();
	if (isCompare(token.tokenType)) {
		root = newExpNode(_OP);
		match(token.tokenType);
		root->son[0] = tmp;
		root->son[1] = simpleExp();
	}
	else {
		root = tmp;
	}
	return root;
}

static Node *moduleExps() {
	Node *root = NULL;
	if (token.tokenType == ID && tokenSeq[tokenSeqInd].tokenType == EQUAL) 
		root = assignment();
	else if ((token.tokenType == ID && tokenSeq[tokenSeqInd].tokenType == LSB && tokenSeq[tokenSeqInd + 3].tokenType == EQUAL) ||
		tokenSeq[tokenSeqInd].tokenType == PPLUS ||
		tokenSeq[tokenSeqInd].tokenType == MMINUS)
		root = assignment();
	else
		root = moduleExp();
	return root;
}

static Node *assignment() {
	Node *root = NULL;
	if (tokenSeq[tokenSeqInd].tokenType == LSB) {
		root = newExpNode(_CITEARRAY);
		Node *tmp = newExpNode(_ID);
		match(ID);
		root->son[0] = tmp;
		match(LSB);
		/*root->son[1] = moduleExps();*/
		if (token.tokenType == NUM)
			root->son[1] = moduleExps();
		else
			root->son[1] = newExpNode(_ID), getToken();
		match(RSB);
		if (token.tokenType == PPLUS || token.tokenType == MMINUS) {
			Node *owo = newStmtNode(_ASSIGN);
			owo->son[0] = root;
			owo->son[1] = newExpNode(_OP);
			match(token.tokenType);
			return owo;
		}
	}
	else if (tokenSeq[tokenSeqInd].tokenType == DOT) {
		root = newExpNode(_ID);
		match(ID);
		match(DOT);
		match(ID);
	}
	else if (tokenSeq[tokenSeqInd].tokenType == PPLUS || tokenSeq[tokenSeqInd].tokenType == MMINUS) {
		root = newStmtNode(_ASSIGN);
		root->son[0] = newExpNode(_ID);
		match(ID);
		root->son[1] = newExpNode(_OP);
		match(token.tokenType);
		return root;
	}
	else if (token.tokenType == ID) {
		root = newExpNode(_ID);
		match(ID);
	}
	match(EQUAL);
	Node *next = newStmtNode(_ASSIGN);
	next->son[0] = root;
	next->son[1] = moduleExp();
	return next;
}

static Node *simpleExp() {
	Node *root = term(), *next;
	while (token.tokenType == PLUS || token.tokenType == MINUS || token.tokenType == XOR || token.tokenType == AND) {
		next = newExpNode(_OP);
		match(token.tokenType);
		next->son[0] = root;
		next->son[1] = term();
		root = next;
	}
	return root;
}

static Node *term() {
	Node *root = factor(), *next;
	while (token.tokenType == TIMES || token.tokenType == DIVIDE || token.tokenType == MODE) {
		next = newExpNode(_OP);
		match(token.tokenType);
		next->son[0] = root;
		next->son[1] = factor();
		root = next;
	}
	return root;
}

static Node *factor() {
	Node *root, *tmp = root = NULL;
	switch (token.tokenType) {
	case LP:
		match(LP);
		root = moduleExp();
		match(RP);
		break;
	case ID:
		if (tokenSeq[tokenSeqInd].tokenType == LP)
			root = funCall();
		else if (tokenSeq[tokenSeqInd].tokenType == LSB)
			root = newExpNode(_CITEARRAY),
			tmp = newExpNode(_ID),
			match(ID),
			match(LSB),
			root->son[0] = tmp,
			root->son[1] = moduleExps(),
			match(RSB);
		else if (tokenSeq[tokenSeqInd].tokenType == DOT)
			root = newExpNode(_ID),
			match(ID),
			match(DOT),
			match(ID);
		else if (tokenSeq[tokenSeqInd].tokenType == PPLUS || tokenSeq[tokenSeqInd].tokenType == MMINUS)
			root = moduleExps();
		else
			root = newExpNode(_ID),
			match(ID);
		break;
	case NUM:
		root = newExpNode(_CONST);
		root->attribute.value = token.numVal;
		match(NUM);
		break;
	}
	return root;
}


static void show(Node *now) {
	if (now->nodeType == _EXP) {
		switch (now->detailedType.expType) {
		case _ID:
			printf("[ID] = %s", now->attribute.name);
			break;
		case _OP:
			printf("[OP] = %s", TokenTypeMap[now->attribute.op]);
			break;
		case _CONST:
			printf("[NUM] = %.2f", now->attribute.value);
			break;
		case _CITEARRAY:
			printf("[ARRAR]");
		}
	}
	else if (now->nodeType == _STMT) {
		switch (now->detailedType.stmtType) {
		case _IF:
			printf("[IF]");
			break;
		case _FOR:
			printf("[FOR]");
			break;
		case _WHILE:
			printf("[WHILE]");
			break;
		case _RETURN:
			printf("[RETURN]");
			break;
		case _ASSIGN:
			printf("[ASSIGN]");
			break;
		case _FUNC:
			if (now->son[2] != NULL)
				printf("[DEFINE FUNCTION TYPE IS [%s]", TokenTypeMap[now->dataType]);
			else
				printf("[CALL FUNCTION]");
			break;
		case _DEFINEPARA:
			if (now->dataType != STRUCT)
				printf("[DEFINE PARA] = %s", TokenTypeMap[now->dataType]);
			else 
				printf("[DEFINE PARA] = %s]", now->attribute.name);
			break;
		case _MAIN:
			printf("[MAIN]");
			break;
		case _DEFINEARRAY:
			printf("[DEFINEARRAY]");
			break;
		case _STRUCT:
			printf("[STRUCT]");
			break;
		case _CASE:
			printf("[CASE]");
			break;
		case _BREAK:
			printf("[BREAK]");
			break;
		case _SWITCH:
			printf("[SWITHC]");
			break;
		case _DEFAULT:
			printf("[DEFAULT]");
			break;
		default:
			printf("[ERRPR]");
		}
	}
	else printf("nodeType error");
}

static void prinTreeNode(Node *now, int g) {
	for (int pos = 0; pos < g; pos++) printf("        ");
	show(now);
	printf("\n");
	for (int pos = 0; pos < 4; pos++)
		if (now->son[pos] != NULL) 
			prinTreeNode(now->son[pos], g + 1);
	if (now->bro != NULL)
		prinTreeNode(now->bro, g);
}

static void prinPARSETreeNode() {
	Node *root = syntaxTreeRoot;
	prinTreeNode(root, 0);
	printf("%s", "\n");
}

static void subDestryTree(Node *now) {
	for (int pos = 0; pos < 4; pos++)
		if (now->son[pos] != NULL)
			subDestryTree(now->son[pos]);
	if (now->bro != NULL)
		subDestryTree(now->bro);
	free(now);
}

void destryTree() {
	subDestryTree(syntaxTreeRoot);
}
