/*
 * my_PWM_16bit_counter.c
 *
 * Created: 2024-10-23 오후 2:41:48
 * Author : user
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__  // delay 함수에 변수를 넣고자 할 떄 사용
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#define LED_Y PORTB5
#define LED_B PORTB6
#define LED_W PORTB7

ISR(TIMER1_COMPA_vect)
{
	
}
ISR(TIMER1_COMPB_vect)
{
	
}
ISR(TIMER1_COMPC_vect)
{
	
}

ISR(INT0_vect)  // Y LED 밝기 (Duty)
{
	OCR1A -= 10000;
	if(OCR1A < 0) OCR1A = 60000;
}
ISR(INT1_vect)  // B LED 밝기 (Duty)
{
	OCR1B -= 10000;
	if(OCR1B < 0) OCR1B = 60000;
}
ISR(INT2_vect)  // C LED 밝기 (Duty)
{
	OCR1C -= 10000;
	if(OCR1C < 0) OCR1C = 60000;
}

int main(void)
{
    DDRB |= (1<<LED_Y)|(1<<LED_B)|(1<<LED_W);
	
	TIMSK |= (1<<OCIE1A)|(1<<OCIE1B);		 // 비교일치 인터럽트 선언
	ETIMSK |= (1<<OCIE1C);					 // 비교일치 인터럽트 선언 
	
	TCCR1A |= (0<<WGM13)|(1<<WGM12)|(1<<WGM11)|(1<<WGM10);  // 8bit 고속 PWM : 최대값 256 -> PWM 주파수 (1/16M)/(분주비*TOP) = 61Hz
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|(1<<COM1C1)|(0<<COM1C0); // 비반전 모드 파형 출력
	TCCR1B |= (1<<CS02)|(0<<CS01)|(1<<CS00); // 분주비 1024 : (1/16M) * 1024 : 64us  | 최대값 (1/16M) * 1024 * 65535
	
	OCR1A = 10000;  // 최대 65535
	OCR1B = 10000;  // 최대 65535
	OCR1C = 10000;  // 최대 65535
	
	EIMSK |= (1<<INT0)|(1<<INT1)|(1<<INT2);
	EICRA |= (1<<ISC21)|(1<<ISC20)|(1<<ISC11)|(1<<ISC10)|(1<<ISC01)|(1<<ISC00); // 인터럽트 상승 엣지에서 동작
	
	sei();
	
    while (1) 
    {
    }
}

