#define F_CPU 16000000UL

#define TIMER8_MODE_NORMAL 0
#define TIMER8_MODE_CORRECT 1
#define TIMER8_MODE_CTC 2
#define TIMER8_MODE_FASTPWM 3

#define TIMER8_NORMAL_PORT 0
#define TIMER8_TOGGLE_OC0 1
#define TIMER8_CLEAR_OC0 2
#define TIMER8_SET_OC0 3
#define TIMER8_NON_INVERT 2
#define TIMER8_INVERT 3

#define TIMER8_PRE_NO 1
#define TIMER8_PRE_8 2
#define TIMER8_PRE_32 3
#define TIMER8_PRE_64 4
#define TIMER8_PRE_128 5
#define TIMER8_PRE_256 6
#define TIMER8_PRE_1024 7

#define TIMER8_INTERRUPT_OVF 1
#define TIMER8_INTERRUPT_COMP 2

/*

#define TIMER16_MODE_NORMAL						0
#define TIMER16_MODE_CORRECT_8					1
#define TIMER16_MODE_CORRECT_9					2
#define TIMER16_MODE_CORRECT_10					3
#define TIMER16_MODE_CTC_TOP_OCR				4
#define TIMER16_MODE_PWM8						5
#define TIMER16_MODE_PWM9						6
#define TIMER16_MODE_PWM10						7
#define TIMER16_MODE_CORRECT_BOTTOM_TOP_ICR		8
#define TIMER16_MODE_CORRECT_BOTTOM_TOP_OCR		9
#define TIMER16_MODE_CORRECT_TOP_TOP_ICR		10
#define TIMER16_MODE_CORRECT_TOP_TOP_OCR		11
#define TIMER16_MODE_CTC						12
#define TIMER16_MODE_PWM_TOP_ICR				14
#define TIMER16_MODE_PWM_TOP_OCR				15

#define TIMER16_INTERRUPT_

void Timer1_Init(uint8_t generation_mode, uint8_t compare_mode, uint8_t pre, uint8_t interrupt = 0, uint8_t tcnt1 = 0, uint8_t ocr1 = 0, uint8_t icr1 = 0)
{
	TCCR1A = (generation_mode & 0x03) | (compare_mode << 3);
	TCCR1B = (generation_mode >> 2 << 3) | (pre);
}

*/

#include <avr/io.h>

void Timer0_Init(uint8_t generation_mode, uint8_t compare_mode, uint8_t prescaler, uint8_t interrupt = 0, uint8_t tcnt0 = 0, uint8_t ocr0 = 0)
{
	TCCR0 = (generation_mode & 0x02 << 5) | (generation_mode & 0x01 << 5) | (compare_mode << 4) | prescaler;
	
	TCNT0 = tcnt0;
	OCR0 = ocr0;
}

void Timer2_Init(uint8_t generation_mode, uint8_t compare_mode, uint8_t pre, uint8_t interrupt = 0, uint8_t tcnt2 = 0, uint8_t ocr2 = 0)
{
	TCCR0 = (generation_mode & 0x02 << 5) | (generation_mode & 0x01 << 5) | (compare_mode << 4) | (pre);
	
	TCNT2 = tcnt2;
	OCR2 = ocr2;
}