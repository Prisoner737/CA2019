//CAU06
#include <stdio.h>
#include <stdlib.h>
#define WhenReadVal 0xFFFFFFFF //read 작업시 전달할 쓰레기값.

unsigned char progMEM[0x100000], dataMEM[0x100000], stakMEM[0x100000];

int MEM(unsigned int A, int V, int nRW, int S);
void memController();

int main()
{
	memController();

	return 0;
}

void memController() {//MEM을 검증하기 위한 테스트함수

	unsigned int address;
	int value, rw, s;
	
	while (1) {// -1입력시까지 무한반복으로 test
		printf("Work <0-Read / 1-Write / -1 for exit> : ");
		scanf_s("%d", &rw);

		if (rw == -1)
			break;
		
		printf("Address <Hex> : ");
		scanf_s("%x", &address);

		printf("Select Word <0-byte / 1-Half_Word / 2-Word> : ");
		scanf_s("%d", &s);

		if (rw == 1)
		{
			if (s == 0) {
				printf("Write your data, That is limited by word size : ");
				scanf_s("%d", &value);
 

				if ((s == 0) && ((value < -128) || (value > 127))) {
					printf("Byte Range Error; %d is too big \n\n", value);
					continue;
				}
			}
			else if (s == 1) {
				printf("Write your data, That is limited by word size : ");
				scanf_s("%d", &value);

				if ((s == 1) && ((value < -32768) || (value > 32767))) {
					printf("Half-Word Range Error; %d is too big \n\n", value);
					continue;
				}
			}
			else if(s == 2) {
				printf("Write your data, That is limited by word size : ");
				scanf_s("%d", &value);
			}
			else {
				printf("Word size error re-try\n\n");
				continue;
			}
		}
		

		if (rw == 0) {//Read 작업시
			printf("The Value : %d \n\n", MEM(address, WhenReadVal, rw, s));
			}
		else {//Write작업시
			MEM(address, value, rw, s);
			printf("Write Complete at %d\n\n", address);
			}

	}

	printf("Program END\n");
	printf("Report-CAU06\n");
	printf("201500122 고 병 현\n");
}

int MEM(unsigned int A, int V, int nRW, int S) {

	int i;
	unsigned int extender;
	unsigned int temp = 0;
	unsigned int sel, offset;
	unsigned int parsed[4];
	unsigned char* pM;
	unsigned int val = NULL;


	sel = A >> 20;
	offset = A & 0xFFFFF;
	
	for (i = 3; i > -1; i--) {//들어온 data를 byte단위로 parse
		parsed[i] = V & 0xFF;
		V = V >> 8;
	}

	if (sel == 0x004)
		pM = progMEM;
	else if (sel == 0x100)
		pM = dataMEM;
	else if (sel == 0x7FF)
		pM = stakMEM;
	else {
		printf("No Memory Address!\n");
		exit(1);
	}
	pM += offset; //해당하는 주소로 pM를 이동

	if (nRW == 0) {// Read작업의 경우
		val = 0; // 0x00 00 00 00 로 초기화

		if (S == 0) {
			val = *pM;
			if (*pM >> 7) {//음수인경우 처리
				val = *pM;
				val = val | 0xFFFFFF00;
			}
		}
		else if (S == 1) {// LSB부터 2쌍의 00 00이 *pM과 OR연산 
			char flag = *pM >> 7;//MSB를 읽어서 음수인지 파악
			for (i = 0; i < 2; i++) {
				extender = *pM; //  char와 int간의 자리수를 맞추기 위해 temp로 char형의 메모리 값을 확장 
				val = val | (extender << ((1 - i) * 8)); // 원래의 big-endian의 위치를 맞추기 위해 8bit단위로 왼쪽으로 shift하여 크기 조정 >> half word 시 pM은 [15:8], pM+1은 [7:0]위치
				*pM++;// 메모리의 다음값으로 
			}
			if (flag) {//음수인경우의 처리
				val = val | 0xFFFF0000;
			}
		}
		else if (S == 2) {
			for (i = 0; i < 4; i++){
				extender = *pM;
				val = val | (extender << ((3 - i) * 8));//FullWord시 pM은 [31:24], pM+1은 [23:16], pM+2는 [15:8], pM+3은 [7:0]
				*pM++;
			}
		}
		else {
			printf("Read/Write Error! Check Your Control Signal\n"); //그 외 워드의 크기로는 접근 불가
			exit(1);
		}
	}
	else if (nRW == 1) {// Write작업의 경우
		if (S == 0) {
			*pM = parsed[3];// byte기록임으로 들어온 값의 마지막 8자리(pared[3])만 기록
		}
		else if (S == 1) {// half word기록임으로 들어온 값의 끝에서 8bit씩 끊어서 16bit만 기록(parsed[2],parsed[3])
			for (i = 0; i < 2; i++) {
				*pM = parsed[2+i];
				*pM++;
			}
		}
		else if (S == 2) {// word기록임으로 들어온값을 8bit씩 끊어서 32bit 모두 기록
			for (i = 0; i < 4; i++) {
				*pM = parsed[i];
				*pM++;
			}
		}
		else {
			printf("Read/Write Error! Check Your Control Signal\n"); //그 외 워드의 크기로는 접근 불가
			exit(1);
		}
	}
	else {
		printf("RW Select Error");
		exit(1);
	}


	return val;
}
