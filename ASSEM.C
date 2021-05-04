#pragma once
#include "ASSEM.H"
#include "TABLE.H"

static void opTableNode(tableNode **, tableNode **, char[], char[], char[]);
static int searchTable(tableNode *, char[]);
static void freeRegister(char[]);
static void getFunctions(quaternion*);
static void getGlobalVariable(quaternion *);
static void BUILDASM(quaternion *);
static char *getRegiserts();
static void defineASM(quaternion *);
static void assignASM(quaternion *);
static void lableASM(quaternion *);
static void jumpASM(quaternion *);
static void returnASM(quaternion *);
static void timeASM(quaternion *);
static void divsASM(quaternion *);
static void addsASM(quaternion *);
static void modeASM(quaternion *);
static void minuASM(quaternion *);
static void smallASM(quaternion *);
static void smallEqualASM(quaternion *);
static void bigASM(quaternion *);
static void bigEqualASM(quaternion *);
static void ifEqualASM(quaternion *);
static void notEqualASM(quaternion *);
static void callFunASM(quaternion *, int);
static void funParaASM(quaternion *);
static void arrAssignASM(quaternion *);
static void incASM(quaternion *);
static void decASM(quaternion *);

static char empty[MAXTOKENLEN] = { '\0' };
int AXFLAG, BXFLAG, CXFLAG, DXFLAG, SIFLAG;
char functionTable[MAXTOKENLEN][MAXTOKENLEN]; int functionTableInd;
char assignTable[MAXTOKENLEN][MAXTOKENLEN];
tableNode *GHead, *GTail; int scope[MAXTOKENLEN], scopeInd = 1, GS;
char para[5][MAXTOKENLEN], OUTFILE[CONSTANT][MAXTOKENLEN]; int OUTIND;

static void initPara() {
	for (int pos = 0; pos < 5; pos++) strcpy(para[pos], empty);
}

static void opTableNode(tableNode **Head, tableNode **Tail, char type[], char name[], char num[]) {
	tableNode *newNode = (tableNode*)malloc(sizeof(tableNode));
	newNode->next = NULL;
	strcpy(newNode->name, name);
	strcpy(newNode->type, type);
	int sze = 1;
	if (strlen(num) != 0) {
		sze = 0;
		for (int pos = 0; num[pos]; pos++) sze = sze * 10 + (num[pos] - '0');
	}
	newNode->first = sze;

	if (*Head == NULL)
		*Head = *Tail = newNode;
	else
		(*Tail)->next = newNode, *Tail = newNode;
}

static int searchTable(tableNode *Head, char name[]) {
	tableNode *nexts = Head; int cnt = 0, pre = 0, suf = 0;
	for (int pos = 1; pos <= scopeInd; pos++) suf += scope[pos];
	pre = suf - scope[scopeInd];
	while (nexts != NULL && cnt <= suf) {
		cnt += nexts->first;
		if ((cnt > pre || cnt <= GS) && strcmp(nexts->name, name) == 0) break;
		nexts = nexts->next;
	}
	if (nexts == NULL) return -1;
	return cnt <= GS ? cnt : (cnt - pre);
}

static void freeRegister(char name[]) {
	if (strcmp(name, "AX") == 0 || strcmp(name, "EAX") == 0) AXFLAG = 0;
	if (strcmp(name, "BX") == 0 || strcmp(name, "EBX") == 0) BXFLAG = 0;
	if (strcmp(name, "CX") == 0 || strcmp(name, "ECX") == 0) CXFLAG = 0;
}

static void FILEOUT() {
	FILE *F = freopen("F:\\ASM\\DEMO.asm", "w", stdout);
	for (int pos = 0; pos < OUTIND; pos++) printf("%s", OUTFILE[pos]);
	fclose(F);
}

static void ASMPTR() {
	for (int pos = 0; pos < OUTIND; pos++) printf("%s", OUTFILE[pos]);
}

