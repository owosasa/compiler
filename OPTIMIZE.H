#pragma once
#ifndef _OPTIMIZE_H_
#define _OPTIMIZE_H_

#include "TRANS.H"

extern quaternion *quaternionHead;
extern char opTypeMap[CONSTANT >> 1][17];

void OPTIMIZE();

#endif
