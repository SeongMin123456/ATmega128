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
#define LED PORTG1

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
				PORT &= ~(1<<LED);
				st = 0;
			}
			else
			{
				PORT |= (1<<LED);
				st = 1;
			}
		}
	}
}

int main(void)
{
	DDR |= (1<<LED);  // = _BV(PORTG1), LED 포트 설정
	PORT |= (1<<LED);  // = _BV(PORTG1), LED On
    StandBy();
	PORT &= ~(1<<LED); // 버튼이 눌린 후, LED Off
	
	// 16ms마다 Timer 인터럽트 실행
	TIMSK |= 0x01;  // TOIE0 (Timer Overflow Interrupt Enable)
	TCCR0 |= 0x06;  // 분주비 256 설정
	sei();  // 인터럽트 종료 함수
	
	EIMSK |= 0x03;  // external 인터럽트 호출 PD0, PD1  : 0b 0000 0011
	EICRA = 0x0F;  // 상승 엣지 동작
	sei();  // 인터럽트 종료
	
	while (1) 
    {
		
    }
}

