#pragma once
#include "TRANS.H"
 
static void transStart(Node *);
static void transMain(Node *);
static void transDefinePara(Node *);
static void transAssign(Node *);
static void transReturn(Node *);
static void transIf(Node *);
static void transWhile(Node *);
static void transFor(Node *);
static void transFunction(Node *);
static void getQuaternion(opType, char[], char[], char[]);
static char *transExp(Node *);
static char *newVariable();
static char *newLable();
static int paraCnt(Node *);
static void printQuaternion(quaternion *);
static void initTransExpMap();
static void transBreak(Node *);

quaternion *quaternionHead;
quaternion *quaternionTail;
static int variableCnt, lableCnt;
static char charBuffOfVariable[SMALLCONSTANT];
static char charBuffOfLable[SMALLCONSTANT];
static int transExpMap[CONSTANT];
char formalAddress[SMALLCONSTANT] = { '\0' };
char empty[SMALLCONSTANT] = { '\0' };
char opTypeMap[CONSTANT >> 1][17];
char para[5][MAXTOKENLEN];
char breakLab[5][MAXTOKENLEN]; int breakLabInd;

static void initPara() {
	for (int pos = 0; pos < 5; pos++) strcpy(para[pos], empty);
}

void TRANS() {
	quaternionHead = quaternionTail = (quaternion*)malloc(sizeof(quaternion));
	initTransExpMap();
	transStart(syntaxTreeRoot);
#ifdef IFMS
	printQuaternion(quaternionHead->next);
#endif
}

static void transStart(Node *root) {
	while (root != NULL) {
		if (root->nodeType == _STMT) {
			switch (root->detailedType.stmtType) {	
			case _MAIN: transMain(root); break;
			case _DEFINEPARA: transDefinePara(root); break;
			case _ASSIGN: transAssign(root); break;
			case _IF: transIf(root); break;
			case _FOR: transFor(root); break;
			case _WHILE: transWhile(root); break;
			case _RETURN: transReturn(root); break;
			case _FUNC: transFunction(root); break;
			case _BREAK: transBreak(root); break;
			}
		}
		else {
			printf("Trans Error\n"); PAUSE; exit(0);
		}
		root = root->bro;
	}
}

static void getQuaternion(opType op, char aAddr[], char bAddr[], char cAddr[]) {
	quaternion *quat = (quaternion*)malloc(sizeof(quaternion));
	char charHead[3] = { aAddr[0],bAddr[0],cAddr[0] };
	quat->opTypes = op;
	for (int pos = 0; pos < 3; pos++) {
		char now = charHead[pos];
		if (now == '\0')
			quat->addr[pos].addressTypes = EMPTYS_, strcpy(quat->addr[pos].name, empty);
		else {
			if (isLetter(now))
				quat->addr[pos].addressTypes = STRINGS_;
			else
				quat->addr[pos].addressTypes = CONSTS_;
			switch (pos) {
			case 0: strcpy(quat->addr[pos].name, aAddr); break;
			case 1:	strcpy(quat->addr[pos].name, bAddr); break;
			case 2:	strcpy(quat->addr[pos].name, cAddr); break;
			}
		}
	}
	quat->next = NULL;
	quaternionTail->next = quat;
	quaternionTail = quat;
}

static void transMain(Node *now) {
	initPara();
	getQuaternion(MAINS_, empty, empty, empty);
	Node *next = now->son[1];
	if (next != NULL) {
		strcpy(para[0], TokenTypeMap[next->dataType]);
		getQuaternion(DEFINEPARAS_, para[0], next->son[0]->attribute.name, empty);
		next = next->bro;
	}
	transStart(now->son[2]);
	getQuaternion(MAINE_, empty, empty, empty);
}

static void transDefinePara(Node *now) {
	initPara();
	Node *next = now->son[0];
	strcpy(para[0], TokenTypeMap[now->dataType]);
	while (next != NULL) {
		if (next->detailedType.stmtType == _ASSIGN && !(next->son[0] != NULL && next->son[0]->detailedType.expType == _CITEARRAY)) {
			getQuaternion(DEFINEPARAS_, para[0], next->son[0]->attribute.name, empty);
			transAssign(next);
		}
		else if (next->detailedType.stmtType == _ASSIGN && (next->son[0] != NULL && next->son[0]->detailedType.expType == _CITEARRAY)) {
			/* ARRAY INIT  */
			transAssign(next);
		}
		else if (next->detailedType.stmtType == _DEFINEARRAY) {
			char buffs[MAXTOKENLEN]; memset(buffs, 0, sizeof(buffs));
			sprintf(buffs, "%.0f", next->son[1]->attribute.value);
			getQuaternion(DEFINEPARAS_, para[0], next->son[0]->attribute.name, buffs);
		}
		else
			getQuaternion(DEFINEPARAS_, para[0], next->attribute.name, empty);
		next = next->bro;
	}
}

