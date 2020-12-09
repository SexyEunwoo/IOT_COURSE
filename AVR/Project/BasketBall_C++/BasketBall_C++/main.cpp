/*
 * BasketBall_C++.cpp
 *
 * Created: 2020-11-19 오후 4:51:54
 * Author : dmsdn
 
	PF0 ~ 7 = Segment
	PC0 ~ 7 = LCD
	PA0 ~ 2 = LCD
	PA3 ~ 4 = Segment
	PD0 ~ 2 = button(start, records, end)
	
	Timer0 Normal : Segment
	Timer1 Normal : reduce remaining time(1s)
	External Interrupt : Button
 */ 

#include "LCD.h"
#include "EEPROM.h"
#include "List.h"
#include <avr/interrupt.h>
#define ADDR_RECORDS 0x1000

volatile uint8_t op_mode = 0;
volatile uint8_t remaining_time = 50;
volatile uint8_t waiting_time = 0;
volatile uint8_t point_user = 0;

uint8_t segment_anode[11] = {
	0b1000000,	// 0
	0b1111001,	// 1
	0b0100100,	// 2
	0b0110000,	// 3
	0b0011001,	// 4
	0b0010010,	// 5
	0b0000010,	// 6
	0b1111000,	// 7
	0b0000000,	// 8
	0b0010000,	// 9
	0b0111111	// -
};
uint8_t segment_cathod[11] = {
	0b0111111,	// 0
	0b0000110,	// 1
	0b1011011,	// 2
	0b1001111,	// 3
	0b1100110,	// 4
	0b1101101,	// 5
	0b1111101,	// 6
	0b0000111,	// 7
	0b1111111,	// 8
	0b1101111,	// 9
	0b1000000,	// -
};
List records;
enum GAME_MODE {SELECT, PREPARING, PLAYING, RECORDS, END};
volatile enum GAME_MODE game_mode = SELECT;


void Program_Init()
{
	// EEPROM_Write_Datas(ADDR_RECORDS, "CEW 76RKR 86AKW 88ASD 12VDS 65", 36);
	char temp[5][7];
	
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			temp[i][j] = EEPROM_Read(ADDR_RECORDS + (i * 6) + j);
		}
		temp[i][6] = '\0';
		records.Append(temp[i]);
	}
	records.Sort();
}

char* RankStr(uint8_t idx)
{
	switch (idx)
	{
		case 0:
			return "1st.";
		case 1:
			return "2nd.";
		case 2:
			return "3rd.";
		case 3:
			return "4th.";
		case 4:
			return "5th.";
	}
}

void Timer0_Init() // 1kHz속도 ( 세그먼트 제어 )
{
	DDRF = 0xFF; DDRG = 0xFF;
	TCCR0 = (1 << CS02) | (1 << CS00);
	TCNT0 = 131;
	TIMSK ^= (1 << TOIE0);
}

void Timer1_Init() // playing mode일 때 1초마다 remaning_time을 1씩 감소
{
	TCCR1B = (1 << CS02);
	TCNT1 = 3036;
	TIMSK ^= (1 << TOIE1);
}

void Timer3_Init()
{
	TCCR3B = (1 << CS32);
	TCNT3 = 59286;
	ETIMSK ^= (1 << TOIE3);
}	  // 10Hz 속도

void External_Init() // Start Button, Record Button, Goal recognition
{
	EICRA = (1 << ISC31) | (1 << ISC21) | (1 << ISC11) | (1 << ISC01);
	EIMSK = (1 << INT3) | (1 << INT2) | (1 << INT1) | (1 << INT0);
}

ISR(TIMER0_OVF_vect)
{
	uint8_t state = PORTG & 0x03;	// 현재 PORTG3,4의 값을 구한다
	uint8_t a;	// 자리수
	
	if ((state == 0x03) || (state == 0x00)) // PORTG3,4의 값이 11이거나 00이라면
	{
		PORTG ^= 0x02;						// 앞쪽 LED power만 On
	}
	else
	{
		PORTG ^= 0x03;						// LED Power 반전
	}
	switch (game_mode)
	{
		case SELECT:
		case RECORDS:
			PORTF = segment_anode[10];
			break;
		case PREPARING:
			PORTF = segment_anode[waiting_time];
			break;
		case PLAYING:
			PORTF = (state == 0x02) ? (segment_anode[remaining_time % 10]) : (segment_anode[remaining_time / 10]);
			break;
		case END:
			PORTF = segment_anode[10];
			break;
	}
}

