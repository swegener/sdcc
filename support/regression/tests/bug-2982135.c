/*
    bug 2982135
*/

#include <testfwk.h>

char* fp;

char* f1(int n){
	char* tmp=fp;
	fp+=n;
	return tmp;
}

char* f2(int n){
	char* tmp=fp;
	fp+=3;
	return tmp;
}

void test2982135(void)
{
	fp = (char *)42;
	f1(23);
	ASSERT(fp == (char *)42);
	f2(23);
	ASSERT(fp == (char *)42);
}

