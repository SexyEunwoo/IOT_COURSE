#pragma once

#ifndef DB	// 사용자가 직접 정의하지 않을 경우 default값 PORTC
#define DB PORTC
#endif

#ifndef MODE_SELECT	// 사용자가 직접 정의하지 않을 경우 default값 PORTA
#define MODE_SELECT PORTA	// 0 ~ 2까지 순서대로 RS, RW, E
#endif

#ifndef F_CPU	// 사용자가 직접 정의하지 않을 경우 default값 16MHz
#define F_CPU 16000000UL
#endif

#define FUNCTION_SET 0x38	// 8bit, 2line, 5x11
#define DISPLAY_ONOFF 0x0C	// Display on, Cursor off, Blink off
#define DISPLAY_CLEAR 0x01	// Display Clear
#define ENTRY_MODE_SET 0x06	// Increase mode, shift none

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void LCD_Write_Instruction(uint8_t instruction)
{
	MODE_SELECT = 0x00;  
	_delay_us(1);
	MODE_SELECT ^= 0x04;
	_delay_us(1);
	DB = instruction;
	_delay_us(1);
	MODE_SELECT ^= 0x04;
	_delay_us(50); 
}

void LCD_Write_Data(char data)
{
	MODE_SELECT = 0x01;
	_delay_us(1);
	MODE_SELECT ^= 0x04;
	_delay_us(1);
	DB = data;
	_delay_us(1);
	MODE_SELECT ^= 0x04;
	_delay_us(50);
}

void LCD_Write_String(char* datas)
{
	while (*datas != '\0') { LCD_Write_Data(*(datas++)); }
}

void LCD_Write_Int(int val)
{
	char datas[4];
	sprintf(datas, "%d\0", val);
	LCD_Write_String(datas);
}

void LCD_GOTO_XY(uint8_t x, uint8_t y)
{
	if (y == 0) LCD_Write_Instruction(0x80 + x);
	else if (y == 1) LCD_Write_Instruction(0xC0 + x);
}

void LCD_Clear()
{
	LCD_Write_Instruction(DISPLAY_CLEAR);
	_delay_ms(2);
}

void LCD_Init()
{
	_delay_ms(50);
	LCD_Write_Instruction(FUNCTION_SET);
	_delay_us(40);
	LCD_Write_Instruction(DISPLAY_ONOFF);
	_delay_us(40);
	LCD_Write_Instruction(DISPLAY_CLEAR);
	_delay_ms(2);
	LCD_Write_Instruction(ENTRY_MODE_SET);
}