void ASSEMBLY() {
	sprintf(OUTFILE[OUTIND++], ".MODEL SMALL\n");
	sprintf(OUTFILE[OUTIND++], ".386\n");
	sprintf(OUTFILE[OUTIND++], ".STACK 100H\n");
	sprintf(OUTFILE[OUTIND++], ".DATA\n");
	sprintf(OUTFILE[OUTIND++], ".CODE\n");
	sprintf(OUTFILE[OUTIND++], ".STARTUP\n");
	quaternion *now = quaternionHead->next;
	getGlobalVariable(now);
	while (now->opTypes != MAINS_) now = now->next;
	quaternion *suf = now;
	while (suf != NULL && suf->opTypes != MAINE_) {
		if (suf->opTypes == DEFINEPARAS_) BUILDASM(suf); suf = suf->next;
	}
	sprintf(OUTFILE[OUTIND++], "MAIN:\n");
	sprintf(OUTFILE[OUTIND++], "	PUSH BP\n");
	sprintf(OUTFILE[OUTIND++], "	MOV BP,SP\n");
	sprintf(OUTFILE[OUTIND++], "	SUB SP,%d\n\n", scope[1] << 1);
	while (now->opTypes != MAINE_) {
		if (now->opTypes != DEFINEPARAS_) BUILDASM(now); now = now->next;
	}
	sprintf(OUTFILE[OUTIND++], ".EXIT\n");
	getFunctions(quaternionHead);
	sprintf(OUTFILE[OUTIND++], "END\n");
#ifdef IFMSS
	FILEOUT();
#else 
	ASMPTR();
#endif
}

static void getFunctions(quaternion *root) {
	quaternion *nexts = root->next;
	while (nexts != NULL) {
		if (nexts->opTypes == DEFINEFUNCS_) {
			strcpy(functionTable[functionTableInd++], nexts->addr[0].name);
			sprintf(OUTFILE[OUTIND++], "%s:\n", nexts->addr[0].name);

			int ARGS = 0; sscanf(nexts->addr[1].name, "%d", &ARGS);

			scopeInd++;
			quaternion *suf = nexts;
			while (suf != NULL && suf->opTypes != DEFINEFUNCE_) {
				if (suf->opTypes == DEFINEPARAS_) BUILDASM(suf); suf = suf->next;
			}

			sprintf(OUTFILE[OUTIND++], "	SUB SP,%d\n", (scope[scopeInd] - ARGS - 1) << 1);

			suf = nexts;
			while (suf != NULL && suf->opTypes != DEFINEFUNCE_) {
				if (suf->opTypes != DEFINEPARAS_) BUILDASM(suf); suf = suf->next;
			}
			sprintf(OUTFILE[OUTIND++], "	ADD SP,%d\n", (scope[scopeInd] - ARGS - 1) << 1);
			sprintf(OUTFILE[OUTIND++], "	RET\n");
		}
		nexts = nexts->next;
	}
}

static void getGlobalVariable(quaternion *now) {
	quaternion *nexts = now;
	/*while (nexts->opTypes != DEFINEFUNCS_ && nexts->opTypes != MAINS_)*/
	while (nexts != NULL) {
		if (nexts->opTypes == DEFINEPARAS_)	defineASM(nexts);
		else if (nexts->opTypes == MAINS_) break;
		else if (nexts->opTypes == DEFINEFUNCS_) {
			while (nexts != NULL && nexts->opTypes != DEFINEFUNCE_)
				nexts = nexts->next;
		}
		if (nexts != NULL) nexts = nexts->next;
	}
	GS = scope[scopeInd];
	freeRegister("AX"), freeRegister("BX"), freeRegister("CX");
}

static void BUILDASM(quaternion *now) {
	switch (now->opTypes) {
	case DEFINEPARAS_: defineASM(now); break;
		/*case DEFINEFUNCS_: defineFunASM(now); break;*/
	case ASSIGN_: assignASM(now); break;
	case LAB_: lableASM(now); break;
	case JUMPS_: jumpASM(now); break;
	case RETURNS_: returnASM(now); break;
	case MINUS_: minuASM(now); break;
	case ADDS_: addsASM(now); break;
	case DIVS_: divsASM(now); break;
	case TIMES_: timeASM(now); break;
	case MODES_: modeASM(now); break;
	case SMALLER_: smallASM(now); break;
	case SMALLEREQUAL_: smallEqualASM(now); break;
	case BIGGER_: bigASM(now); break;
	case BIGGEREQUAL_: bigEqualASM(now); break;
	case IFEQUAL_: ifEqualASM(now); break;
	case NOTEQUAL_: notEqualASM(now); break;
	case ARRASSIGN_: arrAssignASM(now); break;
	case INC_: incASM(now); break;
	case DEC_: decASM(now); break;
		/*case CALLFUNC_: callFunASM(now); break;*/
	case ARGSS_: funParaASM(now); break;
	}
}

static char *getRegiserts() {
	if (AXFLAG == 0) { AXFLAG = 1; return "AX"; }
	if (BXFLAG == 0) { BXFLAG = 1; return "BX"; }
	if (CXFLAG == 0) { CXFLAG = 1; return "CX"; }
	printf("Not Exit Regiserts\n"); PAUSE, exit(0);
	return "";
}

