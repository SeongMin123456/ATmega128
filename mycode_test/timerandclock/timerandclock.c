/*
 * mycode_test.c
 *
 * Created: 2024-10-18 오전 10:13:36
 * Author : user
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL

#define CDDR DDRB
#define CPORT PORTB
#define IDDR DDRA
#define IPORT PORTA
#define LDDR DDRG
#define LPORT PORTG
#define LED0 PORTG0
#define LED1 PORTG1
#define BUZ PORTG2
#define SW0 PORTD0
#define SW1 PORTD1
#define SW2 PORTD2

                    //  0,	  1,	2,	  3,	4,	  5,	6,     7,	8,	  9    
unsigned char img[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
   // segment위치 1000의 자리, 100의 자리, 10의 자리, 1의 자리
char data[] = {1, 2, 3, 4};

volatile int st=0, num=0, cst =0, comn = 0, cnt=0, i=0, mode = 1, clock1 = 0;
// st가 0 : 00.00, 1 : start 00.00, 2 : stop -> reset

int digit(int num)
{
	if(num>60*59+59)
	{
		num = 60*59+59;
		return 0;
	}
	data[0] = ((num/60)/10); // 분 10의 자리 수
	data[1] = ((num/60)%10); // 분 1의 자리 수
	data[2] = ((num/10)%6);  // 초 10의 자리 수
	data[3] = num%10;  // 초 1의 자리 수
	return 1;
}

ISR(INT0_vect) // SW0 : start / stop / finish
{
	st++;
	if(st>2)
	{
		st=0;
		num = 0;
		comn = 0;
	}
}

ISR(INT1_vect) // SW1 : 분 단위 설정
{
	cst = 1;
	if (mode == 0)	comn = comn + 60;
	else if (st == 1)	clock1 = clock1 + 60;
	
	if(st == 2) cst = 0;
	else if(comn > 60*59+59) comn = 0;
	else if(clock1 > 60*59+59) clock1 = 0;
}

ISR(INT2_vect) // SW2 mode : 동작 모드 설정 = 0 : timer , 1 : clock
{
	mode++;
	num = 0;
	comn = 0;
	st = 0;
	PORTG &= ~(1<<LED0);
	PORTG |= (1<<LED1);
	if(mode > 1)
	{
		PORTG &= ~(1<<LED1);
		PORTG |= (1<<LED0);
		mode = 0;
		st = 0;
	}
}

ISR(TIMER0_OVF_vect)
{
	CPORT = 1<<i;
	IPORT = ~ img[data[i]];
	if(i == 1) IPORT &= ~0x80;
	i++;
	if(i ==4) i = 0;
}

ISR(TIMER1_OVF_vect)
{
	if(mode == 0)
	{
		if((st == 0) & (cst == 1))
		{
			digit(comn);
		}
		else if((st == 0) & (cst == 0))
		{
			num = 0;
			digit(num);
			PORTG |= (1<<LED0);
		}
		else if((comn != 0) & (num == comn) & (st == 1))
		{
			PORTG |= (1<<BUZ);		// stop sw 누를 때까지 계속 부저 울리게
			//_delay_ms(3000);
			//st = 2;
		}
		else if(st == 1)
		{
			PORTG &= ~(1<<LED0);
			cnt++;
			if(cnt>244)
			{
				num++;
				digit(num);
				cnt = 0;
			}
		}
		else if(st == 2)
		{
			PORTG &= ~(1<<BUZ);
			digit(num);
			PORTG |= (1<<LED0);
		}
		else
		{
			st=0;
			num = 0;
			comn = 0;
		}
	}
	else if(mode == 1)
	{
		if(st == 0)
		{
			PORTG &= ~(1<<LED1);
			cnt++;
			if(cnt>244)
			{
				clock1++;
				digit(clock1);
				cnt = 0;
			}
			if(clock1 > 60*59+59)
			{
				clock1 = 0;
				digit(clock1);
			}
		}
		else if(st == 1)
		{
			digit(clock1);
			PORTG |= (1<<LED1);
		}
		else if(st == 2)
		{
			digit(clock1);
			PORTG |= (1<<LED1);
			st = 0;
		}
	}
}
	//{
		//if((st == 0) & (cst == 1))
		//{
			//digit(clock1);
		//}
		//else if(st == 0)
		//{
			//clock1 = 0;
			//digit(clock1);
			//PORTG |= (1<<LED1);
		//}
		//else if(st == 1)
		//{
			//PORTG &= ~(1<<LED1);
			//cnt++;
			//if(cnt>244)
			//{
				//clock1++;
				//digit(clock1);
				//cnt = 0;
			//}
		//}
		//else if(st == 2)
		//{
			//digit(clock1);
			//PORTG |= (1<<LED1);
			//st = 0;
		//}
	//}
//}

int main(void)
{
	CDDR |= 0x0F;  // 전체 세그먼트 출력 선언
	IDDR |= 0xFF;  // PA 핀 전체(8비트)를 출력 선언
	LDDR |= 0x07;  // PG0, PG1, PG2 LED & Buzzer 출력 선언
	
	CPORT = 0x0F;	// 전체 세그먼트 high 출력
	IPORT = ~0xFF;  // 전체 PA핀 0 출력 -> 전체 세그먼트 점등
	
	DDRD &= ~(1<<SW0); // sw0 입력 선언 : sw0 버튼으로 최종값 설정
	PORTD |= (1<<SW0); // pull-up
	DDRD &= ~(1<<SW1); // sw1 입력 선언 : sw1 버튼으로 st 제어 = 시간 up
	PORTD |= (1<<SW1); // pull-up
	DDRD &= ~(1<<SW2); // sw2 입력 선언 : sw1 버튼으로 st 제어 = 시간 down
	PORTD |= (1<<SW2); // pull-up
	
	EIMSK |= 0x07;		// PD0, PD1, PD2 호출
	EICRA |= (1<<ISC00)|(1<<ISC01); // PD0 상승 엣지에서 동작
	EICRA |= (1<<ISC10)|(1<<ISC11); // PD1 상승 엣지에서 동작
	EICRA |= (1<<ISC20)|(1<<ISC21); // PD1 상승 엣지에서 동작
	
	TIMSK |= (1<<TOIE0);  // timer0 : 세그먼트 refresh
	TCCR0 |= 0x06;		  // 256 분주비 : (1/16M) * 256 * 256
	TIMSK |= (1<<TOIE1);  // timer1 : 세그먼트 넘버링
	TCCR1B |= 0x01;		  // 기본 분주비 : (1/16M) * 65535 = 0.004096ms = 4.096ms  // 인터럽트 244번 반복 : 4.096ms * 244 = 0.9994s
	
	sei();
	
    while (1)
    {
		
    }
}