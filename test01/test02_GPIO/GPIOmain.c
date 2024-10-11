/*
 * test02_GPIO.c
 *
 * Created: 2024-10-10 오전 9:36:17
 * Author : user
 */ 

#define __DELAY_BACKWARD_COMPATIBLE__  // delay 함수에 변수를 넣고자 할 떄 사용
#define F_CPU 16000000UL // 16MHz ,기본 1MHz로 설정 됨
#include <avr/io.h>  // AVR 기본 헤더
#include <util/delay.h>  // delay 함수용 헤더
#include <avr/interrupt.h>  // 인터럽트용 헤더

// 각 포트 선언
#define SW1 PORTG4 // start
#define SW2 PORTD0 // 인터럽트 0번 핀
#define SW3 PORTD1 // 인터럽트 1번 핀

#define LED1 PORTG1

volatile int ival = 1000;  // 전역변수 = 최적화 금지 변수

ISR(INT0_vect)  // 점점 느리게
{
	ival +=100;  // PD0 : Slower
}

ISR(INT1_vect)  // 점점 빠르게
{
	ival -=100;
	if(ival < 10) ival = 100;
}

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

int main(void)
{
	DDRG &= ~0x0c;  // 1111 0011 입력 마스크 G3, G2 입력 선언
	DDRG |= 0x03;  // 0000 0011 출력 마스크 G1, G0 출력 선언
	
		// (1<<LED1)로 표현 가능 , 매크로 _BV(LED1); 으로도 표현 가능
	PORTG |= _BV(LED1);  // 시작되면 LED On
	
	StandBy();
	
	PORTG &= ~_BV(LED1); // Standby 탈출하면 LED Off
	
	EIMSK |= 0x03;  // 인터럽트 호출 PD0, PD1  : 0b 0000 0011
	EICRA = (EICRA & 0xF0) | 0x0A;  // 기존 동작을 유지하며 하강 엣지 동작으로 변경
	
	sei();  // 인터럽트 종료
	
    while (1)
    {
		PORTG |= _BV(LED1);   // LED ON
		_delay_ms(ival);
		PORTG &= ~_BV(LED1);  // LED OFF
		_delay_ms(ival);
    }
}