static void defineASM(quaternion *now) {
	opTableNode(&GHead, &GTail, now->addr[0].name, now->addr[1].name, now->addr[2].name);
	int sze = 1;
	if (strcmp(now->addr[2].name, empty)) {
		sze = 0; for (int pos = 0; now->addr[2].name[pos]; pos++) sze = sze * 10 + (now->addr[2].name[pos] - '0');
	}
	scope[scopeInd] += sze;
}

static void assignASM(quaternion *now) {
	initPara(); int offs = 0;
	strcpy(para[1], now->addr[0].name);
	strcpy(para[2], now->addr[1].name);
	if (isNum(para[1][0]) || (para[1][0] == '-' && isNum(para[1][1]))) {
		if (isLetter(para[2][0]) && para[2][1] != '#') {
			int Ind = searchTable(GHead, now->addr[1].name);
			if (Ind != -1) {
				for (int pos = 0; now->addr[2].name[pos]; pos++) offs = offs * 10 + now->addr[2].name[pos] - '0';
				if (strcmp(assignTable[Ind], empty) == 0) {
					strcpy(assignTable[Ind], now->addr[0].name);
					sprintf(OUTFILE[OUTIND++], "	MOV AX,%s\n", assignTable[Ind]);
					sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],AX\n", (Ind + offs) << 1);
				}
				else {
					sprintf(OUTFILE[OUTIND++], "	MOV AX,%s\n", now->addr[0].name);
					sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],AX\n", Ind << 1);
				}
				freeRegister("AX");
			}
		}
		else {
			strcpy(para[0], getRegiserts());
			sprintf(OUTFILE[OUTIND++], "	MOV %s,%s\n", para[0], para[1]);
			/*freeRegister(para[0]);*/
		}
	}
	else if (isLetter(para[1][0]) && para[1][1] != '#' &&
		isLetter(para[2][0]) && para[2][1] == '#') {
		int Ind = searchTable(GHead, para[1]);
		if (Ind != -1) {
			strcpy(para[0], getRegiserts());
			if (strlen(now->addr[2].name) == 0) {
				/* not arr */
				sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d]\n", para[0], Ind << 1); return;
			}
			else if (isNum(now->addr[2].name[0])) {
				/* arrInd is num */
				for (int pos = 0; now->addr[2].name[pos]; pos++) offs = offs * 10 + now->addr[2].name[pos] - '0';
				sprintf(OUTFILE[OUTIND++], "	MOV SI,%d\n", offs << 1);
				sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d+SI]\n", para[0], Ind << 1);
			}
			else if (isLetter(now->addr[2].name[0])) {
				/* arrInd is var */
				if (searchTable(GHead, now->addr[2].name) == -1) { printf("Undeclared identifier [%s]\n", now->addr[2].name), PAUSE, exit(0); }
				sprintf(OUTFILE[OUTIND++], "	MOV SI,[BP-%d]\n", searchTable(GHead, now->addr[2].name) << 1);
				sprintf(OUTFILE[OUTIND++], "	ADD SI,SI\n");
				sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d+SI]\n", para[0], Ind << 1);
			}
			else
				printf("Assign Error\n"), PAUSE, exit(0);
		}
		else { printf("Undeclared identifier [%s]\n", para[1]), PAUSE, exit(0); }
	}
	else if (isLetter(para[1][0]) && para[1][1] != '#' &&
		isLetter(para[2][0]) && para[2][1] != '#') {
		int Ind = searchTable(GHead, para[1]);
		strcpy(para[0], getRegiserts());
		if (Ind != -1) {
			sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d]\n", para[0], Ind << 1);
		}
		else { printf("Undeclared identifier [%s]\n", para[1]), PAUSE, exit(0); }
		Ind = searchTable(GHead, para[2]);
		if (Ind != -1) {
			/* offs is arr */
			for (int pos = 0; now->addr[2].name[pos]; pos++) offs = offs * 10 + now->addr[2].name[pos] - '0';
			sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],%s\n", (Ind + offs) << 1, para[0]);
			/* this */
			freeRegister(para[0]);
		}
		else { printf("Undeclared identifier [%s]\n", para[2]), PAUSE, exit(0); }
	}
	else {
		int Ind = searchTable(GHead, now->addr[1].name);
		if (Ind == -1) printf("Undeclared identifier [%s]\n", now->addr[1].name), PAUSE, exit(0);
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],%s\n", Ind << 1, AXFLAG ? "AX" : "CX");
		freeRegister(AXFLAG ? "AX" : "CX");
	}
}

