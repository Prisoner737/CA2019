#include "stdio.h"

void test();
int ALU(int X, int Y, int C, int* Z);
int logicOpreation(int X, int Y, int C);
int logicOpreation(int X, int Y, int C);
int addSubstract(int X, int Y, int C);
int shiftOperation(int V, int Y, int C);
int checkZero(int X, int Y, int C, int* result);
int checkSetLess(int X, int Y);



int main()
{
	test();

	return 0;
}

void test(void) {

	int outputT = 0;
	int x, y, c, s, z, i;

	printf("Please set your output type<0: integer, 1: Hexadecimal> : ");
	scanf_s("%d", &outputT, sizeof(int));
	printf("\n");

	if (outputT == 0) {
		while (1) {

			z = -1;
			while (1) {
				printf("Please input your commnad <0~15> (-1 for exit) : ");
				scanf_s("%d", &c, sizeof(int));

				if ((c < 16) && (c > -2))
					break;
			}
			if (c == -1)
				break;

			printf("Please input your first(x) value : ");
			scanf_s("%8d", &x, sizeof(int));
			printf("Please input your second(y) value : ");
			scanf_s("%8d", &y, sizeof(int));


			s = ALU(x, y, c, &z);
			printf("X: %8d, Y: %8d\n", x, y);
			if (z == -1) {
				printf("s: %8d \n\n", s);
			}
			else {
				printf("s: %8d, ", s);
				printf("z: %8d\n\n", z);
			}
		}
	}
	else {
		while (1) {

			z = -1;
			while (1) {
				printf("Please input your commnad <0~f> (-1 for exit) : ");
				scanf_s("%x", &c, sizeof(int));

				if ((c < 16) && (c > -2))
					break;
			}
			if (c == -1)
				break;

			printf("Please input your first(x) value : ");
			scanf_s("%8x", &x, sizeof(int));
			printf("Please input your second(y) value : ");
			scanf_s("%8x", &y, sizeof(int));

			s = ALU(x, y, c, &z);
			printf("X: %8x, Y: %8x\n", x, y);

			if (z == -1) {
				printf("s: %8x \n\n", s);
			}
			else {
				printf("s: %8x, ", s);
				printf("z: %8x\n\n", z);
			}
		}
	}

	printf("Program END\n");
	printf("Report-CAU05\n");
	printf("201500122 고 병 현\n");

	return 0;

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
		return ~(X | Y);
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