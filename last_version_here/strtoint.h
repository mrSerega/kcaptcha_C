#include <string.h>
#include <stdio.h>
#include <cmath>

using namespace std;

#ifndef CASTS_H_23082016
#define CASTS_H_23082016

int strToInt(const char* str){
	int len = strlen(str);
	int sum =0;
	for(register int i=len-1;i>=0;i--){
		int n = (int)str[i]-48;
		sum+=n*pow(10,len-i-1);
	}
	return sum;
}

int charToOrder(char c){//return order of letter in alphabet(default)
	return int(c)-97;
}

#endif