static void arrAssignASM(quaternion *now) {
	initPara();
	for (int pos = 0; pos < 3; pos++)
		strcpy(para[pos], now->addr[pos].name);
	if (isNum(para[0][0]) || (para[0][0] == '-' && isNum(para[0][1]))) {
		int offs = 0;
		for (int pos = 0; para[0][pos]; pos++) offs = offs * 10 + para[0][pos] - '0';
		sprintf(OUTFILE[OUTIND++], "	MOV AX,%d\n", offs);
	}
	else if (isLetter(para[0][0]) && para[0][1] != '#') {
		int Ind = searchTable(GHead, para[0]);
		if (Ind == -1) printf("Undeclared identifier [%s]\n", para[0]), PAUSE, exit(0);
		sprintf(OUTFILE[OUTIND++], "	MOV AX,[BP-%d]\n", Ind << 1);
	}
	else {
		/* isRegister */
	}
	if (isNum(para[2][0]) || (para[2][0] == '-' && isNum(para[2][1]))) {
		int offs = 0;
		for (int pos = 0; para[2][pos]; pos++) offs = offs * 10 + para[2][pos] - '0';
		sprintf(OUTFILE[OUTIND++], "	MOV SI,%d\n", offs << 1);
		int Ind = searchTable(GHead, para[1]);
		if (Ind == -1) { printf("Undeclared identifier [%s]\n", para[1]), PAUSE, exit(0); }
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d+SI],AX\n", Ind << 1);
		freeRegister("AX");
	}
	else {
		int Ind = searchTable(GHead, para[2]);
		if (Ind == -1) { printf("Undeclared identifier [%s]\n", para[2]), PAUSE, exit(0); }
		sprintf(OUTFILE[OUTIND++], "	MOV SI,[BP-%d]\n", Ind << 1);
		sprintf(OUTFILE[OUTIND++], "	ADD SI,SI\n");
		Ind = searchTable(GHead, para[1]);
		if (Ind == -1) { printf("Undeclared identifier [%s]\n", para[1]), PAUSE, exit(0); }
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d+SI],AX\n", Ind << 1);
		freeRegister("AX");
	}
}

static void lableASM(quaternion *now) {
	sprintf(OUTFILE[OUTIND++], "%s:\n", now->addr[0].name);
}

static void jumpASM(quaternion *now) {
	sprintf(OUTFILE[OUTIND++], "	JMP %s\n", now->addr[0].name);
}

static void returnASM(quaternion *now) {
	initPara();
	strcpy(para[0], now->addr[0].name);
	if (isNum(para[0][0] || (isNum(para[0][1]) && para[0][0] == '-'))) {
		int offs = 0;
		for (int pos = 0; para[0][pos]; pos++) offs = offs * 10 + para[0][pos] - '0';
		sprintf(OUTFILE[OUTIND++], "	MOV DI,%d\n", offs);
	}
	else if (isLetter(para[0][0])) {
		int Ind = searchTable(GHead, para[0]);
		sprintf(OUTFILE[OUTIND++], "	MOV DI,[BP-%d]\n", Ind << 1);
	}
}

static void timeASM(quaternion *now) {
	char AX_BX[MAXTOKENLEN]; initPara();
	strcpy(para[0], now->addr[2].name);
	if (CXFLAG == 1) {
		sprintf(OUTFILE[OUTIND++], "	MUL CX\n"); strcpy(AX_BX, "BX");
	}
	else {
		sprintf(OUTFILE[OUTIND++], "	MUL BX\n");	AXFLAG = 1;	strcpy(AX_BX, "AX");
	}
	if (para[0][1] != '#') {
		int Ind = searchTable(GHead, para[0]);
		if (Ind == -1) { printf("Undeclared identifier [%s]\n", para[0]), PAUSE, exit(0); }
		/* this */
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],%s\n", Ind << 1, AX_BX);
		if (strcmp(AX_BX, "AX") == 0)
			freeRegister("AX"), freeRegister("BX");
		else
			freeRegister("BX"), freeRegister("CX");
	}
	else
		strcmp(AX_BX, "AX") == 0 ? freeRegister("BX") : freeRegister("CX");
}