static void transAssign(Node *now) {
	initPara();
	if (now->son[1]->attribute.op == PPLUS || now->son[1]->attribute.op == MMINUS) {
		if (now->son[0]->detailedType.expType == _ID)
			getQuaternion(now->son[1]->attribute.op == PPLUS ? INC_ : DEC_, now->son[0]->attribute.name, empty, empty);
		else if (now->son[0]->son[1]->detailedType.expType == _ID) 
			getQuaternion(now->son[1]->attribute.op == PPLUS ? INC_ : DEC_, now->son[0]->son[0]->attribute.name, now->son[0]->son[1]->attribute.name, empty);
		else
			sprintf(para[0], "%.0f", now->son[0]->son[1]->attribute.value),
			getQuaternion(now->son[1]->attribute.op == PPLUS ? INC_ : DEC_, now->son[0]->son[0]->attribute.name, para[0], empty);
		return;
	}
	
	strcpy(para[0], newVariable());
	if (now->son[1]->detailedType.expType != _CITEARRAY) {
		strcpy(para[1], transExp(now->son[1]));
	}
	else if (now->son[1]->detailedType.expType == _CITEARRAY) { /* RIGHT */
		strcpy(para[1], now->son[1]->son[0]->attribute.name);
		if (now->son[1]->son[1] != NULL && now->son[1]->son[1]->detailedType.expType == _ID) {
			strcpy(para[2], now->son[1]->son[1]->attribute.name);
		}
		else {
			sprintf(para[2], "%.0f", now->son[1]->son[1]->attribute.value);
		}
	}
	getQuaternion(ASSIGN_, para[1], para[0], para[2]);
	if (now->son[0]->detailedType.expType == _ID)				/* LEFT */
		getQuaternion(ASSIGN_, para[0], now->son[0]->attribute.name, empty);
	else if (now->son[0]->detailedType.expType == _CITEARRAY) {
		if (now->son[0]->son[1] != NULL && now->son[0]->son[1]->detailedType.expType == _ID) {
			strcpy(para[2], now->son[0]->son[1]->attribute.name);
		}
		else {
			sprintf(para[2], "%.0f", now->son[0]->son[1]->attribute.value);
		}
		
		char buffs[MAXTOKENLEN]; memset(buffs, 0, sizeof(buffs));
		if (now->son[0]->detailedType.expType == _CITEARRAY) {
			getQuaternion(ARRASSIGN_, para[0], now->son[0]->son[0]->attribute.name, para[2]);
		}
		else {
			getQuaternion(ASSIGN_, para[0], now->son[0]->son[0]->attribute.name, para[2]);
		}
	}
}

static void transReturn(Node *now) {
	initPara();
	if (now->son[0] != NULL) {
		strcpy(para[0], newVariable());
		strcpy(para[1], transExp(now->son[0]));
		getQuaternion(ASSIGN_, para[1], para[0], empty);
		getQuaternion(RETURNS_, para[0], empty, empty);
	}
	else
		getQuaternion(RETURNS_, empty, empty, empty);
}

static void transIf(Node *now) {
	getQuaternion(IFS_, empty, empty, empty);
	char lab[2][MAXTOKENLEN]; initPara();
	for (int pos = 0; pos < 2; pos++) memset(para[pos], 0, sizeof(para[pos]));

	strcpy(para[0], newVariable());
	if (now->son[0]->detailedType.expType == _OP || 
		now->son[0]->detailedType.expType == _ID || 
		now->son[0]->detailedType.expType == _CONST)
		strcpy(para[1], transExp(now->son[0]));


	getQuaternion(ASSIGN_, para[1], para[0], empty), strcpy(lab[0], newLable());
	getQuaternion(IFFALSE_, para[0], lab[0], empty), transStart(now->son[1]);

	getQuaternion(IFE_, empty, empty, empty);
	strcpy(lab[1], newLable()), getQuaternion(JUMPS_, lab[1], empty, empty);

	getQuaternion(LAB_, lab[0], empty, empty), transStart(now->son[2]);
	getQuaternion(LAB_, lab[1], empty, empty);
}

