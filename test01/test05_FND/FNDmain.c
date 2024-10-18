/*
 * test05_FND.c
 *
 * Created: 2024-10-16 오후 3:12:29
 * Author : user
 */ 
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#define CDDR DDRB
#define CPORT PORTB
#define IDDR DDRA
#define IPORT PORTA
#define SW0 PORTG0  // reset & start & stop
#define SW1 PORTG1

//#define FND_TYPE_CATHODE
//#ifdef FND_TYPE_CATHODE
		//CPORT = ~_BV(i); IPORT = img[data[i]]; _delay_ms(5);
//#else
		//CPORT = _BV(i); IPORT = ~img[data[i]]; _delay_ms(5);

                      // 0,		1,		2,		3,		4,		5,		6,		7,		8,		9
unsigned char img[] = {0x3F,	0x06,	0x5B,	0x4F,	0x66,	0x6D,	0x7D,	0x27,	0x7F,	0x67};
char data[] = {1, 2, 3, 4};

volatile int st = 0, num = 0, cnt = 0;  // 0 : 0000, 1 : start 00.00 , 2: stop , 3: reset

int digit(int num)
{
	if(num>9999) return 0;
	data[0] = num/1000;
	data[1] = (num%1000)/100;
	data[2] = (num%100)/10;
	data[3] = num % 10;
	return 1;
}

ISR(INT0_vect)  // SW0
{
	st++;
	if(st>2)
	{
		st = 0;
	}
}

ISR(TIMER2_COMP_vect) // (1/16M) * 125 * 256 * 5 = 0.01s
{
	TCNT2 = 0;
	switch(st)
	{
		case 0:
		{
			num = 0;
			digit(num);
			break;
		}
		case 1:
		{
			cnt++;
			if(cnt>4)
			{
				num++;
				digit(num);
				cnt = 0;
			}
			break;
		}
		case 2:
		{
			digit(num);
			break;
		}
	}
	//if(st == 0)
	//{
		//num = 0;
		//digit(num);
	//}
	//else if(st == 1)
	//{
		//cnt++;
		//if(cnt>4)
		//{
			//num++;
			//digit(num);
			//cnt = 0;
		//}
	//}
	//else if(st == 2)
	//{
		//digit(num);
	//}
	//else
	//{
		//st = 0;
	//}
}

volatile int i = 0;
ISR(TIMER0_OVF_vect)  // FND Refresh  // 4ms 마다 1회 씩
{	
	CPORT = 1<<i;
	IPORT = ~img[data[i]];
	if(i == 1) IPORT &= ~0x80;
	i++;
	if(i == 4) i = 0;
}

int main(void)
{
	CDDR |= 0x0F;  // 전체 세그먼트 출력 선언
	IDDR |= 0xFF;  // PA 핀 전체(8비트)를 출력 선언
	
	CPORT = 0x0F; // 전체 세그먼트 high 출력
	IPORT = ~0xFF; // 전체 PA 핀 0 출력 -> 전체 세그먼트 점등
	
	DDRG &= ~(1<<SW0);
	PORTG |= (1<<SW0);
	
	EIMSK |= 0x01;  // PD0 호출
	EICRA |= (1<<ISC01)|(1<<ISC00);  // 상승 엣지에서 동작
	
	//StandBy();  // G4 sw는 초기 안정화
	
	// timer2 : 100ms stop-watch 용 타이머 , timer0 : FND refresh
	TIMSK |= (1<<TOIE0);  // TOIE0 호출  // FND Refresh
	TIMSK |= (1<<OCIE2);  // OCIE2 호출  // number count trigger
	TCCR0 |= 0x06;		  // 256 분주비, (1/16M) * 256 * 256 = 0.004096s = 4.096ms
	TCCR2 |= 0x04;		  // 256 분주비, (1/16M) * 256 * 256 = 0.004096s = 4.096ms
	OCR2 = 125;			// (1/16M) * 256(분주비) * 125(OCR) * 5(for문 트리거) = 0.01s
	
	sei();
	
    while (1)
    {

    }
}


	//while (1)
	//{
		//for(int num = 0; num < 10000; num++){
			//data[0] = ((num/60)/10);        // 분 10의 자리 수
			//data[1] = ((num/60)%10);		// 분 1의 자리 수
			//data[2] = (num/10)%6;           // 초 10의 자리 수
			//data[3] = num % 10;             // 초 1의 자리 수
			//int i=1;
			//if(num>600) i = 0;			// 분의 10의 자리 수는 초기에 off 10분이 넘어가면 on
		//
			//for(int q=0; q<1 ;q++)  // 타이머 속도 제어, 1초 기준 : 5ms * 4회 * 50 cycle = 1000ms = 1s
			//{
				//for (i; i < 4; i++)  // 세그먼트 전환 횟수 4번
				//{
					//CPORT = 1<<i;  // i번째 세그먼트 on
					//{
						//IPORT = ~img[data[i]];  // i번째 세그먼트의 숫자 표시
						//if(i == 1) IPORT &= ~0x80;  // 1 2번째 세그먼트 : 분, 3 4번째 세그먼트 : 초
						//_delay_ms(5);   // 세그먼트 전환 속도 (변경 x)
					//}
				//}
			//}
			//int q = 0;
		//}
	//}
//}