static void modeASM(quaternion *now) {
	char AX_BX[MAXTOKENLEN]; initPara();
	strcpy(para[0], now->addr[2].name);
	sprintf(OUTFILE[OUTIND++], "	MOV DX,0\n");
	if (CXFLAG == 1) {
		sprintf(OUTFILE[OUTIND++], "	DIV CX\n"); strcpy(AX_BX, "BX");
	}
	else {
		sprintf(OUTFILE[OUTIND++], "	DIV BX\n");	AXFLAG = 1;	strcpy(AX_BX, "AX");
	}
	if (para[0][1] != '#') {
		int Ind = searchTable(GHead, para[0]);
		if (Ind == -1) { printf("Undeclared identifier [%s]\n", para[0]), PAUSE, exit(0); }
		/* this */
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],DX\n", Ind << 1);
		if (strcmp(AX_BX, "AX") == 0)
			freeRegister("AX"), freeRegister("BX");
		else
			freeRegister("BX"), freeRegister("CX");
	}
	else
		strcmp(AX_BX, "AX") == 0 ? freeRegister("BX") : freeRegister("CX");
}

static void divsASM(quaternion *now) {
	char AX_BX[MAXTOKENLEN]; initPara();
	strcpy(para[0], now->addr[2].name);
	if (CXFLAG == 1) {
		sprintf(OUTFILE[OUTIND++], "	DIV CX\n"); strcpy(AX_BX, "BX");
	}
	else {
		sprintf(OUTFILE[OUTIND++], "	DIV BX\n");	AXFLAG = 1;	strcpy(AX_BX, "AX");
	}
	if (para[0][1] != '#') {
		int Ind = searchTable(GHead, para[0]);
		if (Ind == -1) { printf("Undeclared identifier [%s]\n", para[0]), PAUSE, exit(0); }
		/* this */
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],%s\n", Ind << 1, AX_BX);
		if (strcmp(AX_BX, "AX") == 0)
			freeRegister("AX"), freeRegister("BX");
		else
			freeRegister("BX"), freeRegister("CX");
	}
	else
		strcmp(AX_BX, "AX") == 0 ? freeRegister("BX") : freeRegister("CX");
}

static void addsASM(quaternion *now) {
	char AX_BX[MAXTOKENLEN]; initPara();
	for (int pos = 0; pos < 3; pos++)
		strcpy(para[pos], now->addr[(pos + 1) % 3].name);
	if (isNum(para[2][0]) || (para[2][0] == '-' && isNum(para[2][1]))) {
		if (CXFLAG != 1) {
			sprintf(OUTFILE[OUTIND++], "	MOV BX,%s\n", para[2]);
			sprintf(OUTFILE[OUTIND++], "	ADD AX,BX\n");
			strcpy(AX_BX, "AX");
		}
		else {
			sprintf(OUTFILE[OUTIND++], "	MOV CX,%s\n", para[2]);
			sprintf(OUTFILE[OUTIND++], "	ADD AX,CX\n");
			strcpy(AX_BX, "BX");
		}
		strcmp(AX_BX, "AX") == 0 ? freeRegister("BX") : freeRegister("CX");
	}
	else if (para[2][1] == '#') {
		if (CXFLAG != 1) {
			sprintf(OUTFILE[OUTIND++], "	ADD AX,BX\n");
			strcpy(AX_BX, "AX");
		}
		else {
			sprintf(OUTFILE[OUTIND++], "	ADD AX,CX\n");
			strcpy(AX_BX, "BX");
		}
		strcmp(AX_BX, "AX") == 0 ? freeRegister("BX") : freeRegister("CX");
	}
	else if (isLetter(para[2][0])) {
		int Ind = searchTable(GHead, para[2]);
		if (Ind == -1) { printf("Undeclared identifier [%s]\n", para[2]), PAUSE, exit(0); }
		if (CXFLAG != 1) {
			sprintf(OUTFILE[OUTIND++], "	MOV BX,[BP-%d]\n", Ind << 1);
			sprintf(OUTFILE[OUTIND++], "	ADD AX,BX\n");
			strcpy(AX_BX, "AX");
		}
		else {
			sprintf(OUTFILE[OUTIND++], "	MOV CX,[BP-%d]\n", Ind << 1);
			sprintf(OUTFILE[OUTIND++], "	ADD AX,CX\n");
			strcpy(AX_BX, "BX");
		}
		if (strcmp(AX_BX, "AX") == 0)
			freeRegister("AX"), freeRegister("BX");
		else
			freeRegister("BX"), freeRegister("CX");
	}
	if (para[1][1] != '#') {
		int d = searchTable(GHead, para[1]);
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],AX\n", d << 1);
		freeRegister("AX");
	}
}