static void transWhile(Node *now) {
	char lab[3][MAXTOKENLEN]; initPara();
	strcpy(lab[0], newLable());
	getQuaternion(LAB_, lab[0], empty, empty);
	getQuaternion(WHILES_, empty, empty, empty);

	strcpy(para[0], newVariable());
	/*if (now->son[0]->detailedType.expType == _OP)
		strcpy(para[1], transExp(now->son[0]));*/
	strcpy(para[1], transExp(now->son[0]));
	getQuaternion(ASSIGN_, para[1], para[0], empty);

	strcpy(lab[1], newLable());
	strcpy(breakLab[breakLabInd], lab[1]);
	breakLabInd++;
	getQuaternion(IFFALSE_, para[0], lab[1], empty);

	transStart(now->son[1]);
	getQuaternion(WHILEE_, empty, empty, empty);
	getQuaternion(JUMPS_, lab[0], empty, empty);
	getQuaternion(LAB_, lab[1], empty, empty);
	breakLabInd--;
}

static void transFor(Node *now) {
	char lab[3][MAXTOKENLEN]; initPara();
	getQuaternion(FORS_, empty, empty, empty);
	transStart(now->son[0]);
	
	strcpy(lab[0], newLable());
	getQuaternion(LAB_, lab[0], empty, empty);
	if (now->son[1] != NULL) {
		strcpy(para[0], newVariable());
		/*if (now->son[1]->detailedType.expType == _OP)
			strcpy(para[1], transExp(now->son[1]));*/
		strcpy(para[1], transExp(now->son[1]));
		getQuaternion(ASSIGN_, para[1], para[0], empty);
	}
	strcpy(lab[1], newLable());
	strcpy(breakLab[breakLabInd], lab[1]);
	breakLabInd++;
	if (para[0][0] == '\0') para[0][0] = '1';
	getQuaternion(FORCMP_, para[0], lab[1], empty);
	transStart(now->son[3]);
	transStart(now->son[2]);
	getQuaternion(FORE_, empty, empty, empty);
	getQuaternion(JUMPS_, lab[0], empty, empty);
	getQuaternion(LAB_, lab[1], empty, empty);
	breakLabInd--;
}

static void transBreak(Node *root) {
	getQuaternion(JUMPS_, breakLab[breakLabInd - 1], empty, empty);
}

static void transFunction(Node *now) {
	/* son0-FunName son1-paraList son2-FunBlack */
	char paraNum[MAXTOKENLEN]; initPara();
	if (now->son[2] == NULL) {
		strcpy(paraNum, "0");
		if (now->son[1] != NULL)
			sprintf(paraNum, "%d", paraCnt(now->son[1]));
		getQuaternion(ARGSS_, paraNum, empty, empty);
		Node *next = now->son[1];
		while(next != NULL) {
			strcpy(para[0], newVariable());
			strcpy(para[1], transExp(next));
			getQuaternion(ASSIGN_, para[1], para[0], empty);
			getQuaternion(ARGS_, para[0], empty, empty);
			next = next->bro;
		}
		getQuaternion(CALLFUNC_, now->son[0]->attribute.name, empty, empty);
	}
	else {
		strcpy(paraNum, "0");
		if (now->son[1] != NULL)
			sprintf(paraNum, "%d", paraCnt(now->son[1]));
		getQuaternion(DEFINEFUNCS_, now->son[0]->attribute.name, paraNum, empty);
		Node *next = now->son[1];
		while (next != NULL) {
			if (next->son[0]->detailedType.expType == _CITEARRAY)
				getQuaternion(DEFINEPARAS_, TokenTypeMap[now->dataType], next->son[0]->son[0]->attribute.name, empty);
			else 
				getQuaternion(DEFINEPARAS_, TokenTypeMap[now->dataType], next->son[0]->attribute.name, empty);
			next = next->bro;
		}
		getQuaternion(DEFINEPARAS_, "CS-IP", "CS-IP", empty);
		transStart(now->son[2]);
		getQuaternion(DEFINEFUNCE_, now->son[0]->attribute.name, empty, empty);
	}
}

