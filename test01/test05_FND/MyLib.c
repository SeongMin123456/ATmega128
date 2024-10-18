/*
 * MyLib.c
 *
 * Created: 2024-10-11 오후 2:38:21
 *  Author: user
 */ 

#include <avr/io.h>  // AVR 기본 헤더

int TestBit(char pin, char mask)  // PINx 레지스터의 값의 mask bit가 0인지 1인지 판별
{
	if((pin & mask) != 0) return 1;
	return 0;
}


void StandBy()  // PORT G4를 프로그램 시작 안정화 스위치로, 범용 함수 설정
{
	DDRG &= ~0x10;  // G4를 입력 선언
	PORTG |= 0x10;  // PG4 : Pull-up
	
	while(!TestBit(PING,0x10));  // TestBit 가 0이 아니면 while문 탈출
	while(TestBit(PING,0x10));  // TestBit 가 0이면 while문 탈출
}