static void minuASM(quaternion *now) {
	char AX_BX[MAXTOKENLEN]; initPara();
	for (int pos = 0; pos < 3; pos++)
		strcpy(para[pos], now->addr[(pos + 1) % 3].name);
	if (isNum(para[2][0]) || (para[2][0] == '-' && isNum(para[2][1]))) {
		if (CXFLAG != 1) {
			sprintf(OUTFILE[OUTIND++], "	MOV BX,%s\n", para[2]);
			sprintf(OUTFILE[OUTIND++], "	SUB BX,AX\n");
			sprintf(OUTFILE[OUTIND++], "	MOV AX,BX\n");
			strcpy(AX_BX, "AX");
		}
		else {
			sprintf(OUTFILE[OUTIND++], "	MOV CX,%s\n", para[2]);
			sprintf(OUTFILE[OUTIND++], "	SUB AX,CX\n");
			strcpy(AX_BX, "BX");
		}
		strcmp(AX_BX, "AX") == 0 ? freeRegister("BX") : freeRegister("CX");
	}
	else if (para[2][1] == '#') {
		if (CXFLAG != 1) {
			sprintf(OUTFILE[OUTIND++], "	SUB AX,BX\n");
			strcpy(AX_BX, "AX");
		}
		else {
			sprintf(OUTFILE[OUTIND++], "	SUB AX,CX\n");
			strcpy(AX_BX, "BX");
		}
		strcmp(AX_BX, "AX") == 0 ? freeRegister("BX") : freeRegister("CX");
	}
	else if (isLetter(para[2][0])) {
		int Ind = searchTable(GHead, para[2]);
		if (Ind == -1) { printf("Undeclared identifier [%s]\n", para[2]), PAUSE, exit(0); }
		if (CXFLAG != 1) {
			sprintf(OUTFILE[OUTIND++], "	MOV BX,[BP-%d]\n", Ind << 1);
			sprintf(OUTFILE[OUTIND++], "	SUB AX,BX\n");
			strcpy(AX_BX, "AX");
		}
		else {
			sprintf(OUTFILE[OUTIND++], "	MOV CX,[BP-%d]\n", Ind << 1);
			sprintf(OUTFILE[OUTIND++], "	SUB AX,CX\n");
			strcpy(AX_BX, "BX");
		}
		if (strcmp(AX_BX, "AX") == 0)
			freeRegister("AX"), freeRegister("BX");
		else
			freeRegister("BX"), freeRegister("CX");
	}

	if (para[1][1] != '#') {
		int d = searchTable(GHead, para[1]);
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],AX\n", d << 1);
		freeRegister("AX");
	}
}

static void incASM(quaternion *now) {
	initPara(); int Ind = searchTable(GHead, now->addr[0].name);
	if (Ind == -1) { printf("Undeclared identifier [%s]\n", now->addr[0].name), PAUSE, exit(0); }
	strcpy(para[0], getRegiserts());
	if (isNum(now->addr[1].name[0])) {
		int offs = 0;
		for (int pos = 0; now->addr[1].name[pos]; pos++) offs = offs * 10 + now->addr[1].name[pos] - '0';
		sprintf(OUTFILE[OUTIND++], "	MOV SI,%d\n", offs << 1);
		sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d+SI]\n", para[0], Ind << 1);
		sprintf(OUTFILE[OUTIND++], "	INC %s\n", para[0]);
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d+SI],%s\n", Ind << 1, para[0]);
	}
	else if (isLetter(now->addr[1].name[0])) {
		int offs = searchTable(GHead, now->addr[1].name);
		if (offs == -1) { printf("Undeclared identifier [%s]\n", now->addr[1].name), PAUSE, exit(0); }
		sprintf(OUTFILE[OUTIND++], "	MOV SI,[BP-%d]\n", offs << 1);
		sprintf(OUTFILE[OUTIND++], "	ADD SI,SI\n");
		sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d+SI]\n", para[0], Ind << 1);
		sprintf(OUTFILE[OUTIND++], "	INC %s\n", para[0]);
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d+SI],%s\n", Ind << 1, para[0]);
	}
	else {
		sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d]\n", para[0], Ind << 1);
		sprintf(OUTFILE[OUTIND++], "	INC %s\n", para[0]);
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],%s\n", Ind << 1, para[0]);
	}
	freeRegister(para[0]);
}

