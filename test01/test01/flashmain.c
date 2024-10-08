/*
 * test01.c
 *
 * Created: 2024-10-02 오후 12:17:22
 * Author : user
 */ 

#define F_CPU 16000000UL // 16MHz
#define __DELAY_BACKWARD_COMPATIBLE__  // delay 함수에 변수를 넣고자 할 떄 사용
#include <avr/io.h>  // AVR 기본 헤더
#include <util/delay.h>  // delay 함수용 헤더
// LED : PG4, output

// SW0 : PG0, Start, input
// SW1 : PG1, more fast, input
// SW2 : PG2, more slow, input

int Check()
{
	char v = PING & 0x01;  // V = G0 bit의 값
	if(v == 0) return 1;  // 눌리면 1
	else return 0;  // 안눌리면 0
}

int main(void)
{
	DDRG |= 0x10;     // G4를 1로 (LED 출력으로)
	DDRG &= ~0x01;  // G0를 0으로 (start 입력으로)
	DDRG &= ~0x02;  // G1을 0으로 (More fast 입력으로)
	DDRG &= ~0x04;  // G2를 0으로 (More slow 입력으로)
	
	char mode = 0;  // mode = 0 이면 disable, mode = 1 이면 active
	while(1) // 무한 대기 상태
	{
		if(Check() == 1) {mode = 1; break;} // SW가 눌리면 대기 상태 탈출
	}
	double t;
	t = 200;  // 200ms
	double t1;
	double t2;
	
	while (1)
	{
		if(Check())  // 0이 아니면 수행할 수 있으므로, == 1은 빼도 됨
		{
			if(mode == 1) mode = 0;
			else mode = 1;
			_delay_ms(200);
		}
		char bf = PING & 0x02; // G1 more fast
		char bs = PING & 0x04; // G2 more slow
		t1 = t - 30;
		t2 = t + 100;
		if(bf == 0)
		{
			t = t1;
		}
		if(bs == 0)
		{
			t = t2;
		}
		if(mode == 1)
		{
			PORTG |= 0x10;   // 해당 bit ON
			_delay_ms(t);  // ms x 200 = 1/5초
			PORTG &= 0xEF;   // 해당 bit OFF
			_delay_ms(t);  // ms x 200 = 1/5초
		}
		
		_delay_ms(100);
	}
}