static char *transExp(Node *now) {
	char interPara[5][MAXTOKENLEN];
	for (int pos = 0; pos < 5; pos++)
		memset(interPara[pos], 0, sizeof(interPara[pos]));
	if (now == NULL) printf("Missing Exp\n"), PAUSE, exit(0);
	switch (now->detailedType.expType) {
	case _OP: {
		strcpy(interPara[0], newVariable());
		strcpy(interPara[1], newVariable());
		if (now->son[1]->detailedType.expType == _CONST) {
			sprintf(interPara[2], "%.0f", now->son[1]->attribute.value);
			if ((now->attribute.op == PLUS || now->attribute.op == MINUS) &&
				!strcmp(interPara[2], "1") &&
				now->son[0]->bro != NULL
				) {
				printf("Trans Exp Errpr\n"), PAUSE, exit(0);
			}
		}
		strcpy(interPara[3], transExp(now->son[0]));
		strcpy(interPara[4], transExp(now->son[1]));
		/*printf("%d %d\n", now->son[0]->detailedType.expType, now->son[1]->detailedType.expType);
		printf("%s %s\n", interPara[3], interPara[4]); PAUSE;*/
		if (now->attribute.op == TIMES ||
			now->attribute.op == DIVIDE ||
			now->attribute.op == MODE ||
			now->attribute.op == NOTEQU ||
			now->attribute.op == IFEQU ||
			now->attribute.op == SMALLER ||
			now->attribute.op == SMALLEREQU ||
			now->attribute.op == BIGGER ||
			now->attribute.op == BIGGEREQU) {

			char buffs[MAXTOKENLEN]; 
			memset(buffs, 0, sizeof(buffs));
			if (now->son[0]->detailedType.expType == _CITEARRAY && now->son[0]->son[1]->detailedType.expType == _CONST) 
				sprintf(buffs, "%.0f", now->son[0]->son[1]->attribute.value);
			else if (now->son[0]->detailedType.expType == _CITEARRAY)
				strcpy(buffs, now->son[0]->son[1]->attribute.name);
			getQuaternion(ASSIGN_, interPara[3], interPara[0], buffs);

			memset(buffs, 0, sizeof(buffs));
			if (now->son[1]->detailedType.expType == _CITEARRAY && now->son[1]->son[1]->detailedType.expType == _CONST)
				sprintf(buffs, "%.0f", now->son[1]->son[1]->attribute.value);
			else if (now->son[1]->detailedType.expType == _CITEARRAY)
				strcpy(buffs, now->son[1]->son[1]->attribute.name);
			getQuaternion(ASSIGN_, interPara[4], interPara[1], buffs);

			strcpy(interPara[2], newVariable());
			getQuaternion(transExpMap[now->attribute.op], interPara[0], interPara[1], interPara[2]);
		}
		else if (now->attribute.op == PLUS || now->attribute.op == MINUS) {
			getQuaternion(ASSIGN_, interPara[3], interPara[0], empty);
			strcpy(interPara[2], newVariable());
			if (interPara[4][1] != '#')
				getQuaternion(ASSIGN_, interPara[4], interPara[1], empty),
				getQuaternion(transExpMap[now->attribute.op], interPara[0], interPara[1], interPara[2]);
			else
				getQuaternion(transExpMap[now->attribute.op], interPara[0], interPara[4], interPara[2]);
		}
		break;
	}
	case _CONST: {
		sprintf(interPara[2], "%.2f", now->attribute.value); 
		int pos = 0;
		while (interPara[2][pos] != '.') pos++;
		if (interPara[2][pos + 1] == '0' && interPara[2][pos + 2] == '0')
			interPara[2][pos] = '\0';
		break;
	}
	case _ID: 
		strcpy(interPara[2], now->attribute.name); break;
	case _CITEARRAY:
		strcpy(interPara[2], now->son[0]->attribute.name); break;
	case _FUNC: {
		sprintf(interPara[3], "%d", paraCnt(now->son[1]));
		getQuaternion(ARGSS_, interPara[3], empty, empty);

		Node *next = now->son[1];
		while (next != NULL) {
			strcpy(interPara[0], newVariable());
			strcpy(interPara[1], transExp(next));
			if (next->detailedType.expType == _CITEARRAY) {
				if (next->son[1]->detailedType.expType == _CONST)
					sprintf(para[3], "%.0f", next->son[1]->attribute.value);
				else
					strcpy(para[3], next->son[1]->attribute.name);
				getQuaternion(ARGS_, interPara[1], para[3], empty);
			}
			else 
				getQuaternion(ARGS_, interPara[1], empty, empty);
			next = next->bro;
		}
		getQuaternion(CALLFUNC_, now->son[0]->attribute.name, empty, empty);
		strcpy(interPara[2], newVariable());
		getQuaternion(FUNCRETURNS_, now->son[0]->attribute.name, interPara[2], empty);
		break;
	}
	}
	return interPara[2];
}

static char *newVariable() {
	sprintf(charBuffOfVariable, "t#%d", variableCnt++); return charBuffOfVariable;
}

static char *newLable() {
	sprintf(charBuffOfLable, "L%d", lableCnt++); return charBuffOfLable;
}

static int paraCnt(Node *now) {
	int cnt = 0;
	if (now != NULL) {
		cnt = 1;
		while (now->bro != NULL) cnt += 1, now = now->bro;
	} return cnt;
}

