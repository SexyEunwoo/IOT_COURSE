/*
 * Slide_LCD.c
 *
 * Created: 2020-11-19 오후 2:30:49
 * Author : dmsdn
 
 
	시작스위치, 기록스위치, 골인식		PE5,6,7
	LED								PF0 ~ 7
	LCD								PA0 ~ 2, PC0 ~ 7
	
	
 */ 
#define READ_ADDR 0x1000

#include <avr/io.h>
#include "LCD.h"
#include "EEPROM.h"

char history[5][7] = {
	{"1st : "},
	{"2st : "},
	{"3st : "},
	{"4st : "},
	{"5st : "},
};

uint8_t history_score[5] = { 0, };

void System_Init()
{
	uint8_t data;
	for(int i = 0; i < 5; i++)
	{
		history[i][6] = '\0';
		data = EEPROM_Read(READ_ADDR+i);
		
		if(data != 255)
		{
			history_score[i] = data;
		}
	}
}

void LCD_Print_History()
{
	while(1)
	{
		for(int i = 0; i < 5; i++)
		{
			LCD_Clear();
			LCD_GOTO_XY(4, 0);
			LCD_Write_String(history[i]);
			LCD_Write_Int(history_score[i]);
			_delay_ms(1000);
		}
	}
}

int main(void)
{
	System_Init();
	DDRA = 0xFF; DDRC = 0xFF;
	LCD_Init();
	LCD_Print_History();
    while (1) 
    {
    }
	
}

