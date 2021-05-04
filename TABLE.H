#pragma once
#ifndef _TABLE_H_
#define _TABLE_H_

#pragma once
#include "GLOBALS.H"
#include "TRANS.H"

#define offset 4
#define bucketSIZE 401

extern Node *syntaxTreeRoot;

typedef struct tableNode {
	char name[MAXTOKENLEN >> 1];
	char type[MAXTOKENLEN >> 1];
	int first, second;
	struct tableNode *next;
} tableNode;

typedef struct record {
	int pos;
	struct record *next;
} record;

typedef struct bucketList {
	char name[MAXTOKENLEN >> 1];
	int pos;
	record *recordList;
	struct bucketList *next;
} buckedList;

void TABLE();

#endif;