static void printQuaternion(quaternion *now) {
	while (now != NULL) {
		if (now->opTypes == IFS_ ||
			now->opTypes == IFE_ ||
			now->opTypes == MAINE_ ||
			now->opTypes == MAINS_ ||
			now->opTypes == FORS_ ||
			now->opTypes == FORE_ ||
			now->opTypes == WHILES_ ||
			now->opTypes == WHILEE_) {
			now = now->next; continue;
		}
		/*if (now->opTypes == DEFINEPARAS_) {
			now = now->next; continue;
		}*/
		printf("(");
		printf("%-12s,", opTypeMap[now->opTypes]);
		for (int pos = 0; pos < 3; pos++) {
			if (now->addr[pos].addressTypes == EMPTYS_)
				printf("%-12s", "_");
			else
				printf("%-12s", now->addr[pos].name);
			if (pos != 2)
				printf(",");
		}
		now = now->next;
		printf(")\n");
	}
}

static void subDestoryQuaternion(quaternion *now) {
	if (now->next != NULL)
		subDestoryQuaternion(now->next);
	free(now);
}

void destoryQuaternion() {
	subDestoryQuaternion(quaternionHead);
}

void initTransExpMap() {
	transExpMap[TIMES] = TIMES_;
	transExpMap[PLUS] = ADDS_;
	transExpMap[MINUS] = MINUS_;
	transExpMap[DIVIDE] = DIVS_;
	transExpMap[MODE] = MODES_;
	transExpMap[NOTEQU] = NOTEQUAL_;
	transExpMap[IFEQU] = IFEQUAL_;
	transExpMap[SMALLER] = SMALLER_;
	transExpMap[SMALLEREQU] = SMALLEREQUAL_;
	transExpMap[BIGGER] = BIGGER_;
	transExpMap[BIGGEREQU] = BIGGEREQUAL_;
	strcpy(opTypeMap[IFFALSE_], "IFFALSE");
	strcpy(opTypeMap[ASSIGN_], "=");
	strcpy(opTypeMap[ARRASSIGN_], "=[]");
	strcpy(opTypeMap[LAB_], "LAB");
	strcpy(opTypeMap[TIMES_], "*");
	strcpy(opTypeMap[ADDS_], "+");
	strcpy(opTypeMap[MODES_], "%");
	strcpy(opTypeMap[MINUS_], "-");
	strcpy(opTypeMap[CALLFUNC_], "CALLFUNC");
	strcpy(opTypeMap[DEFINEFUNCS_], "DEFINEFUNCS");
	strcpy(opTypeMap[MAINS_], "MAINS");
	strcpy(opTypeMap[DIVS_], "/");
	strcpy(opTypeMap[IFEQUAL_], "==");
	strcpy(opTypeMap[NOTEQUAL_], "!=");
	strcpy(opTypeMap[BIGGER_], ">");
	strcpy(opTypeMap[BIGGEREQUAL_], ">=");
	strcpy(opTypeMap[SMALLER_], "<");
	strcpy(opTypeMap[SMALLEREQUAL_], "<=");
	strcpy(opTypeMap[RETURNS_], "RETURNS");
	strcpy(opTypeMap[JUMPS_], "JUMP");
	strcpy(opTypeMap[ARGS_], "ARG");
	strcpy(opTypeMap[ARGSS_], "ARGS");
	strcpy(opTypeMap[MAINS_], "MAINS");
	strcpy(opTypeMap[MAINE_], "MAINE");
	strcpy(opTypeMap[DEFINEFUNCS_], "DEFINEFUNCS");
	strcpy(opTypeMap[DEFINEFUNCE_], "DEFINEFUNCE");
	strcpy(opTypeMap[FUNCRETURNS_], "FUNCRETURN");
	strcpy(opTypeMap[DEFINEPARAS_], "DEFINEPARA");
	strcpy(opTypeMap[IFS_], "IFS");
	strcpy(opTypeMap[IFE_], "IFE");
	strcpy(opTypeMap[WHILES_], "WHILES");
	strcpy(opTypeMap[WHILEE_], "WHILEE");
	strcpy(opTypeMap[FORS_], "FORS");
	strcpy(opTypeMap[FORE_], "FORE");
	strcpy(opTypeMap[ADDNEXT_], "ADDNEXT");
	strcpy(opTypeMap[MINUSNEXT_], "MINUSNEXT");
	strcpy(opTypeMap[FORCMP_], "FORCMP");
	strcpy(opTypeMap[INC_], "INC");
	strcpy(opTypeMap[DEC_], "DEC");
}