ISR(TIMER1_OVF_vect)
{
	if (game_mode == PLAYING && remaining_time > 0)	// playing mode 작동
	{
		remaining_time--;
		if(remaining_time == 0)	
		{
			waiting_time = 3;
			game_mode = END;
		}
	}
	else if(game_mode == PREPARING)							// preparing mode 작동
	{
		waiting_time--;
		if(waiting_time == 0)								
		{
			game_mode = PLAYING;
			remaining_time = 60;
		}
	}
	else if(game_mode == RECORDS)							// records mode 작동
	{
		records.UpCount();
		if(records.GetCount() >= records.GetValidNum())
		{
			records.SetCount(0);
		}
	}
	else if(game_mode == END)								// 게임이 끝났을 경우
	{
		waiting_time--;
		if(waiting_time == 0)
		{
			LCD_Clear();
			LCD_Write_String("Your Point is ");
			LCD_Write_Int(point_user);
			point_user = 0;
			
			// records.SavePoint(point_user);
			_delay_ms(1500);
			game_mode = SELECT;
		}
	}
}

ISR(TIMER3_OVF_vect)
{
	switch (game_mode)
	{
		case SELECT:
		LCD_Clear();
		LCD_GOTO_XY(1, 0);
		LCD_Write_String("You can select");
		LCD_GOTO_XY(0, 1);
		LCD_Write_String("1.Play 2.Records");
		break;
		
		case PREPARING:
		LCD_Clear();
		LCD_Write_String("Waiting ");
		LCD_Write_Int(waiting_time);
		LCD_Write_String("sec");
		break;
		
		case PLAYING:
		LCD_Clear();
		LCD_Write_String("YOUR POINT: ");
		LCD_Write_Int(point_user);
		LCD_GOTO_XY(0, 1);
		LCD_Write_String("BEST POINT: ");
		LCD_Write_Int(records.GetBestPoint());
		break;
		
		case RECORDS:
		char str[11];
		sprintf(str, "%s%s%n", RankStr(records.GetCount()), records.At(records.GetCount()));
		LCD_Clear();
		LCD_GOTO_XY(3, 0);
		LCD_Write_String(str);
		break;
		
		case END:
		LCD_Clear();
		LCD_Write_String("Waiting ");
		LCD_Write_Int(waiting_time);
		LCD_Write_String("sec");
			
	}
}

ISR(INT0_vect)			// 게임 시작 버튼
{
	if(game_mode == SELECT)
	{
		game_mode = PREPARING;
		waiting_time = 5;
	}
}

ISR(INT1_vect)			// 기록 보기 버튼
{
	uint8_t validNum = records.GetValidNum();
	if (validNum == 0)
	{
		return;
	}
	
	switch (game_mode)
	{
		case SELECT:
			records.SetCount(validNum - 1);
			game_mode = RECORDS;
			break;
		case RECORDS:
			game_mode = SELECT;
			break;
	}
}

ISR(INT2_vect)			// 중지 버튼
{
	point_user = 0;
	switch (game_mode)
	{
		case PREPARING:
			game_mode = SELECT;
			waiting_time = 0;
			break;
		case PLAYING:
			game_mode = SELECT;
			remaining_time = 0;
			break;
	}
}

ISR(INT3_vect)			// 점수 get버튼
{
	if(game_mode == PLAYING)
		point_user++;
}

int main(void)
{
	DDRA = 0xFF; DDRF = 0xFF; DDRC = 0xFF; DDRG = 0xFF;
	sei();
	
	Program_Init();
	Timer0_Init();
	Timer1_Init();
	Timer3_Init();
    External_Init();
	
    while (1) 
    {
    }
}