static void decASM(quaternion *now) {
	initPara(); int Ind = searchTable(GHead, now->addr[0].name);
	if (Ind == -1) { printf("Undeclared identifier [%s]\n", now->addr[0].name), PAUSE, exit(0); }
	strcpy(para[0], getRegiserts());
	if (isNum(now->addr[1].name[0])) {
		int offs = 0;
		for (int pos = 0; now->addr[1].name[pos]; pos++) offs = offs * 10 + now->addr[1].name[pos] - '0';
		sprintf(OUTFILE[OUTIND++], "	MOV SI,%d\n", offs << 1);
		sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d+SI]\n", para[0], Ind << 1);
		sprintf(OUTFILE[OUTIND++], "	INC %s\n", para[0]);
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d+SI],%s\n", Ind << 1, para[0]);
	}
	else if (isLetter(now->addr[1].name[0])) {
		int offs = searchTable(GHead, now->addr[1].name);
		if (offs == -1) { printf("Undeclared identifier [%s]\n", now->addr[1].name), PAUSE, exit(0); }
		sprintf(OUTFILE[OUTIND++], "	MOV SI,[BP-%d]\n", offs << 1);
		sprintf(OUTFILE[OUTIND++], "	ADD SI,SI\n");
		sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d+SI]\n", para[0], Ind << 1);
		sprintf(OUTFILE[OUTIND++], "	DEC %s\n", para[0]);
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d+SI],%s\n", Ind << 1, para[0]);
	}
	else {
		sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d]\n", para[0], Ind << 1);
		sprintf(OUTFILE[OUTIND++], "	DEC %s\n", para[0]);
		sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],%s\n", Ind << 1, para[0]);
	}
	freeRegister(para[0]);
}

static void smallASM(quaternion *now) {
	quaternion *T = quaternionHead;
	while (T->next != now) T = T->next;
	sprintf(OUTFILE[OUTIND++], "	CMP AX,BX\n");
	/*if (T->addr[0].name[1] != '#' && !isNum(T->addr[0].name[0]))
		sprintf(OUTFILE[OUTIND++], "	JB %s\n", now->next->addr[1].name);
	else*/
		sprintf(OUTFILE[OUTIND++], "	JAE %s\n", now->next->addr[1].name);
	freeRegister("AX");
	freeRegister("BX");
}

static void smallEqualASM(quaternion *now) {
	quaternion *T = quaternionHead;
	while (T->next != now) T = T->next;
	sprintf(OUTFILE[OUTIND++], "	CMP AX,BX\n");
	/*if (T->addr[0].name[1] != '#' && !isNum(T->addr[0].name[0]))
		sprintf(OUTFILE[OUTIND++], "	JBE %s\n", now->next->addr[1].name);
	else*/
		sprintf(OUTFILE[OUTIND++], "	JA %s\n", now->next->addr[1].name);
	freeRegister("AX");
	freeRegister("BX");
}

static void bigASM(quaternion *now) {
	quaternion *T = quaternionHead;
	while (T->next != now) T = T->next;
	sprintf(OUTFILE[OUTIND++], "	CMP AX,BX\n");
	/*if (T->addr[0].name[1] != '#' && !isNum(T->addr[0].name[0]))
		sprintf(OUTFILE[OUTIND++], "	JA %s\n", now->next->addr[1].name);
	else*/
		sprintf(OUTFILE[OUTIND++], "	JBE %s\n", now->next->addr[1].name);
	freeRegister("AX");
	freeRegister("BX");
}

static void bigEqualASM(quaternion *now) {
	quaternion *T = quaternionHead;
	while (T->next != now) T = T->next;
	sprintf(OUTFILE[OUTIND++], "	CMP AX,BX\n");
	/*if (T->addr[0].name[1] != '#' && !isNum(T->addr[0].name[0]))
		sprintf(OUTFILE[OUTIND++], "	JAE %s\n", now->next->addr[1].name);
	else*/
		sprintf(OUTFILE[OUTIND++], "	JB %s\n", now->next->addr[1].name);
	freeRegister("AX");
	freeRegister("BX");
}

static void ifEqualASM(quaternion *now) {
	quaternion *T = quaternionHead;
	while (T->next != now) T = T->next;
	sprintf(OUTFILE[OUTIND++], "	CMP AX,BX\n");
	/*if (T->addr[0].name[1] != '#' && !isNum(T->addr[0].name[0]))
		sprintf(OUTFILE[OUTIND++], "	JE %s\n", now->next->addr[1].name);
	else*/
		sprintf(OUTFILE[OUTIND++], "	JNE %s\n", now->next->addr[1].name);
	freeRegister("AX");
	freeRegister("BX");
}

