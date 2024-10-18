/*
 * test03_Timer.c
 *
 * Created: 2024-10-11 오후 2:33:15
 * Author : user
 */ 
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>

#define DDR DDRG
#define PORT PORTG
#define LED0 PORTG0
#define LED1 PORTG1
#define LED2 PORTG2

int cnt = 0, cnt1 = 0, cntEX = 5, st = 0;

ISR(INT0_vect)  // 점점 느리게
{
	cntEX++;  // PD0 : Slower
}

ISR(INT1_vect)  // 점점 빠르게
{
	cntEX--;
	if(cntEX < 1) cntEX = 1;
}

ISR(TIMER0_OVF_vect)
{
	cnt ++;  // 타이머 호출 시마다 cnt 1씩 증가
	if(cnt > 25)  // (1/16M) * 256 * 256 * 25 = 0.102s == 100ms
	{
		cnt = 0;
		cnt1++;
		if(cnt1 >= cntEX)
		{
			cnt1 = 0;
			if(st)
			{
				PORT &= ~(1<<LED0);
				st = 0;
			}
			else
			{
				PORT |= (1<<LED0);
				st = 1;
			}
		}
	}
}

int ocnt = 0, ocnt1 = 0, ocntEX = 5, ost = 0;
ISR(TIMER0_COMP_vect)
{
	ocnt ++;  // 타이머 호출 시마다 cnt 1씩 증가
	if(ocnt > 25)  // (1/16M) * 256 * 256 * 25 = 0.102s == 100ms
	{
		ocnt = 0;
		ocnt1++;
		if(ocnt1 >= ocntEX)
		{
			ocnt1 = 0;
			if(ost)
			{
				PORT &= ~(1<<LED1);
				ost = 0;
			}
			else
			{
				PORT |= (1<<LED1);
				ost = 1;
			}
		}
	}
}

int main(void)
{
	DDR |= ((1<<LED0) | (1<<LED1) | (1<<LED2));  // G0, G1, G2 출력 포트 선언 (LED)
	PORT |= ((1<<LED0) | (1<<LED1) | (1<<LED2));  // G0, G1, G2 LED On
    StandBy();
	PORT &= ~((1<<LED0) | (1<<LED1) | (1<<LED2)); // 버튼이 눌린 후, LED Off
	
	// 16ms마다 Timer 인터럽트 실행
	TIMSK |= 0x01;  // TOIE0 (Timer Overflow Interrupt Enable)
	TCCR0 |= 0x0E;  // 분주비 256 설정
	
	OCR0 = 128;  // CTC 모드의 인터럽트 발생 지점 : 256의 절반
	TIMSK |= (1<<OCIE0);  // 비교일치인터럽트 선언
	
	EIMSK |= 0x03;  // external 인터럽트 호출 PD0, PD1  : 0b 0000 0011
	EICRA = 0x0F;  // 상승 엣지 동작
	sei();  // 인터럽트 종료
	
	while (1) 
    {
		
    }
}

