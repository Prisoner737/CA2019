// Report05.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "stdio.h"

int main()
{
	printf("test");
}

int ALU(int X, int Y, int C, int* Z) {

	int c32, c10;
	int ret;
	c32 = (C >> 2) & 3;
	c10 = C & 3;

	if (c32 == 0) {//shift
		ret = shiftOperation(X, Y, c10);
	}
	else if (c32 == 1) {//set less
		ret = checkSetLess(X, Y);
	}
	else if (c32 == 2) {//addSubstract
		*Z = checkZero(X, Y, c10, &ret);
	}
	else {//logic
		ret = logicOpreation(X, Y, c10);
	}

	return ret;
}

int logicOpreation(int X, int Y, int C) {

	if (C < 0 || C>3) {
		printf("Error in logic operation\n");
		exit(1);
	}
	
	if (C == 0) {
		return X & Y;
	}
	else if (C == 1) {
		return X | Y;
	}
	else if (C == 2) {
		return X ^ Y;
	}
	else {
		return ~(X ^ Y);
	}
}

int addSubstract(int X, int Y, int C) {
	int ret;
	if (C < 0 || C>1) {
		printf("Error in addSub operation\n");
		exit(1);
	}

	if (C == 0) {
		ret = X + Y;
	}
	else {
		ret = X - Y;
	}
	return ret;
}

int shiftOperation(int V, int Y, int C) {

	int ret; 

	int sVal = V;
	unsigned int uVal = V;

	if (C < 0 || C>3) {
		printf("Error in shift operation\n");
		exit(1);
	}

	if (C == 0) {
		ret = V;
	}
	else if (C == 1) {
		ret = uVal << Y;
	}
	else if (C == 2) {
		ret = uVal >> Y;
	}
	else {
		ret = sVal >> Y;
	}

	return ret;
}

int checkZero(int X, int Y, int C, int* result) {

	int ret = addSubstract(X, Y, C);
	*result = ret;

	if (ret == 0)
		return 1;
	else
		return 0;

}

int checkSetLess(int X, int Y) {

	if (X - Y < 0)
		return 1;
	else
		return 0;
}