static void notEqualASM(quaternion *now) {
	quaternion *T = quaternionHead;
	while (T->next != now) T = T->next;
	sprintf(OUTFILE[OUTIND++], "	CMP AX,BX\n");
	/*if (T->addr[0].name[1] != '#' && !isNum(T->addr[0].name[0]))
		sprintf(OUTFILE[OUTIND++], "	JNE %s\n", now->next->addr[1].name);
	else*/
		sprintf(OUTFILE[OUTIND++], "	JE %s\n", now->next->addr[1].name);
	freeRegister("AX");
	freeRegister("BX");
}

static void funParaASM(quaternion *now) {
	int ARGS; initPara();
	sprintf(OUTFILE[OUTIND++], "	PUSH BP\n");
	sscanf(now->addr[0].name, "%d", &ARGS);
	now = now->next;
	for (int pos = 0; pos < ARGS; pos++) {
		if (now->opTypes != ARGS_) printf("Args Error\n"), PAUSE, exit(0);
		strcpy(para[0], getRegiserts());
		if (isNum(now->addr[0].name[0]) || (now->addr[0].name[0] == '-' && isNum(now->addr[0].name[1]))) {
			strcpy(para[1], now->addr[0].name);
			sprintf(OUTFILE[OUTIND++], "	MOV %s,%s\n", para[0], para[1]);
		}
		else if (isLetter(now->addr->name[0]) && strlen(now->addr[1].name) == 0) {
			int Ind = searchTable(GHead, now->addr[0].name);
			if (Ind == -1) { printf("Undeclared identifier [%s]\n", now->addr[0].name), PAUSE, exit(0); }
			sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d]\n", para[0], Ind << 1);
		}
		else if (isLetter(now->addr->name[0]) && isNum(now->addr[1].name[0])) {
			int Ind = searchTable(GHead, now->addr[0].name);
			if (Ind == -1) { printf("Undeclared identifier [%s]\n", now->addr[0].name), PAUSE, exit(0); }
			int offs = 0;
			for (int pos = 0; now->addr[1].name[pos]; pos++) offs = offs * 10 + now->addr[1].name[pos] - '0';
			sprintf(OUTFILE[OUTIND++], "	MOV SI,%d\n", offs << 1);
			sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d+SI]\n", para[0], Ind << 1);
		}
		else if (isLetter(now->addr->name[0]) && isLetter(now->addr[1].name[0])) {
			int Ind = searchTable(GHead, now->addr[0].name);
			if (Ind == -1) { printf("Undeclared identifier [%s]\n", now->addr[0].name), PAUSE, exit(0); }
			int offs = searchTable(GHead, now->addr[1].name);
			sprintf(OUTFILE[OUTIND++], "	MOV SI,[BP-%d]\n", offs << 1);
			sprintf(OUTFILE[OUTIND++], "	ADD SI,SI\n");
			sprintf(OUTFILE[OUTIND++], "	MOV %s,[BP-%d+SI]\n", para[0], Ind << 1);
		}
		else printf("Args Error\n"), PAUSE, exit(0);
		sprintf(OUTFILE[OUTIND++], "	PUSH %s\n", para[0]);
		freeRegister(para[0]);
		now = now->next;
	}
	sprintf(OUTFILE[OUTIND++], "	MOV BP,SP\n");
	sprintf(OUTFILE[OUTIND++], "	ADD BP,%d\n", ARGS << 1);

	callFunASM(now, ARGS);
	sprintf(OUTFILE[OUTIND++], "	ADD SP,%d\n", ARGS << 1);
	sprintf(OUTFILE[OUTIND++], "	POP BP\n");
	if (now->next->opTypes == FUNCRETURNS_) {
		if (now->next->addr[1].name[1] != '#') {
			int Ind = searchTable(GHead, now->next->addr[1].name);
			if (Ind == -1) { printf("Undeclared identifier [%s]\n", now->addr[1].name), PAUSE, exit(0); }
			sprintf(OUTFILE[OUTIND++], "	MOV [BP-%d],DI\n", Ind << 1);
		}
		else {
			strcpy(para[0], getRegiserts()), sprintf(OUTFILE[OUTIND++], "	MOV %s,DI\n", para[0]);
		}
	}
}

static void callFunASM(quaternion *now, int ARGS) {
	sprintf(OUTFILE[OUTIND++], "	CALL %s\n", now->addr[0].name);
}

static void destoryASMs(tableNode *now) {
	if (now != NULL) {
		destoryASMs(now->next); free(now);
	}
}

void destoryASM() {
	destoryASMs(GHead);
}
