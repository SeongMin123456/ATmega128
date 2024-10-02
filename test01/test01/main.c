/*
 * test01.c
 *
 * Created: 2024-10-02 오후 12:17:22
 * Author : user
 */ 

#include <avr/io.h>  // AVR 기본 헤더
// LED PORT : PG4


int main(void)
{
	
	/* Replace with your application code */
	
	DDRG |= 0x10; // xxxx xxxx == xxx1 0000 G포트의 4번 비트를 출력으로 -> G 포트는 0 ~ 4까지 5개의 포트만 있음. 1은 출력, 0은 입력
	// 기존값 연동을 위해서는 DDRG |= 0x10; 으로 표시 ex) 기존에 0, 2번 포트 사용 중이면, xxx1 0101로 출력
    while (1) 
    {
		PORTG |= 0x10;
    }
}

