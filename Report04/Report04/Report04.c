#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int PrintBinary(char a);
int MipsNumParser(int start, int end);
int opcparser(int irnum, int pos);
int fctparser(int irnum, int pos);
int Instrparser(int pos);

unsigned char *MEM;
unsigned char ReadAllMasking = 0xFF;
unsigned char OPmasking = 0xFC;
unsigned char FUmasking = 0x3F;
unsigned char Masking[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

char *optable[] = { "R-format", "bltz", "j", "jal", "beq", "bne", "none", "none",
				   "addi", "none", "slti", "none", "andi", "ori", "xori", "lui",
				   "none", "none", "none", "none", "none", "none", "none", "none",
				   "none", "none", "none", "none", "none", "none", "none", "none",
				   "lb", "none", "none", "lw", "blu", "none", "none", "none",
				   "sb", "none", "none", "sw", "none", "none", "none", "none", };

char *futable[] = { "sll", "none", "srl", "sra", "none", "none", "none", "none",
				   "jr", "none", "none", "none", "syscall", "none", "none", "none",
				   "mfhi", "none", "mflo", "none", "none", "none", "none", "none",
				   "mul", "none", "none", "none", "none", "none", "none", "none",
				   "add", "none", "sub", "none", "and", "or", "xor", "nor",
				   "none", "none", "slt", "none", "none", "none", "none", "none", };


struct IRManager {
	unsigned int IR;
	int size;
};

struct IRManager MyIR;

struct rtype {
	unsigned int I;
	struct rtype {
		unsigned int opcode : 6;
		unsigned int rs : 5;
		unsigned int rt : 5;
		unsigned int rd : 5;
		unsigned int sh : 5;
		unsigned int funct : 6;
	} RI;
} IR;

struct itype {
	unsigned int I;
	struct itype {
		unsigned int opcode : 6;
		unsigned int rs : 5;
		unsigned int rt : 5;
		unsigned int offset : 16;
	} III;
} II;

struct jtype {
	unsigned int I;
	struct jtype {
		unsigned int opcode : 6;
		unsigned int ja : 26;
	} JI;
} IJ;

int main()
{
	FILE *pFile = NULL;
	errno_t err;
	int count;
	int size;
	unsigned int EachNum[2] = { 0, };
	int i, j;
	int opc, fac;


	err = fopen_s(&pFile, "ex01.bin", "rb");
	if (err) {
		printf("Cannot open file\n");
		return 1;
	}

	fseek(pFile, 0, SEEK_END);
	size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	MEM = (char*)malloc(size);

	count = fread(MEM, size, 1, pFile);
	if (count != 1) {
		printf("Cannot read file\n");
		return 0;
	}
	fclose(pFile);

	EachNum[0] = MipsNumParser(0, 4);
	EachNum[1] = MipsNumParser(4, 8);


	printf("Number Of Instructions: %d, Number of Data: %d\n", EachNum[0], EachNum[1]);

	for (i = 8; i < (4 * EachNum[0]) + 8; i += 4)
	{
		int opc = opcparser(6, i);
		int fct = fctparser(6, i);

		Instrparser(i, opc, fct);
		printf("Opc: %2x, Fct: %2x, ", opc, fct);
		if (opc == 0) {
			printf("Inst: %s, ", futable[fct]);
			printf("Rs: %2x, Rt: %2x, Rd: %2x, Sh: %2x", IR.RI.rs, IR.RI.rt, IR.RI.rd, IR.RI.sh);
		}
		else if ((opc == 2) || (opc == 3)) {
			printf("Inst: %s, ", optable[opc]);
			printf("Jump Address: %2x ", IJ.JI.ja);
		}
		else {
			printf("Inst: %s, ", optable[opc]);
			printf("Rs: %2x, Rt: %2x, Offset: %2x ", II.III.rs, II.III.rt, II.III.offset);
		}
		printf("\n");
	}

	return 0;
}


int Instrparser(int pos, int opc, int fct)
{
	int i, j, result = 0;
	unsigned char iR[32] = { 0, };

	for (j = 0; j < 4; j++) {
		for (i = 0; i < 8; i++)
		{
			if ((MEM[pos + j] & Masking[i]))
				iR[i + (j * 8)] = 1;
			else
				iR[i + (j * 8)] = 0;
		}
	}
	if (opc == 0) {
		for (i = 6; i < 26; i+=5) {
			result = 0;
			for (j = 0; j < 5; j++) {
				result += (iR[i+j] * pow(2.0, 4 - j));
				//printf("i: %d, iR[i+j] : %d  result: %d\n",i, iR[i + j], result);
			}
			if (i == 6)
				IR.RI.rs = result;
			else if (i == 11)
				IR.RI.rt = result;
			else if (i == 16)
				IR.RI.rd = result;
			else
				IR.RI.sh = result;
		}
		IR.RI.opcode = opc;
		IR.RI.funct = fct;
	}
	else if ((opc == 2) || (opc == 3)) {
		result = 0;
		for (i = 6; i < 32; i++)
			result += (iR[i] * pow(2.0, 31 - i));
		IJ.JI.ja = result;
		IJ.JI.opcode = opc;
	}
	else {
;		for (i = 6; i < 16; i += 5) {
			result = 0;
			for (j = 0; j < 5; j++) {
				result += (iR[i + j] * pow(2.0, 4 - j));
				//printf("i: %d, iR[i+j] : %d  result: %d\n", i, iR[i + j], result);
			}
			if (i == 6)
				II.III.rs = result;
			else
				II.III.rt = result;
		}
		result = 0;
		for (;i < 32; i++)
			result += (iR[i] * pow(2.0, 31 - i));
		II.III.opcode = opc;
		II.III.offset = result;
	}


	printf("parsed %2d ir : ", pos);
	for (i = 1; i <=32; i++) {
		printf("%d ", iR[i-1]);
		if (i % 4 == 0)
			printf("   ");
	}
	printf("\n");
}


int opcparser(int irnum, int pos)
{
	int i, result = 0;
	int instruction[8] = { 0, };

	for (i = 0; i < irnum; i++)
	{
		if ((MEM[pos] & Masking[i]))
			instruction[i] = 1;
		else
			instruction[i] = 0;
	}

	for (i = 0; i < irnum; i++)
	{
		result += (instruction[i] * pow(2.0, 5 - i));
	}
	return result;
}

int fctparser(int irnum, int pos)
{
	pos += 3;
	int i, result = 0;
	int instruction[8] = { 0, };

	for (i = 8 - irnum; i < 8; i++)
	{
		if ((MEM[pos] & Masking[i]))
			instruction[i] = 1;
		else
			instruction[i] = 0;
	}

	for (i = 2; i < 8; i++)
	{
		result += (instruction[i] * pow(2.0, 7 - i));
	}
	return result;
}


int MipsNumParser(int start, int end)
{
	int i, j;

	for (i = start; i < end; i += 4)
	{
		MyIR.IR = 0;
		for (j = 0; j < 4; j++)
		{
			MyIR.IR += MEM[i + j] * pow(16.0, 3 - j);
		}
	}
	return MyIR.IR;
}

int PrintBinary(char a)
{
	int i;
	unsigned char p;

	p = Masking[0];
	for (i = 0; i < 8; i++) {
		if (a & p) {
			printf("[1]");
		}
		else {
			printf("[0]");
		}
		p = p >> 1;
	}
}