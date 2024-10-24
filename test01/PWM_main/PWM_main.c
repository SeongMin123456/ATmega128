/*
 * PWM_main.c
 *
 * Created: 2024-10-23 오전 9:51:11
 * Author : user
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__  // delay 함수에 변수를 넣고자 할 떄 사용
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#define LED_R PORTG0
#define LED_G PORTG1
#define LED_Y PORTG2
#define DDR	  DDRG
#define PORT  PORTG

volatile int period = 50;
volatile int DutyRate = 50;

volatile int led_y_st = 0;			// LED_Y 상태 확인 변수
volatile int cnt = 0, cnt_ref = 10;

ISR(INT0_vect)
{
	period -= 10;					// 주기 감소
	if(period < 0) period = 500;
}
ISR(INT1_vect)
{
	DutyRate += 10;
	if(DutyRate > 90) DutyRate = 10;
}
ISR(INT2_vect)
{
	//cnt_ref--;
	//if(cnt_ref < 0) cnt_ref = 50;
	OCR0+= 10;
}

ISR(TIMER0_COMP_vect)
{
	//if(++cnt > cnt_ref)
	//{
		//if(led_y_st)
		//{
			//PORT &= ~(1<<LED_Y);	// LED_Y off
			//led_y_st = 0;
		//}
		//else
		//{
			//PORT |= (1<<LED_Y);		// LED_Y on
			//led_y_st = 1;
		//}
		//cnt = 0;
	//}
	// TCNT0 = 0;
}
int main(void)
{
	
	EIMSK |= (1<<INT0)|(1<<INT1)|(1<<INT2);	  // 외부 인터럽트 0,1,2 호출
	EICRA |= (1<<ISC00)|(0<<ISC01);			  // 인터럽트 PD0 하강 엣지 동작
	EICRA |= (1<<ISC10)|(0<<ISC11);			  // 인터럽트 PD1 하강 엣지 동작
	EICRA |= (1<<ISC20)|(0<<ISC21);			  // 인터럽트 PD2 하강 엣지 동작
	
	//TIMSK |= (1<<OCIE1A)|(1<<OCIE1B);
	//ETIMSK |= (1<<OCIE1C);
	
	OCR3A = 
	
	TCCR0 |= (1<<CS00)|(1<<CS01)|(1<<CS02);	  // 분주비 1024 : (1/16M) * 1024 = 64us
	TCCR0 |= (1<<WGM01)|(1<<WGM00);           // Fast PWM mode
	TCCR0 |= (1<<COM01)|(0<<COM00);			  // OC0 정상 출력 설정
	OCR0 = 157;								  // ~ 10ms  // 156.25
	
	sei();
	DDRB |= 0x10;							  // PB4 : OC0 출력 포트로 선언
	DDR |= (1<<LED_R)|(1<<LED_G);//|(1<<LED_Y);  // LED 포트 출력 포트로 선언
	
    while (1)
    {
		PORT |= (1<<LED_R)|(1<<LED_G);				 // LED On
		_delay_ms(period*DutyRate/100);			     // Duty 50% : on time
		PORT &= ~((1<<LED_R)|(1<<LED_G));			 // LED off
		_delay_ms(period*(100-DutyRate)/100);		 // Duty 50% : off time
    }
}