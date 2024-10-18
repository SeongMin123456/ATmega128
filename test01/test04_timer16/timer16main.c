/*
 * test04_timer16.c
 *
 * Created: 2024-10-16 오전 11:38:23
 * Author : user
 */ 
#define F_CPU 16000000UL  // delay에서 정확한 시간을 알기 위해 사용되므로, timer에는 안써줘도 무방
#include <avr/interrupt.h>
#include <avr/io.h>

#define DDR DDRG
#define PORT PORTG
#define LED0 PORTG0
#define LED1 PORTG1
#define LED2 PORTG2

int st3 = 0;
ISR(TIMER3_OVF_vect)
{
	if(st3)
	{
		PORT &= ~(1<<LED1);
		st3 = 0;
	}
	else
	{
		PORT |= (1<<LED1);
		st3 = 1;
	}
}
int st1 = 0;
ISR(TIMER1_OVF_vect)
{
	if(st1 < 2)
	{
		PORT &= ~(1<<LED0);
		st1 = st1 + 1;
	}
	else if((st1 >=2) & (st1<6))
	{
		PORT |= (1<<LED0);
		st1 = st1 + 1;
	}
	else
	{
		PORT &= ~(1<<LED0);
		st1 = 0;
	}
}

int main(void)
{
	DDR |= ((1<<LED0) | (1<<LED1) | (1<<LED2));  // G0, G1, G2 출력 선언
	
	ETIMSK |= (1<<TOIE3);  // timer 3번 OVF 인터럽트 사용 선언
	TIMSK |= (1<<TOIE1);  // timer 1번 OVF 인터럽트 사용 선언
	
	TCCR1B |= ((0<<CS02) | (1<<CS01) | (1<<CS00));  // 분주비 64 설정 : 0.004096 * 64 = 0.2621s 간격으로 인터럽트 발생
	TCCR3B |= ((0<<CS02) | (1<<CS01) | (1<<CS00));  // 분주비 64 설정 : 0.004096 * 64 = 0.2621s 간격으로 인터럽트 발생
    
	sei();
	
	while (1) 
    {
    }
}

