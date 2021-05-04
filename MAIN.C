#pragma once
#include "GLOBALS.H"
#include "SCAN.H"
#include "PARSE.H"
#include "TRANS.H"
#include "OPTIMIZE.H"
#include "ASSEM.H"
 
int main(int argc, char *argv[]) {
#ifdef IFMSS
	SCAN(argv[1]);
#else
	SCAN("D:\\VisualStudio\\compiler\\compiler\\compiler\\DEMO.txt");
#endif
	PARSE();
	TRANS();
	OPTIMIZE();
	ASSEMBLY();

	destoryASM();
	destoryQuaternion();
	destryTree();
}
