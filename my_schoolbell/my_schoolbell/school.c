/*
 * my_schoolbell.c
 *
 * Created: 2024-10-24 오전 11:47:40
 * Author : user
 */ 

#define __DELAY_BACKWARD_COMPATIBLE__  // delay 함수에 변수를 넣고자 할 떄 사용
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#define buzz PORTB5


volatile int i = 0;
int CS = 8; // 분주비
enum { DO = 130, Do = 138, RE = 146, Re = 155, MI = 164, FA = 174, Fa = 185, SL = 196, Sl = 207, LA =  220, La = 233, SI = 246};
int song[] = { SL, SL, LA, LA, SL, SL, MI, SL, SL, MI, MI, RE, SL, SL, LA, LA, SL, SL, MI, SL, MI, RE, MI, DO};
int song1[100];

void conv()
{
	for(i; i<25; i++)  // 학교 종이 1 cycle
	{
		song1[i] = F_CPU / (CS * song[i] * 5);  // buzz 주파수 선정을 위한 top
	}
}

ISR(TIMER1_COMPA_vect)
{
	
}
int num = 0;
int main(void)
{
	DDRB |= (1<<buzz);
	
	TIMSK |= (1<<OCIE1A);		 // 비교일치 인터럽트 선언
	
	TCCR1A |= (1<<WGM11)|(0<<WGM10);					// 고속 PWM : 최대값 ICR1 (주파수), OCR1A (듀티)
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0);					// 비반전 모드 파형 출력
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);	// 분주비
	
	sei();
	
	conv();
	
	while (1)
	{
		ICR1 = song1[num];
		OCR1A = song1[num] / 2;
		_delay_ms(500);
		OCR1A = 1;
		_delay_ms(7);
		num++;
		if(num>24) num = 0;
	}
}


