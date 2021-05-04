#pragma once
#include "OPTIMIZE.H"

static void startOptimize(quaternion *);
static void printOptimize(quaternion *);

void OPTIMIZE() {
	startOptimize(quaternionHead);
#ifdef IFMS
	printOptimize(quaternionHead);
#endif
}

static void startOptimize(quaternion *root) {
	quaternion *now = root->next, *first = now->next, *second = first->next, *pre = now;
	char interPara[5][MAXTOKENLEN];
	while (second != NULL) {
		if (first->opTypes == ASSIGN_ || first->opTypes == FUNCRETURNS_) {
			if (second->opTypes == ASSIGN_) {
				strcpy(interPara[0], first->addr[1].name);
				if (strcmp(first->addr[1].name, second->addr[0].name) == 0 && interPara[0][0] == 't' && interPara[0][1] == '#' && 
					strlen(first->addr[2].name) == 0 && strlen(second->addr[2].name) == 0) {
					/*first->addr[2].addressTypes = second->addr[2].addressTypes;
					strcpy(first->addr[2].name, second->addr[2].name);*/
					strcpy(first->addr[1].name, second->addr[1].name);
					first->next = second->next;
					free(second);
					second = first->next;
				}
				else now = first, first = second, second = second->next, pre = pre->next;;
			}
			else if (first->opTypes == ASSIGN_ && first->addr[0].name[1] == '#' && first->addr[1].name[1] == '#' && strcmp(second->addr[1].name, first->addr[1].name) == 0
				&& (second->opTypes == BIGGEREQUAL_ || second->opTypes == BIGGER_ || second->opTypes == SMALLER_ || second->opTypes == SMALLEREQUAL_)) {
				strcpy(second->addr[1].name, first->addr[0].name);
				pre->next = second;
				free(first);
				first = second;
				second = second->next;
			}
			else {
				for (int pos = 0; pos < 3; pos++)
					strcpy(interPara[pos], first->addr[pos].name);
				if (strcmp(first->addr[1].name, second->addr[0].name) == 0 && interPara[1][0] == 't' && interPara[1][1] == '#' &&
					(interPara[2][0] == '\0' || interPara[0][1] == '#') && second->opTypes != IFFALSE_ && second->opTypes != ARRASSIGN_) {
					strcpy(second->addr[0].name, first->addr[0].name);
					now->next = first->next;
					free(first);
					first = now->next;
					second = second->next;
				}
				else now = first, first = second, second = second->next, pre = pre->next;;
			}
		}
		else {
			strcpy(interPara[0], first->addr[2].name);
			if (second->opTypes == ASSIGN_ && 
				first->addr[2].addressTypes != EMPTYS_ &&
				strcmp(first->addr[2].name, second->addr[0].name) == 0 && interPara[0][0] == 't' && interPara[0][1] == '#') {

				strcpy(first->addr[2].name, second->addr[1].name);
				first->next = second->next;
				free(second);
				second = first->next;
			}
			else now = first, first = second, second = second->next, pre = pre->next;;
		}
	}
}

static void printOptimize(quaternion *root) {
	printf("\nOPTIMIZE:\n");
	quaternion *now = root->next;
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
	printf("%s", "\n");
}