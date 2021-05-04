#pragma once
#include "TABLE.H"

static int hashCode(char[]);
static void traverse(Node *, void(*pre)(Node *), void(*suf)(Node *));
static void NULLFUNC(Node *);
static insertNode(Node *);
static void buildTable(Node *);
static buckedList *get(char[]);
static void insert(char[], int, int);
static void printTable();

static firstPos;
static buckedList *bucked[bucketSIZE];

static int hashCode(char key[]) {
	int w = 0;
	for (int pos = 0; key[pos]; pos++)
		w = ((w << offset) + key[pos]) % bucketSIZE;
	return w;
}

void TABLE() {
	buildTable(syntaxTreeRoot);
}

static insertNode(Node *now) {
	switch (now->nodeType) {
	case _STMT:
		switch (now->detailedType.stmtType) {
		case _ASSIGN:
		case _DEFINEPARA:
			if (get(now->attribute.name) == NULL)
				insert(now->attribute.name, now->LINE, firstPos++);
			else 
				insert(now->attribute.name, now->LINE, 0);
			break;
		}
		break;
	case _EXP:
		switch (now->detailedType.expType) {
		case _ID:
			if (get(now->attribute.name) == NULL)
				insert(now->attribute.name, now->LINE, firstPos++);
			else
				insert(now->attribute.name, now->LINE, 0);
			break;
		}
		break;
	}
}

static void NULLFUNC(Node * now) {
	return;
}

static void buildTable(Node *root) {
	traverse(syntaxTreeRoot, insertNode, NULLFUNC);
#ifdef IFMS
	printTable();
#endif
}

static void traverse(Node *now, void(*pre)(Node *), void(*suf)(Node *)) {
	if (now != NULL) {
		pre(now);
		for (int pos = 0; pos < 4; pos++)
			traverse(now->son[pos], pre, suf);
		suf(now);
		traverse(now->bro, pre, suf);
	}
}

static buckedList *get(char str[]) {
	int w = hashCode(str);
	buckedList *now = bucked[w];
	while (now != NULL && strcmp(str, now->name) != 0)
		now = now->next;
	return now;
}

static void insert(char str[], int posOfLine, int pos) {
	buckedList *now = get(str);
	if (now == NULL) {
		now = (buckedList*)malloc(sizeof(buckedList));
		strcpy(now->name, str);
		now->recordList = (record*)malloc(sizeof(record));
		now->recordList->pos = posOfLine;
		now->recordList->next = NULL;
		now->pos = pos;
		int w = hashCode(str);
		now->next = bucked[w];
		bucked[w] = now;
	}
	else {
		record *ptrLine = now->recordList;
		while (ptrLine->next != NULL)
			ptrLine = ptrLine->next;
		ptrLine->next = (record*)malloc(sizeof(record));
		ptrLine->next->pos = posOfLine;
		ptrLine->next->next = NULL;
	}
}

static void printTable() {
	for (int pos = 0; pos < bucketSIZE; pos++) {
		buckedList *now = bucked[pos];
		while (now != NULL) {
			record *e = now->recordList;
			printf("%-14s ", now->name);
			printf("%-8d  ", now->pos);
			while (e != NULL) {
				printf("%4d ", e->pos); e = e->next;
			}
			printf("\n"); now = now->next;
		}
	}
}

void destryTable() {
	for (int pos = 0; pos < bucketSIZE; pos++) {
		buckedList *nowBucked = bucked[pos], *nextBucket = NULL;
		while (nowBucked != NULL) {
			record *now = nowBucked->recordList, *next = NULL;
			while (now != NULL) {
				next = now->next; free(now); now = next;
			}
			nextBucket = nowBucked->next;
			free(nowBucked);
			nowBucked = nextBucket;
		}
	}
}

